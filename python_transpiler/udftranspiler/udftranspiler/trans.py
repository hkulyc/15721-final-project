import pglast
import json
import yaml
import re
from pathlib import Path
from .utils import Udf_Type, is_assignment, parse_assignment, is_const_or_var, function_arg_decl, dbg_assert
from .query import prepare_statement

# GLOBAL const variables
root_path = Path(__file__).parent

################ Configurations ################
with open(root_path/'query.yaml', 'r') as file:
    query_config = yaml.safe_load(file)

with open(root_path/'function.yaml', 'r') as file:
    function_config = yaml.safe_load(file)

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
gv = GV()

def new_function():
    "create a new function, return the function name"
    gv.function_count += 1
    return 'query'+str(gv.function_count)
    

def translate_query(query: str, expected_type: Udf_Type) -> str:
    """
    Transpile a query statement.
    This will return a string that initializes the temp, and the temp name.
    """
    # variable value assignment
    query = query.strip()
    if is_assignment(query):
        leftv, rightv = parse_assignment(query, gv.func_vars)
        if is_const_or_var(rightv, gv.func_vars):
            return '{} = {}\n'.format(leftv, rightv)
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
        return '{} = {};\n'.format(leftv, query_config['function_call'].format(**params))

    # directly returning a value based on the expected_type
    else:
        if is_const_or_var(query, gv.func_vars):
            return '{}\n'.format(query)
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

def translate_expr(expr: dict, expected_type: Udf_Type) -> str:
    if "query" in expr and len(expr) == 1:
        return translate_query(expr['query'], expected_type)
    else:
        raise Exception('Unsupport PLpgSQL_expr: {}'.format(expr))
    
def translate_assign_stmt(stmt: dict) -> str:
    # example: {'lineno': 5, 'varno': 3, 'expr': {'PLpgSQL_expr': {'query': 'pd2 := pd'}}
    # other possible fields: lineno, varno
    return translate_body((stmt['expr'],))


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


def translate_body(body: list, expected_type: Udf_Type = None) -> str:
    """
    Transpile a body from PL/pgSQL to C++.\n
    This function might be called recursively inside trans_*. If there is called
    by return type determined stmts like "cond":{"PLpgSQL_expr"..., should set 
    argument expected_type.
    """
    output = ""
    for stmt in body:
        # Example, need to add more stmts
        if ("PLpgSQL_stmt_if" in stmt):
            output += translate_if_stmt(stmt["PLpgSQL_stmt_if"])
        elif ("PLpgSQL_stmt_return" in stmt):
            output += translate_return_stmt(stmt["PLpgSQL_stmt_return"])
        elif ("PLpgSQL_expr" in stmt):
            output += translate_expr(stmt["PLpgSQL_expr"], expected_type)
        elif ("PLpgSQL_stmt_assign" in stmt):
            output += translate_assign_stmt(stmt["PLpgSQL_stmt_assign"])
        else:
            raise Exception("Unknown statement type: {}".format(str(stmt)))
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


def get_function_vars(datums: list, udf_str: str) -> tuple[list, list]:
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
                    # query, assigned_temp = translate_query(
                    #     var["default_val"]["PLpgSQL_expr"]["query"], udf_type)
                    # initializations.append(
                    #     function_config["fdecl_var"].format(
                    #         var_init=query, type=udf_type.cpp_type, name=name,
                    #         var=assigned_temp)
                    # )
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
        "initializations": "\n".join(initializations)
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
    # # TODO: Reenable translate_action
    # output += translate_action(function["action"])
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
