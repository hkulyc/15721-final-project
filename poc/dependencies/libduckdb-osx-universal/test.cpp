#include "duckdb.hpp"
// #include "../libduckdb-osx-universal/duckdb.h"

DuckDB db(nullptr);
Connection con(db);
auto result = con.Query("SELECT 42");
result->Print();