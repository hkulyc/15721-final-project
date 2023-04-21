import pglast
import json
from pglast.visitors import Visitor

def traverse_ast(ast: pglast.ast.SelectStmt, sql: str):
    "traverse the ast and modify the query"
    # doing a dfs traversal

class QueryTraverser(Visitor):
    def __init__(self):
        self.sql_modifier = []

    def sql_modifier_callback(self, node):
        "Callback function to modify the sql"
        if(len(node.fields) != 1):
            raise Exception('Only one field is allowed: '+node())
        self.sql_modifier.append((node.location, node.fields[0].val))

    def rules(self, node):
        if(type(node) == pglast.ast.ColumnRef):
            self.sql_modifier_callback(node)
        # more rules
        return
    
    def visit(self, ancestors, node):
        # print(ancestors, ':', node(depth=0, skip_none=True))
        self.rules(node)


def parse_sql_query(sql: str):
    "Parse a sql query with restrictions."
    try:
        # translate_query(None, None)
        tmp = pglast.parse_sql(sql)
        # print(ast)
        if(len(tmp) != 1):
            raise Exception('Only one query is allowed at a time')
        ast = tmp[0].stmt
        if(type(ast) != pglast.ast.SelectStmt):
            raise Exception('Only select statement is allowed')
        if(ast.fromClause is not None):
            raise Exception('Does not support select ... from ...')
        return tmp
        # ast = json.loads(ast_str)
        # with open("parse_sql_dump.json", "w") as f:
        #     f.write(json.dumps(ast, indent=2))
    except pglast.parser.ParseError as e:
        raise Exception("Failed to parse sql query: ", e)
        return
    
def prepare_statement(query: str, vars: dict):
    "transform a sql query with possible variable to prepared statement"
    # first try to parse the query (trial run)
    try:
        pglast.parse_sql(query)
    except pglast.parser.ParseError as e:
        query = 'select '+query
    res = parse_sql_query(query)
    trvr = QueryTraverser()
    trvr(res)
    # print(trvr.sql_modifier)
    # print(query)
    # sort the modifier rule and apply the rules in order
    modifier = sorted(trvr.sql_modifier, key=lambda x: x[0])
    args = []
    pre = 0
    prep_statement = []
    var_map = {}
    for loc, var in modifier:
        if var not in vars:
            raise Exception('Variable {} is not defined'.format(var))
        prep_statement.append(query[pre:loc])
        if var not in var_map:
            prep_statement.append('$'+str(len(args)+1))
            var_map[var] = '$'+str(len(args)+1)
            args.append(var)
        else:
            prep_statement.append(var_map[var])
        pre = loc + len(var)
    prep_statement.append(query[pre:])
    # print(''.join(prep_statement))
    return ''.join(prep_statement), args