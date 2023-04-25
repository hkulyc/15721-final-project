import pglast
import json
import yaml
import re
from pathlib import Path
from typing import Tuple
from .utils import Udf_Type, parse_assignment, is_const_or_var, function_arg_decl, dbg_assert, substitute_variables, is_loop_tempvar
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
    func_args = []  # list of (arg_name, Udf_Type)
    func_vars = {}  # variables in the current function
    # will contain a dictionary with mapping from var_name -> (Udf_Type, initialized?)
    func_name = ""
    func_return_type: Udf_Type = None
    query_macro = False

    temp_var_count = 0 # fresh variables created for for-loops
    temp_var_substitutes = {} # dictionary mapping var_name and their temp var substitutes
gv = GV()

def new_function():
    "create a new function, return the function name"
    gv.function_count += 1
    return 'query'+str(gv.function_count)
    
def new_variable():
    "create a new variable, return the variable name"
    gv.temp_var_count += 1
    return 'tempvar'+str(gv.temp_var_count) # no variables can be prefixed with tempvar

def translate_query(query: str, expected_type: Udf_Type, query_is_assignment : bool = False) -> str:
    """
    Transpile a query statement.
    This will return a string that initializes the temp, and the temp name.
    """
    # variable value assignment
    query = query.strip()
    query = substitute_variables(query,gv.temp_var_substitutes)
    if query_is_assignment:
        leftv, rightv = parse_assignment(query, gv.func_vars)

        if is_loop_tempvar(leftv): # ignores assignment to for-loop variable
            return ''

        if is_const_or_var(rightv, gv.func_vars):
            return '{} = {};'.format(leftv, rightv)
        prep_statement, args = prepare_statement(rightv, gv.func_vars)
        params = {
            'db_name': 'db',
            'query_formatter': prep_statement,
            'return_type': gv.func_vars[leftv][0].cpp_type,
            'function_name': new_function(),
            'function_args': function_arg_decl(args, gv.func_vars),
            'prepare_args': ', '.join(args)
        }
        if not gv.query_macro:
            gv.global_macros.append(query_config['macro'].format(**params))
            gv.query_macro = True
        gv.global_variables.append(query_config['global'].format(**params))
        gv.global_functions.append(query_config['function'].format(**params))
        return '{} = {};'.format(leftv, query_config['function_call'].format(**params))

    # directly returning a value based on the expected_type
    else:
        if is_const_or_var(query, gv.func_vars):
            return '{}'.format(query)
        prep_statement, args = prepare_statement(query, gv.func_vars)
        params = {
            'db_name': 'db',
            'query_formatter': prep_statement,
            'return_type': expected_type.cpp_type,
            'function_name': new_function(),
            'function_args': function_arg_decl(args, gv.func_vars),
            'prepare_args': ', '.join(args)
        }
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

def translate_expr(expr: dict, expected_type: Udf_Type, query_is_assignment : bool = False) -> str:
    # print("translate_expr()",expr)
    if "query" in expr and len(expr) == 1:
        return translate_query(expr['query'], expected_type, query_is_assignment)
    else:
        raise Exception('Unsupport PLpgSQL_expr: {}'.format(expr))
    
def translate_assign_stmt(stmt: dict) -> str:
    # example: {'lineno': 5, 'varno': 3, 'expr': {'PLpgSQL_expr': {'query': 'pd2 := pd'}}
    # other possible fields: lineno, varno
    stmt = stmt['expr']
    dbg_assert("PLpgSQL_expr" in stmt, 'assignment must be in form of expression')
    

    return translate_expr(stmt["PLpgSQL_expr"], None, True)


def translate_return_stmt(return_stmt: dict) -> str:
    """;
    Transpile a return statement from PL/pgSQL to C++.
    """
    # supported field lineno, expr
    dbg_assert(len(return_stmt) == 2, "return_stmt should only have lineno and expr")
    output = "return "
    return output+translate_body([return_stmt['expr']], gv.func_return_type)+';\n'


def translate_if_stmt(if_stmt: dict) -> str:
    """
    Transpile an if statement from PL/pgSQL to C++.
    """
    output = ""
    return output

def translate_loop_stmt(loop_stmt: dict) -> str:
    """
    Transpile a simple loop statement from PL/pgSQL to C++.
    """
    output = ""
    params = {
        "body": translate_body(loop_stmt['body'])
    }
    output += control_config['simple'].format(**params)
    return output

def translate_for_stmt(for_stmt: dict) -> str:
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

    var = for_stmt["var"]["PLpgSQL_var"]
    name = var["refname"]

    if name in gv.temp_var_substitutes:
        prev_sub = gv.temp_var_substitutes[name]
    else:
        prev_sub = None

    temp_var_name = new_variable()
    gv.temp_var_substitutes[name] = temp_var_name
    dbg_assert(temp_var_name not in gv.func_vars and temp_var_name not in gv.temp_var_substitutes, f"temporary loop variable {temp_var_name} cannot be used elsewhere")
    gv.func_vars[temp_var_name] = (Udf_Type("INTEGER"), False)

    is_reverse = "reverse" in for_stmt
    if is_reverse:
        dbg_assert(for_stmt["reverse"],"reverse must be true")

    step_size = translate_expr(for_stmt["step"]["PLpgSQL_expr"], Udf_Type("INTEGER"), False) if "step" in for_stmt else "1"
    try:
        dbg_assert(int(step_size) > 0, "Step size must be positive")
    except ValueError:
        raise Exception("step_size is not integer")

    params = {
        "body": translate_body(for_stmt['body']),
        "start": translate_expr(for_stmt["lower"]["PLpgSQL_expr"], Udf_Type("INTEGER"), False),
        "end": translate_expr(for_stmt["upper"]["PLpgSQL_expr"], Udf_Type("INTEGER"), False),
        "name": temp_var_name,
        "step":  step_size,
    }
    output += control_config['revfor' if is_reverse else 'for'].format(**params)

    del gv.func_vars[temp_var_name]

    if prev_sub is None:
        del gv.temp_var_substitutes[name]
    else:
        gv.temp_var_substitutes[name] = prev_sub

    return output

def translate_while_stmt(while_stmt: dict) -> str:
    """
    Transpile a while loop statement from PL/pgSQL to C++.
    """
    output = ""

    params = {
        "body": translate_body(while_stmt['body']),
        "condition": translate_expr(while_stmt["cond"]["PLpgSQL_expr"], Udf_Type("BOOLEAN"), False),
    }
    output += control_config['while'].format(**params)
    return output

def translate_exitcont_stmt(exitcont_stmt: dict) -> str:
    """
    Transpile a exit/continue statement from PL/pgSQL to C++.
    """

    if "is_exit" in exitcont_stmt:
       dbg_assert(exitcont_stmt["is_exit"],"is_exit must be true")
       return "break;"
    else:
       return "continue;"
 

def translate_body(body: list, expected_type: Udf_Type = None) -> str:
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
            output += translate_if_stmt(stmt["PLpgSQL_stmt_if"])
        elif ("PLpgSQL_stmt_return" in stmt):
            output += translate_return_stmt(stmt["PLpgSQL_stmt_return"])
        
        elif ("PLpgSQL_expr" in stmt):
            output += translate_expr(stmt["PLpgSQL_expr"], expected_type, False)

        elif ("PLpgSQL_stmt_assign" in stmt):
            output += translate_assign_stmt(stmt["PLpgSQL_stmt_assign"])
        
        # loop
        elif ("PLpgSQL_stmt_loop" in stmt):
            output += translate_loop_stmt(stmt["PLpgSQL_stmt_loop"])
        elif ("PLpgSQL_stmt_fori" in stmt):
            output += translate_for_stmt(stmt["PLpgSQL_stmt_fori"])
        elif ("PLpgSQL_stmt_while" in stmt):
            output += translate_while_stmt(stmt["PLpgSQL_stmt_while"])
        elif ("PLpgSQL_stmt_exit" in stmt):
            output += translate_exitcont_stmt(stmt["PLpgSQL_stmt_exit"])

        else:
            raise Exception("Unknown statement type: {}".format(str(stmt)))
        
        # TODO: support EXIT and CONTINUE
        
        is_first = False
    return output


def translate_stmt_block(stmt_block: dict) -> str:
    """
    Transpile a statement block from PL/pgSQL to C++.
    """
    output = ""
    output += translate_body(stmt_block["body"])
    return output


def translate_action(action: dict) -> str:
    """
    Transpile an action from PL/pgSQL to C++.
    """
    output = ""
    output += translate_stmt_block(action["PLpgSQL_stmt_block"])
    return output


def get_function_vars(datums: list, udf_str: str) -> Tuple[list, list]:
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
                        var["default_val"]["PLpgSQL_expr"], udf_type)
                    initializations.append(
                        function_config["fdecl_var"].format(
                            type=udf_type.cpp_type, name=name,
                            var=query_result)
                    )
                    gv.func_vars[name] = (udf_type, True)
                else:
                    initializations.append(
                        function_config["finit_var"].format(
                            type=udf_type.cpp_type, name=name)
                    )
                    gv.func_vars[name] = (udf_type, False)

        else:
            raise Exception("Unknown datum field, ", datum)
    return initializations


def translate_function(function: dict, udf_str: str) -> str:
    """
    Transpile a function from PL/pgSQL to C++.
    Returns a string with the transpiled function.
    """
    # Get function args
    output = ""
    initializations = get_function_vars(function["datums"], udf_str)
    args_str = map(lambda a: function_config["farg"].format(
        type=a[1].cpp_type, name=a[0]), gv.func_args)
    params = {
        "return_type": gv.func_return_type.cpp_type,
        "function_name": gv.func_name,
        "function_args": ", ".join(args_str),
        "initializations": "\n".join(initializations),
        "body": translate_action(function["action"])
    }
    output += function_config['fshell'].format(**params)
    params = {
        "cpp_ret_type": gv.func_return_type.cpp_type,
        "cpp_arg_types": ", ".join(map(lambda a: a[1].cpp_type, gv.func_args)),
        "duck_ret_type": gv.func_return_type.get_cpp_sqltype(),
        "duck_arg_types": ", ".join(map(lambda a: a[1].get_cpp_sqltype(), gv.func_args)),
        "func_name": gv.func_name
    }
    output += "\n"+function_config['fcreate'].format(**params)
    return output


def translate_plpgsql_udf_str(udf_str: str) -> str:
    try:
        # translate_query(None, None)
        ast_str = pglast.parser.parse_plpgsql_json(udf_str)
        ast = json.loads(ast_str)
        with open("parse_dump.json", "w") as f:
            f.write(json.dumps(ast, indent=2))
    except pglast.parser.ParseError as e:
        print("Failed to parse UDF: ", e)
        return

    cpp_str = ""

    return_types = re.findall(r"RETURNS\s+(\w+)", udf_str, flags=re.IGNORECASE)
    function_names = re.findall(
        r"CREATE\s+FUNCTION\s+(\w+)", udf_str, flags=re.IGNORECASE)
    if len(return_types) < len(ast):
        raise Exception("Return type not specified for all functions")
    if len(function_names) < len(ast):
        raise Exception("Function name not specified for all functions")

    for idx, function in enumerate(ast):
        if ("PLpgSQL_function" in function):
            gv.func_args = []
            gv.func_vars = {}
            gv.func_name = function_names[idx]
            gv.func_return_type = Udf_Type(return_types[idx], udf_str)
            cpp_str += translate_function(
                function["PLpgSQL_function"], udf_str)
            cpp_str += "\n\n"

    return "\n".join(gv.global_macros + gv.global_variables + gv.global_functions) + "\n" + cpp_str
