# 15721 Final Project - PL/PgSQL UDF Transpiler

## File Structure

- ### poc

  CMake project used to proof the significance of the work.

- ### libpg-query

  PL/PgSQL Parser.

  #### Usage:

  ```
  cd libpg-query
  make build
  cc -I. -I./vendor -o main main.c  libpg_query.a -pthread
  ./main
  ```

  #### Output:

  ```
  [
  {"PLpgSQL_function":{"datums":[{"PLpgSQL_var":{"refname":"cmkt","datatype":{"PLpgSQL_type":{"typname":"#36#"}}}},{"PLpgSQL_var":{"refname":"odate","datatype":{"PLpgSQL_type":{"typname":"#47#"}}}},{"PLpgSQL_var":{"refname":"shipdate","datatype":{"PLpgSQL_type":{"typname":"#62#"}}}},{"PLpgSQL_var":{"refname":"found","datatype":{"PLpgSQL_type":{"typname":"UNKNOWN"}}}},{"PLpgSQL_var":{"refname":"thedate","lineno":1,"datatype":{"PLpgSQL_type":{"typname":"varchar(30) "}},"default_val":{"PLpgSQL_expr":{"query":"'1995-03-15'"}}}}],"action":{"PLpgSQL_stmt_block":{"lineno":1,"body":[{"PLpgSQL_stmt_if":{"lineno":1,"cond":{"PLpgSQL_expr":{"query":"odate \u003c 'asd'"}},"then_body":[{"PLpgSQL_stmt_return":{"lineno":1,"expr":{"PLpgSQL_expr":{"query":"2"}}}}]}},{"PLpgSQL_stmt_return":{"lineno":1,"expr":{"PLpgSQL_expr":{"query":"thedata"}}}}]}}}}
  ]
  ```

  *Note that "#{%d}#" means that the datatype name is the substring starting from this location, and ending by `,` or `)`.*

- ### python_transpiler
  Usage see its own README file.