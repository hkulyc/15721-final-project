import pglast
import json
import yaml
import re
from pathlib import Path
from .utils import Udf_Type

# GLOBAL const variables
root_path = Path(__file__).parent


class GV:
    # GLOBAL VERSITILE variables
    function_count = 0
    global_macros = ["/* GLOBAL MACROS */"]  # also has includes
    global_variables = ["/* GLOBAL VARIABLES */"]
    # non-udf utility functions (maybe need it)
    global_functions = ["/* GLOBAL FUNCTIONS */"]
    temp_count = 0
    func_args = []  # list of (arg_name, Udf_Type)
    func_vars = {}  # variables in the current function
    # will contain a dictionary with mapping from var_name -> (Udf_Type, initialized?)
    func_name = ""
    func_return_type: Udf_Type = None


################ Configurations ################
with open(root_path/'query.yaml', 'r') as file:
    query_config = yaml.safe_load(file)

with open(root_path/'function.yaml', 'r') as file:
    function_config = yaml.safe_load(file)

################################################


def new_function():
    "create a new function, return the function name"
    GV.function_count += 1
    return 'query'+str(GV.function_count)


def translate_query(query: dict, return_type: str = None) -> tuple[str, str]:
    """
    Transpile a query statement.
    This will return a string that initializes the temp, and the temp name.
    """

    params = {
        'db_name': 'db',
        'function_name': None,
        'query_formatter': None,
        'return_type': None,
        'function_name': new_function(),
        'function_args': None,
        'prepare_args': None
    }
    # print(query_config['global'].format(**params))
    temp_str = f"t{GV.temp_count}"
    GV.temp_count += 1
    return f"initialization of {temp_str} = {query};", temp_str


def translate_return_stmt(return_stmt: dict) -> str:
    """;
    Transpile a return statement from PL/pgSQL to C++.
    """
    output = ""
    return output


def translate_if_stmt(if_stmt: dict) -> str:
    """
    Transpile an if statement from PL/pgSQL to C++.
    """
    output = ""
    return output


def translate_body(body: list) -> str:
    """
    Transpile a body from PL/pgSQL to C++.
    """
    output = ""
    for stmt in body:
        # Example, need to add more stmts
        if ("PLpgSQL_stmt_if" in stmt):
            output += translate_if_stmt(stmt["PLpgSQL_stmt_if"])
        elif ("PLpgSQL_stmt_return" in stmt):
            output += translate_return_stmt(stmt["PLpgSQL_stmt_return"])
        else:
            raise Exception("Unknown statement type, ", stmt)
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
                GV.func_vars[name] = (udf_type, True)
                GV.func_args.append((name, udf_type))
            else:
                if "default_val" in var:
                    query, assigned_temp = translate_query(
                        var["default_val"]["PLpgSQL_expr"]["query"])
                    initializations.append(
                        function_config["fdecl_var"].format(
                            var_init=query, type=udf_type.cpp_type, name=name,
                            var=assigned_temp)
                    )
                    GV.func_vars[name] = (udf_type, True)
                else:
                    initializations.append(
                        function_config["finit_var"].format(
                            type=udf_type.cpp_type, name=name)
                    )
                    GV.func_vars[name] = (udf_type, False)

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
        type=a[1].cpp_type, name=a[0]), GV.func_args)
    params = {
        "return_type": GV.func_return_type.cpp_type,
        "function_name": GV.func_name,
        "function_args": ", ".join(args_str),
        "initializations": "\n".join(initializations)
    }
    output += function_config['fshell'].format(**params)
    params = {
        "cpp_ret_type": GV.func_return_type.cpp_type,
        "cpp_arg_types": ", ".join(map(lambda a: a[1].cpp_type, GV.func_args)),
        "duck_ret_type": GV.func_return_type.get_cpp_sqltype(),
        "duck_arg_types": ", ".join(map(lambda a: a[1].get_cpp_sqltype(), GV.func_args)),
        "func_name": GV.func_name
    }
    output += "\n"+function_config['fcreate'].format(**params)
    # TODO: Reenable translate_action
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
            GV.func_args = []
            GV.func_vars = {}
            GV.func_name = function_names[idx]
            GV.func_return_type = Udf_Type(return_types[idx], udf_str)
            cpp_str += translate_function(
                function["PLpgSQL_function"], udf_str)
            cpp_str += "\n\n"

    return "\n".join(GV.global_macros + GV.global_variables + GV.global_functions) + "\n" + cpp_str
