from typing import Tuple
import re


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
        v_list = query.split('=')
    # print(v_list)
    dbg_assert(len(v_list) == 2, 'bad assignment: {}'.format(query))
    leftv = v_list[0].strip()
    dbg_assert(leftv in vars,
               'variable {} is not a declared local variable'.format(leftv))
    rightv = v_list[1].strip()
    return leftv, rightv


def is_number(s: str):
    "check if a string represents either float or int"
    return s.replace('.', '', 1).isdigit()


def is_var(expr: str, vars: dict):
    "check if an expression is a constant such as 2/'string', or variable"
    if (expr in vars):
        return True
    else:
        return False


def is_const(expr: str, vars: dict):
    "check if an expression is a constant such as 2/'string', or variable"
    if (expr.startswith('"') and expr.endswith('"')) or (expr.startswith("'") and expr.endswith("'")):
        # it is a string const
        return True
    elif (is_number(expr)):
        return True
    else:
        return False


def reformat_sql_string(sql_str: str):
    if (sql_str.startswith("'") and sql_str.endswith("'")):
        return '"'+sql_str[1:-1].replace('"', '\"')+'"'
    else:
        return sql_str


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
    alias_to_duckdb_type = {
        "BIGINT": "BIGINT",
        "INT8": "BIGINT",
        "LONG": "BIGINT",
        "BIT": "BIT",
        "BITSTRING": "BIT",
        "BOOLEAN": "BOOLEAN",
        "BOOL": "BOOLEAN",
        "LOGICAL": "BOOLEAN",
        "BLOB": "BLOB",
        "BYTEA": "BLOB",
        "BINARY": "BLOB",
        "VARBINARY": "BLOB",
        "DATE": "DATE",
        "DOUBLE": "DOUBLE",
        "FLOAT8": "DOUBLE",
        "NUMERIC": "DOUBLE",
        "DECIMAL": "DOUBLE",  # this is decimal without fixed precision
        # DECIMAL(s, p) needs to be checked for manually
        "HUGEINT": "HUGEINT",
        "INTEGER": "INTEGER",
        "INT": "INTEGER",
        "INT4": "INTEGER",
        "SIGNED": "INTEGER",
        "INTERVAL": "INTERVAL",
        "REAL": "REAL",
        "FLOAT4": "REAL",
        "FLOAT": "REAL",
        "SMALLINT": "SMALLINT",
        "INT2": "SMALLINT",
        "SHORT": "SMALLINT",
        "TIME": "TIME",
        "TIMESTAMP": "TIMESTAMP",
        "DATETIME": "TIMESTAMP",
        "TINYINT": "TINYINT",
        "INT1": "TINYINT",
        "UBIGINT": "UBIGINT",
        "UINTEGER": "UINTEGER",
        "USMALLINT": "USMALLINT",
        "UTINYINT": "UTINYINT",
        "UUID": "UUID",
        "VARCHAR": "VARCHAR",
        "CHAR": "VARCHAR",
        "BPCHAR": "VARCHAR",
        "TEXT": "VARCHAR",
        "STRING": "VARCHAR",
    }

    @staticmethod
    def resolve_type(type_name: str, udf_str: str) -> str:
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
            if type_name == "DECIMAL":
                # check for DECIMAL(s, p)
                res = re.findall(r"^(\(\d+,\d+\))",
                                 udf_str[type_end:], flags=re.IGNORECASE)
                if len(res) > 0:
                    return type_name + res[0]

        type_name = type_name.upper()
        type_name = type_name.replace(" ", "")

        if type_name.startswith("DECIMAL"):
            res = re.findall(r"^DECIMAL(\(\d+,\d+\))",
                             type_name, flags=re.IGNORECASE)
            if len(res) > 0:
                return type_name

        if type_name in Udf_Type.alias_to_duckdb_type:
            return Udf_Type.alias_to_duckdb_type[type_name]
        else:
            raise Exception("Unknown type: ", type_name)

    def __init__(self, duckdb_type: str, udf_str: str = ""):
        duckdb_type = self.resolve_type(duckdb_type, udf_str)
        self.duckdb_type = duckdb_type

    def __str__(self):
        return f"{self.duckdb_type}"

    def __repr__(self):
        return str(self)

    def is_unknown(self):
        return self.duckdb_type == "UNKNOWN"

    def get_cpp_sqltype(self):
        return f"duckdb::LogicalType::{self.duckdb_type}"


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

    def __get_mask_pointer(self, name: str):
        if name == None:
            return 'NULL'
        else:
            return '&'+name

    def get_mask_pointers(self):
        "get a list of names of pointers to the four masks"
        return [self.__get_mask_pointer(self.get_active()),
                self.__get_mask_pointer(self.get_returns()),
                self.__get_mask_pointer(self.get_loop_active()),
                self.__get_mask_pointer(self.get_continues())]

    def get_mask_names(self):
        "get a list of names of the four masks"
        return [self.get_active(), self.get_returns(), self.get_loop_active() or 'NULL', self.get_continues() or 'NULL']
