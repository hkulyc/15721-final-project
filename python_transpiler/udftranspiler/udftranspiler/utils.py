def dbg_assert(cond: bool, msg: str):
    if not cond:
        raise Exception(msg)
    
def is_assignment(query: str):
    "Check if a query is an assignment statement"
    return ':=' in query


def parse_assignment(query: str, vars: dict):
    "Parse an assignment query. May raise exception if wrong"
    v_list = query.split(':=')
    dbg_assert(len(v_list) == 2, 'bad assignment: {}'.format(query))
    leftv = v_list[0].strip()
    dbg_assert(leftv in vars, 'variable {} is not a declared local variable'.format(leftv))
    rightv = v_list[1].strip()
    return leftv, rightv

def is_number(s: str):
    "check if a string represents either float or int"
    return s.replace('.','',1).isdigit()

def is_const_or_var(expr: str, vars: dict):
    "check if an expression is a constant such as 2/'string', or variable"
    if (expr.startswith('"') and expr.endswith('"'))  or (expr.startswith("'") and expr.endswith("'")):
        # it is a string const
        return True
    elif (is_number(expr)):
        return True
    elif (expr in vars):
        return True
    else: return False
    

class Udf_Type:
    duckdb_to_cpp_type = {
        "BOOLEAN": "bool",
        "TINYINT": "int8_t",
        "SMALLINT": "int16_t",
        "INTEGER": "int32_t",
        "BIGINT": "int64_t",
        "FLOAT": "double",
        "DOUBLE": "double",
        "VARCHAR": "string_t",
        "CHAR": "string_t",
        "BLOB": "string_t",
        "UNKNOWN": "UNKNOWN"
    }

    @staticmethod
    def resolve_type(type_name: str, udf_str: str) -> tuple[str, str]:
        """
        Resolve a type name to a C++ type name and a type size.
        """
        if type_name.startswith("#"):
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

    def __init__(self, duckdb_type: str, udf_str: str):
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