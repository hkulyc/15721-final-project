# PL/PgSQL UDF Transpiler Features

For more information on Pl/PgSQL, see [PL/PgSQL documentation](https://www.postgresql.org/docs/current/plpgsql.html)

## Function

## Loop

Supports `LOOP`, `EXIT`, `CONTINUE`, `WHILE`, `FOR`, with limitations of:
- Does not support specifying label. For instance, `EXIT [ label ]` cannot be called to exit from the loop labeled with `label`. The same holds true for `CONTINUE` as well.
- Does not support `WHEN` for  `EXIT` and `CONTINUE`. One should use explicit conditional statement instead.