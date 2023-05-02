from typing import Tuple


def dbg_assert(cond: bool, msg: str):
    if not cond:
        raise Exception(msg)

# def is_assignment(query: str):
#     "Check if a query is an assignment statement"
#     return ':=' in query


def parse_assignment(query: str, vars: dict):
    "Parse an assignment query. May raise exception if wrong"
    if ":=" in query:
        v_list = query.split(':=')
    else:
        v_list = query.split('=', 1)

    dbg_assert(len(v_list) == 2, 'bad assignment: {}'.format(query))
    leftv = v_list[0].strip()
    dbg_assert(leftv in vars,
               'variable {} is not a declared local variable'.format(leftv))
    rightv = v_list[1].strip()
    return leftv, rightv


def is_number(s: str):
    "check if a string represents either float or int"
    return s.replace('.', '', 1).isdigit()


def is_const_or_var(expr: str, vars: dict):
    "check if an expression is a constant such as 2/'string', or variable"
    if (expr.startswith('"') and expr.endswith('"')) or (expr.startswith("'") and expr.endswith("'")):
        # it is a string const
        return True
    elif (is_number(expr)):
        return True
    elif (expr in vars):
        return True
    else:
        return False


def function_arg_decl(args, vars):
    "return a string that declares the function arguments"
    arg_list = []
    for arg in args:
        arg_list.append(vars[arg][0].cpp_type + ' ' + arg)
    return ', '.join(arg_list)


def substitute_variables(query: str, substitutes: dict) -> str:
    tokens = query.split()
    new_tokens = []
    for token in tokens:
        if token in substitutes:
            new_tokens.append(substitutes[token])
        else:
            new_tokens.append(token)
    return ' '.join(new_tokens)


def is_loop_tempvar(var: str) -> bool:
    return 'tempvar' in var


def add_identition(code: str, iden: str = '    '):
    "Add identition to each line to make the generated code look prettier"
    return iden+code.replace('\n', '\n'+iden)


class Udf_Type:
    duckdb_to_cpp_type = {
        "BOOLEAN": "bool",
        "TINYINT": "int8_t",
        "SMALLINT": "int16_t",
        "INTEGER": "int32_t",
        "INT": "int32_t",  # alias of INTEGER
        "BIGINT": "int64_t",
        "FLOAT": "double",
        "DOUBLE": "double",
        "VARCHAR": "string_t",
        "CHAR": "string_t",
        "BLOB": "string_t",
        "UNKNOWN": "UNKNOWN",
        "DECIMAL": "double",  # not actually supported

    }

    @staticmethod
    def resolve_type(type_name: str, udf_str: str) -> Tuple[str, str]:
        """
        Resolve a type name to a C++ type name and a type size.
        """
        if type_name.startswith("#"):
            if len(udf_str) == 0:
                raise Exception("udf_str is empty")
            type_start = int(type_name.replace("#", ""))
            type_end = type_start
            while udf_str[type_end].isalpha():
                type_end += 1

            type_name = udf_str[type_start:type_end]
        type_name = type_name.upper()
        type_name = type_name.replace(" ", "")
        if type_name in Udf_Type.duckdb_to_cpp_type:
            return type_name, Udf_Type.duckdb_to_cpp_type[type_name]
        else:
            raise Exception("Unknown type: ", type_name)

    def __init__(self, duckdb_type: str, udf_str: str = ""):
        duckdb_type, cpp_type = self.resolve_type(duckdb_type, udf_str)
        self.duckdb_type = duckdb_type
        self.cpp_type = cpp_type

    def __str__(self):
        return f"{self.duckdb_type}|{self.cpp_type}"

    def __repr__(self):
        return str(self)

    def is_unknown(self):
        return self.duckdb_type == "UNKNOWN"

    def get_cpp_sqltype(self):
        return f"SQLType::{self.duckdb_type}"


class ActiveLanes:
    def __init__(self):
        self.counter = 0
        self.active = []
        self.loop_active = []
        self.continues = []
        self.returns = []

    def get_active(self):
        assert len(self.active) >= 1
        return self.active[0]

    def get_loop_active(self):
        if len(self.loop_active) == 0:
            return None
        return self.loop_active[0]

    def get_continues(self):
        if len(self.continues) == 0:
            return None
        return self.continues[0]

    def get_returns(self):
        assert len(self.returns) >= 1
        return self.returns[0]

    def new_active(self):
        self.counter += 1
        name = "active"+str(self.counter)
        self.active.insert(0, name)
        return name

    def new_loop_active(self):
        self.counter += 1
        name = "loop_active"+str(self.counter)
        self.loop_active.insert(0, name)
        return name

    def new_continues(self):
        self.counter += 1
        name = "continues"+str(self.counter)
        self.continues.insert(0, name)
        return name

    def new_returns(self):
        self.counter += 1
        name = "returns"+str(self.counter)
        self.returns.insert(0, name)
        return name

    def pop_active(self):
        return self.active.pop(0)

    def pop_loop_active(self):
        return self.loop_active.pop(0)
    
    def pop_continues(self):
        return self.continues.pop(0)

    def pop_returns(self):
        return self.returns.pop(0)
