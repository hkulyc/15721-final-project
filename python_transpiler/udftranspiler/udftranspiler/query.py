import pglast
import json
from pglast.visitors import Visitor
from .utils import *

def traverse_ast(ast: pglast.ast.SelectStmt, sql: str):
    "traverse the ast and modify the query"
    # doing a dfs traversal

class QueryTraverser(Visitor):
    def __init__(self, vars: dict, subs: dict):
        self.sql_modifier = []
        self.vars = vars
        self.subs = subs

    def sql_modifier_callback(self, ancestors, node):
        "Callback function to modify the sql"
        if(len(node.fields) != 1):
            raise Exception('Only one field is allowed: '+node())
        self.sql_modifier.append((node.location, node.fields[0].val))

    def can_compile(self, node):
        # print(type(node))
        try:
            node_type = type(node)
            if node_type == pglast.ast.String:
                return False
            elif node_type == pglast.ast.ColumnRef:
                # print(node.fields)
                val = node.fields[0].val
                if val in self.subs:
                    # print(self.vars.get(val))
                    val = self.subs[val]
                    node.extra['logical_type'] = [self.vars.get(val)[0]]
                elif val in self.vars:
                    # print(type(self.vars.get(val)[0]),self.vars.get(val)[0])
                    node.extra['logical_type'] = [self.vars.get(val)[0].duckdb_type]
                else:
                    return False
                dbg_assert(None not in node.extra['logical_type'], 'Var logical type wrong')
                return True
            elif node_type == pglast.ast.A_Const:
                node.extra['logical_type'] = node.val
                return True
            elif node_type == pglast.ast.A_Expr:
                dbg_assert(len(node.name) == 1, 'A_Expr has more than one name: {}'.format(node.name))
                l_type = node.lexpr.extra['logical_type']
                r_type = node.rexpr.extra['logical_type']
                name = node.name[0].val
                # + / -
                if (name == '+' or name == '-'):
                    res_type = add_match(l_type, r_type)
                    if res_type:
                        node.extra['logical_type'] = res_type
                        return True
                    else: return False
                
                # > / < / >= / <=
                elif (name == '>' or name == '<' or name == '>=' or name == '<='):
                    res_type = comparison_match(l_type, r_type)
                    if res_type:
                        node.extra['logical_type'] = res_type
                        return True
                    else: return False

                else:
                    return False
                
                
            elif node_type == pglast.ast.ResTarget:
                return node.val.extra.get('compile', False)
            elif node_type == pglast.ast.SelectStmt:
                for v in node.targetList:
                    if not v.extra.get('compile', False):
                        return False
                return True
            else:
                return False
        except Exception as e:
            # print(e)
            return False
        
    def compile_callback(self, node):
        if(self.can_compile(node)):
            node.extra['compile'] = True
        else:
            if hasattr(node, 'extra'):
                node.extra.clear()
                node.extra['compile'] = False

    def rules(self, ancestors, node):
        if(type(node) == pglast.ast.ColumnRef):
            self.sql_modifier_callback(ancestors, node)
        self.compile_callback(node)

        # more rules
        return
    
    def visit(self, ancestors, node):
        self.rules(ancestors, node)
        # print(ancestors, ':', node(depth=0, skip_none=True))


def parse_sql_query(sql: str):
    "Parse a sql query with restrictions."
    # print(sql)
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
    
def prepare_statement(query: str, vars: dict, vector_size: int, substitutes: dict):
    "transform a sql query with possible variable to prepared statement"
    # first try to parse the query (trial run)
    try:
        pglast.parse_sql(query)
    except pglast.parser.ParseError as e:
        query = 'select '+query
    res = parse_sql_query(query)
    dbg_assert(len(res) == 1, 'unsupported query: {}'.format(res))
    # print(res)
    trvr = QueryTraverser(vars, substitutes)
    trvr(res)
    # print(res)
    # if res[0].stmt.extra.get('compile', False):
    #     print('Can compile: {}'.format(query))
    # else:
    #     print('Can not compile: {}'.format(query))
    # sort the modifier rule and apply the rules in order
    modifier = sorted(trvr.sql_modifier, key=lambda x: x[0])
    args = []
    pre = 0
    prep_statement = []
    var_map = {}
    for loc, var in modifier:
        if var not in vars and var not in substitutes:
            raise Exception('Variable {} is not defined'.format(var))
        prep_statement.append(query[pre:loc])
        if var not in var_map:
            prep_statement.append('col'+str(len(args)))
            var_map[var] = 'col'+str(len(args))
            if var in substitutes:
                args.append('const_vector_gen(static_cast<int>({}))'.format(substitutes[var]))
            else:
                args.append(var)
        else:
            prep_statement.append(var_map[var])
        pre = loc + len(var)
    prep_statement.append(query[pre:])
    args_len = len(args)
    from_stmt = ' from (values {})'.format(', '.join(['({})'.format(', '.join(['$'+str(i*vector_size + j+1) for i in range(args_len)])) for j in range(vector_size)]))
    prep_statement.append(from_stmt)
    # print(''.join(prep_statement))
    return ''.join(prep_statement), args