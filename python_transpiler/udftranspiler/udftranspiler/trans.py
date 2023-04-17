import pglast
import json
import yaml
from pathlib import Path

# GLOBAL const variables
root_path = Path(__file__).parent

# GLOBAL VERSITILE variables
function_count = 0
functions = {"query"}

with open(root_path/'query.yaml', 'r') as file:
    query_config = yaml.safe_load(file)

def translate_query(query: dict, return_type:str = None) -> str:
    """
    Transpile a query statement
    """
    
    params = {
        'db_name': 'db',
        'function_name': None,
        'query_formatter': None,
        'return_type': None,
        'function_name': None,
        'function_args': None,
        'prepare_args': None
    }


def translate_return_stmt(return_stmt: dict) -> str:
    """
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


def translate_function(function: dict) -> str:
    """
    Transpile a function from PL/pgSQL to C++.
    """
    output = ""
    # TODO: handle function arguments
    output += translate_action(function["action"])
    return output


def translate_plpgsql_udf_str(udf_str: str) -> str:
    try:
        ast_str = pglast.parser.parse_plpgsql_json(udf_str)
        print(ast_str)
        ast = json.loads(ast_str)
    except pglast.parser.ParseError as e:
        print("Failed to parse UDF: ", e)
        return

    cpp_str = "Need to implement"

    for function in ast:
        if ("PLpgSQL_function" in function):
            cpp_str += translate_function(function["PLpgSQL_function"])

    return cpp_str
