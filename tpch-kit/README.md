# How to use TPC-H

## Basic Overview
- `dbgen` folder contains the code for generating TPC-H data and queries
- Data is already generated. They're placed in `tpch-kit/dbgen-output` folder. Ignore 
- Queries are already generated. They're placed in the `tpch-kit/queries` folder. 
  - `tpch-q<number>.sql` is the sql file for query number `<number>`. 
  - `tpch-stream.sql` is a single sql file containing all the queries.

## Run TPC-H benchmark on Postgres database
- You do not need to use `dbgen`'s tools because I have already created the data and queries.
- You will need postgres on your machine and be able to call it via `psql`
- See right below for steps to run TPC-H.

`cd` into `tpch-kit` directory

Create postgres database named `tpch`, and load it with tpch schema
```
createdb tpch
psql tpch -f dbgen/dss.ddl
```

To load TPC-H data into the `tpch` postgres database you just created,
```
cd dbgen-output
sh ../populate.sh
cd ..
```

To run the `query#`th TPC-H query (this is where you're actually running the TPC-H benchmark),
```
psql tpch < queries/tpch-q<query#>.sql
```


## Run TPC-H benchmark on DuckDB database
- Not implemented yet, but should be similar as Postgres


## TPC-H queries using UDF
- Query 19 (`tpch-kit/queries/tpch-q19.sql`) has been translated into UDF format in the file `tpch-kit/queries/udf-q19.sql`. The same exact file is included in `python_transpiler/samples`.


## Useful references:

- https://ankane.org/tpc-h
- https://github.com/gregrahn/tpch-kit