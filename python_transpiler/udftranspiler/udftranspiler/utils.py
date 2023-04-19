def is_assignment(query: str):
    "Check if a query is an assignment statement"
    return ':=' in query


def parse_assignment(query: str, vars):
    "Parse an assignment query. May raise exception if wrong"
    pass


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