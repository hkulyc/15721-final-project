import pglast
import json

def parse_sql_query(sql: str):
    "Parse a normal sql query"
    try:
        # translate_query(None, None)
        ast = pglast.parse_sql(sql)
        print(ast)
        # ast = json.loads(ast_str)
        # with open("parse_sql_dump.json", "w") as f:
        #     f.write(json.dumps(ast, indent=2))
    except pglast.parser.ParseError as e:
        raise Exception("Failed to parse UDF: ", e)
        return
    
def prepare_statement(query: str, vars: dict):
    "transform a sql query with possible variable to prepared statement"
    parse_sql_query('select '+query)
    return None, None