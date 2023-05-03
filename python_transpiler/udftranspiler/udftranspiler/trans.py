import pglast
import json
import yaml
import re
from pathlib import Path
from typing import Tuple
from .utils import Udf_Type, ActiveLanes, parse_assignment, is_const, is_var, reformat_sql_string, dbg_assert, substitute_variables, is_loop_tempvar, add_identition
from .query import prepare_statement


# GLOBAL const variables
root_path = Path(__file__).parent

################ Configurations ################
with open(root_path/'query.yaml', 'r') as file:
    query_config = yaml.safe_load(file)

with open(root_path/'function.yaml', 'r') as file:
    function_config = yaml.safe_load(file)

with open(root_path/'control.yaml', 'r') as file:
    control_config = yaml.safe_load(file)

################################################

# GLOBAL VERSITILE variables


class GV:
    function_count = 0

    global_macros = ["/* GLOBAL MACROS */"]  # also has includes
    global_variables = ["/* GLOBAL VARIABLES */"]
    # non-udf utility functions (maybe need it)
    global_functions = ["/* GLOBAL FUNCTIONS */"]
    # temp_count = 0
    vector_size = 2048
    func_args = []  # list of (arg_name, Udf_Type)
    func_vars = {}  # variables in the current function
    # will contain a dictionary with mapping from var_name -> (Udf_Type, initialized?)
    func_name = ""
    func_return_type: Udf_Type = None
    query_macro = False

    temp_var_count = 0  # fresh variables created for for-loops
    temp_var_substitutes = {}  # dictionary mapping var_name and their temp var substitutes


gv = GV()


def new_function():
    "create a new function, return the function name"
    gv.function_count += 1
    return 'query'+str(gv.function_count)


def new_variable():
    "create a new variable, return the variable name"
    gv.temp_var_count += 1
    # no variables can be prefixed with tempvar
    return 'tempvar'+str(gv.temp_var_count)


def translate_query(query: str, active_lanes: ActiveLanes, query_is_assignment: bool) -> str:
    """
    Transpile a query statement.
    This will return a string that initializes the temp, and the temp name.
    """
    # if assignment, only parse the right value
    # print(query)
    if query_is_assignment:
        query = parse_assignment(query, gv.func_vars)[1]
    query = query.strip()
    # todo: this substitute is suspectable to errors
    # query = substitute_variables(query, gv.temp_var_substitutes)

    if is_const(query, gv.func_vars):
        return '{}({})'.format('const_vector_gen', reformat_sql_string(query))
    if is_var(query, gv.func_vars):
        return query
    prep_statement, args = prepare_statement(
        query, gv.func_vars, gv.vector_size, gv.temp_var_substitutes)

    function_arg_temp = query_config['function_arg']
    function_args = ', '.join([function_arg_temp.format(i)
                              for i in range(len(args))])
    params = {
        'db_name': 'db',
        'prep_statement': prep_statement,
        'function_name': new_function(),
        'function_args': function_args,
        'prepare_args': ', '.join(args+active_lanes.get_mask_pointers()),
        'vector_size': gv.vector_size,
        # 'args_count': len(args),
        'input_size': gv.vector_size*len(args)
    }
    params['prepare_input'] = add_identition('\n'.join(
        [query_config['prepare_input'].format(**params, arg=arg) for arg in range(len(args))]))
    if not gv.query_macro:
        gv.global_macros.append(query_config['macro'].format(**params))
        gv.query_macro = True
    gv.global_variables.append(query_config['global'].format(**params))
    gv.global_functions.append(query_config['function'].format(**params))
    return '{}'.format(query_config['function_call'].format(**params))

    # print(query_config['global'].format(**params))
    # temp_str = f"t{gv.temp_count}"
    # gv.temp_count += 1
    # return f"initialization of {temp_str} = {query};", temp_str
    return


def translate_expr(expr: dict, active_lanes: ActiveLanes, query_is_assignment: bool = False) -> str:
    # print("translate_expr()",expr)
    if "query" in expr and len(expr) == 1:
        return translate_query(expr['query'], active_lanes, query_is_assignment)
    else:
        raise Exception('Unsupport PLpgSQL_expr: {}'.format(expr))


def translate_assign_stmt(stmt: dict, active_lanes: ActiveLanes) -> str:
    # example: {'lineno': 5, 'varno': 3, 'expr': {'PLpgSQL_expr': {'query': 'pd2 := pd'}}
    # other possible fields: lineno, varno
    stmt = stmt['expr']
    dbg_assert("PLpgSQL_expr" in stmt,
               'assignment must be in form of expression')
    # todo: need to prepare the left value and '='
    
    left = parse_assignment(stmt["PLpgSQL_expr"]['query'], gv.func_vars)[0]

    return_loop_mask_conditions = ""
    if active_lanes.get_loop_active() is not None:
        params = {
            "loop_active_var": active_lanes.get_loop_active(),
            "continued_var": active_lanes.get_continues(),
        }
        return_loop_mask_conditions = control_config['return_loop_cond'].format(
            **params)
    params = {
        "temp_var": new_variable(),
        "i_var": new_variable(),
        "active_var": active_lanes.get_active(),
        "return_mask_var": active_lanes.get_returns(),
        "return_loop_mask_conditions": return_loop_mask_conditions,
        "assigned_var": left,
        "expr": translate_expr(stmt["PLpgSQL_expr"], active_lanes, True),
    }
    return control_config['assign'].format(**params)


def translate_return_stmt(return_stmt: dict, active_lanes: ActiveLanes) -> str:
    """;
    Transpile a return statement from PL/pgSQL to C++.
    """
    # supported field lineno, expr
    dbg_assert(len(return_stmt) == 2,
               "return_stmt should only have lineno and expr")
    return_loop_mask_conditions = ""
    if active_lanes.get_loop_active() is not None:
        params = {
            "loop_active_var": active_lanes.get_loop_active(),
            "continued_var": active_lanes.get_continues(),
        }
        return_loop_mask_conditions = control_config['return_loop_cond'].format(
            **params)
    params = {
        "i_var": new_variable(),
        "return_exp_var": new_variable(),
        "return_exp": translate_body([return_stmt['expr']], active_lanes, gv.func_return_type),
        "active_var": active_lanes.get_active(),
        "return_mask_var": active_lanes.get_returns(),
        "return_loop_mask_conditions": return_loop_mask_conditions,
    }
    return control_config['return'].format(**params)


def translate_if_stmt(if_stmt: dict, active_lanes: ActiveLanes) -> str:
    """
    Transpile an if statement from PL/pgSQL to C++.
    """
    output = ""
    dbg_assert("cond" in if_stmt, "if_stmt should have cond")
    cond_exp = translate_body(
        [if_stmt['cond']], active_lanes, Udf_Type("BOOLEAN"))
    then_body = ""
    else_ = ""

    input_active = active_lanes.get_active()
    new_active = active_lanes.new_active()

    if "then_body" in if_stmt:
        then_body = translate_body(if_stmt['then_body'], active_lanes)

    if "elsif_list" in if_stmt and len(if_stmt['elsif_list']) > 0:
        if_stmt_copy = if_stmt.copy()
        first_elif = if_stmt_copy['elsif_list'].pop(0)
        if_stmt_copy['cond'] = first_elif['PLpgSQL_if_elsif']['cond']
        if "stmts" in first_elif['PLpgSQL_if_elsif']:
            if_stmt_copy['then_body'] = first_elif['PLpgSQL_if_elsif']['stmts']
        else:
            if_stmt_copy.pop('then_body', None)
        else_ = control_config['else'].format(
            else_body=translate_if_stmt(if_stmt_copy, active_lanes))

    elif "else_body" in if_stmt:
        else_ = control_config['else'].format(
            else_body=translate_body(if_stmt['else_body'], active_lanes))

    params = {
        "condition": cond_exp,
        "i_var": new_variable(),
        "cond_var": new_variable(),
        "active_var": new_variable(),
        "input_active_lanes": input_active,
        "active_var": new_active,
        "then_body": then_body,
        "else": else_
    }
    output += control_config['if_block'].format(**params)

    active_lanes.pop_active()
    return output


def translate_loop_stmt(loop_stmt: dict, active_lanes: ActiveLanes) -> str:
    """
    Transpile a simple loop statement from PL/pgSQL to C++.
    """
    new_loop_active = active_lanes.new_loop_active()
    new_continues = active_lanes.new_continues()
    output = ""
    params = {
        "i_var": new_variable(),
        "any_active_lane_var": new_variable(),
        "input_active_lanes": active_lanes.get_active(),
        "return_mask_var": active_lanes.get_returns(),
        "loop_active_var": new_loop_active,
        "continued_var": new_continues,
        "body": translate_body(loop_stmt['body'], active_lanes)
    }
    output += control_config['simple'].format(**params)
    active_lanes.pop_loop_active()
    active_lanes.pop_continues()
    return output


def translate_for_stmt(for_stmt: dict, active_lanes : ActiveLanes) -> str:
    """
    Transpile a for loop statement from PL/pgSQL to C++.
    """
    output = ""

    # TODO: support REVERSE

    """
    
    from postgres docs: 
        The variable name is automatically defined as type integer and exists only 
        inside the loop (any existing definition of the variable name is ignored 
        within the loop). 

    to handle this behavior:    
        - `temp_var_name` substitutes all instance of `name` variable
        - to handle nested loops with same loop variable name,
          gv.temp_var_substitutes will be reversed upon loop exit
        - also note that assignment to `temp_var_name` variables are ignored
          because this has no effect on plpgsql for loops

        also see translate_query()

    """

    new_loop_active = active_lanes.new_loop_active()
    new_continues = active_lanes.new_continues()

    var = for_stmt["var"]["PLpgSQL_var"]
    name = var["refname"]

    if name in gv.temp_var_substitutes:
        prev_sub = gv.temp_var_substitutes[name]
    else:
        prev_sub = None

    temp_var_name = new_variable()
    gv.temp_var_substitutes[name] = temp_var_name
    dbg_assert(temp_var_name not in gv.func_vars and temp_var_name not in gv.temp_var_substitutes,
               f"temporary loop variable {temp_var_name} cannot be used elsewhere")
    gv.func_vars[temp_var_name] = (Udf_Type("INTEGER"), False)

    is_reverse = "reverse" in for_stmt
    if is_reverse:
        dbg_assert(for_stmt["reverse"], "reverse must be true")

    step_size = int(for_stmt["step"]["PLpgSQL_expr"]["query"]) if "step" in for_stmt else "1"
    try:
        dbg_assert(int(step_size) > 0, "Step size must be positive")
    except ValueError:
        raise Exception("step_size is not integer")

    params = {
        "body": translate_body(for_stmt['body'], active_lanes),
        "start": for_stmt["lower"]["PLpgSQL_expr"]["query"],
        "end": for_stmt["upper"]["PLpgSQL_expr"]["query"],
        "name": temp_var_name,
        "step":  step_size,
        "i_var": new_variable(),
        "any_active_lane_var": new_variable(),
        "input_active_lanes": active_lanes.get_active(),
        "return_mask_var": active_lanes.get_returns(),
        "loop_active_var": new_loop_active,
        "continued_var": new_continues,
    }
    output += control_config['revfor' if is_reverse else 'for'].format(
        **params)

    del gv.func_vars[temp_var_name]

    if prev_sub is None:
        del gv.temp_var_substitutes[name]
    else:
        gv.temp_var_substitutes[name] = prev_sub

    active_lanes.pop_loop_active()
    active_lanes.pop_continues()
    return output


def translate_while_stmt(while_stmt: dict, active_lanes: ActiveLanes) -> str:
    """
    Transpile a while loop statement from PL/pgSQL to C++.
    """
    output = ""
    new_loop_active = active_lanes.new_loop_active()
    new_continues = active_lanes.new_continues()
    output = ""
    params = {
        "i_var": new_variable(),
        "any_active_lane_var": new_variable(),
        "cond_var": new_variable(),
        "input_active_lanes": active_lanes.get_active(),
        "return_mask_var": active_lanes.get_returns(),
        "loop_active_var": new_loop_active,
        "continued_var": new_continues,
        "condition": translate_expr(while_stmt["cond"]["PLpgSQL_expr"], active_lanes, False),
        "body": translate_body(while_stmt['body'], active_lanes)
    }
    output += control_config['while'].format(**params)
    active_lanes.pop_loop_active()
    active_lanes.pop_continues()
    return output


def translate_exitcont_stmt(exitcont_stmt: dict, active_lanes: ActiveLanes) -> str:
    """
    Transpile a exit/continue statement from PL/pgSQL to C++.
    """
    assert active_lanes.get_loop_active() is not None, "exit/continue must be inside a loop"
    assert active_lanes.get_continues() is not None, "exit/continue must be inside a loop"
    params = {
        "i_var": new_variable(),
        "active_var": active_lanes.get_active(),
        "continued_var": active_lanes.get_continues(),
        "return_mask_var": active_lanes.get_returns(),
        "loop_active_var": active_lanes.get_loop_active()
    }
    if "is_exit" in exitcont_stmt:
        dbg_assert(exitcont_stmt["is_exit"], "is_exit must be true")
        return control_config['break'].format(**params)
    else:
        return control_config['continue'].format(**params)


def translate_body(body: list, active_lanes: ActiveLanes, expected_type: Udf_Type = None) -> str:
    """
    Transpile a body from PL/pgSQL to C++.\n
    This function might be called recursively inside trans_*. If there is called
    by return type determined stmts like "cond":{"PLpgSQL_expr"..., should set 
    argument expected_type.
    """
    output = ""
    is_first = True
    for stmt in body:
        if not is_first:
            output += "\n"

        # Example, need to add more stmts
        if ("PLpgSQL_stmt_if" in stmt):
            output += translate_if_stmt(stmt["PLpgSQL_stmt_if"], active_lanes)
        elif ("PLpgSQL_stmt_return" in stmt):
            output += translate_return_stmt(
                stmt["PLpgSQL_stmt_return"], active_lanes)
        elif ("PLpgSQL_expr" in stmt):
            output += translate_expr(stmt["PLpgSQL_expr"], active_lanes, False)
        elif ("PLpgSQL_stmt_assign" in stmt):
            output += translate_assign_stmt(stmt["PLpgSQL_stmt_assign"], active_lanes)

        # loop
        elif ("PLpgSQL_stmt_loop" in stmt):
            output += translate_loop_stmt(
                stmt["PLpgSQL_stmt_loop"], active_lanes)
        elif ("PLpgSQL_stmt_fori" in stmt):
            output += translate_for_stmt(stmt["PLpgSQL_stmt_fori"], active_lanes)
        elif ("PLpgSQL_stmt_while" in stmt):
            output += translate_while_stmt(
                stmt["PLpgSQL_stmt_while"], active_lanes)
        elif ("PLpgSQL_stmt_exit" in stmt):
            output += translate_exitcont_stmt(
                stmt["PLpgSQL_stmt_exit"], active_lanes)

        else:
            raise Exception("Unknown statement type: {}".format(str(stmt)))

        # TODO: support EXIT and CONTINUE

        is_first = False
    return output


def translate_stmt_block(stmt_block: dict, active_lanes) -> str:
    """
    Transpile a statement block from PL/pgSQL to C++.
    """
    output = ""
    output += translate_body(stmt_block["body"], active_lanes)
    return output


def translate_action(action: dict, active_lanes) -> str:
    """
    Transpile an action from PL/pgSQL to C++.
    """
    output = ""
    output += translate_stmt_block(action["PLpgSQL_stmt_block"], active_lanes)
    return output


def get_function_vars(datums: list, udf_str: str, active_lanes : ActiveLanes) -> Tuple[list, list]:
    """
    Get the function arguments from the datums.
    Returns a tuple with a list of the formatted function arguments, and a
    list of initializations/declarations for function variables.
    """
    scanning_func_args = True
    initializations = []
    for datum in datums:
        if ("PLpgSQL_var" in datum):
            var = datum["PLpgSQL_var"]
            name = var["refname"]
            if (name == "found"):
                scanning_func_args = False
                continue
            typ = var["datatype"]["PLpgSQL_type"]["typname"]
            udf_type = Udf_Type(typ, udf_str)
            if (udf_type.is_unknown()):
                # variables with UNKNOWN types are created by for loops later in the code
                # We don't need to do anything with them here
                continue
            if (scanning_func_args):
                gv.func_vars[name] = (udf_type, True)
                gv.func_args.append((name, udf_type))
            else:
                if "default_val" in var:
                    query_result = translate_expr(
                        var["default_val"]["PLpgSQL_expr"], active_lanes, False)
                    initializations.append(
                        function_config["fdecl_var"].format(
                            name=name,
                            var=query_result)
                    )
                    gv.func_vars[name] = (udf_type, True)
                else:
                    initializations.append(
                        function_config["finit_var"].format(name=name)
                    )
                    gv.func_vars[name] = (udf_type, False)

        else:
            raise Exception("Unknown datum field, ", datum)
    return initializations


def translate_function(function: dict, udf_str: str, active_lanes: ActiveLanes) -> tuple[str]:
    """
    Transpile a function from PL/pgSQL to C++.
    Returns a string with the transpiled function.
    """
    # Get function args
    output = ""
    initializations = get_function_vars(function["datums"], udf_str, active_lanes)
    args_str = ""
    for i in range(len(gv.func_args)):
        name, _ = gv.func_args[i]
        params = {
            "arg_num": str(i),
            "name": name,
            "i_var": new_variable()
        }
        args_str += function_config["farg"].format(**params)
        args_str += "\n"
    return_mask_var = active_lanes.new_returns()
    active_lanes_var = active_lanes.new_active()
    params = {
        "return_mask_var": return_mask_var,
        "active_lanes_var": active_lanes_var,
        "function_name": gv.func_name,
        "function_args": args_str,
        "initializations": "\n".join(initializations),
        "body": translate_action(function["action"], active_lanes)
    }
    output += function_config['fshell'].format(**params)+"\n"
    decl = function_config['func_dec'].format(**params)
    params = {
        "duck_ret_type": gv.func_return_type.get_cpp_sqltype(),
        "duck_arg_types": ", ".join(map(lambda a: a[1].get_cpp_sqltype(), gv.func_args)),
        "func_name": gv.func_name
    }
    reg = function_config['fcreate'].format(**params)
    active_lanes.pop_returns()
    active_lanes.pop_active()
    return output, reg, decl


def translate_plpgsql_udf_str(udf_str: str) -> tuple[str]:
    """
    return:
    str1: main udf implementations
    str2: udf registration line
    """
    try:
        # translate_query(None, None)
        ast_str = pglast.parser.parse_plpgsql_json(udf_str)
        ast = json.loads(ast_str)
        with open("parse_dump.json", "w") as f:
            f.write(json.dumps(ast, indent=2))
    except pglast.parser.ParseError as e:
        raise Exception("Failed to parse UDF: ", e)
        return

    cpp_str = ""

    return_types = re.findall(r"RETURNS\s+(\w+)", udf_str, flags=re.IGNORECASE)
    function_names = re.findall(
        r"CREATE\s+FUNCTION\s+(\w+)", udf_str, flags=re.IGNORECASE)
    if len(return_types) < len(ast):
        raise Exception("Return type not specified for all functions")
    if len(function_names) < len(ast):
        raise Exception("Function name not specified for all functions")

    params = {
        'db_name': 'db',
        'vector_size': 2048 #todo
    }
    gv.global_macros.append(query_config['macro'].format(**params))
    gv.query_macro = True

    regs = []
    decls = []
    for idx, function in enumerate(ast):
        if ("PLpgSQL_function" in function):
            gv.func_args = []
            gv.func_vars = {}
            gv.func_name = function_names[idx]
            gv.func_return_type = Udf_Type(return_types[idx], udf_str)
            func_ret = translate_function(
                function["PLpgSQL_function"], udf_str, ActiveLanes())
            cpp_str += func_ret[0]
            cpp_str += "\n\n"
            regs.append(func_ret[1])
            decls.append(func_ret[2])
    return "\n".join(gv.global_macros + gv.global_variables + gv.global_functions) + "\n" + cpp_str, '\n'.join(regs), '\n'.join(decls)
