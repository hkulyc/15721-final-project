import pglast

if __name__ == '__main__':
    sql = "\
CREATE FUNCTION retrieve_parents(cid integer) RETURNS text AS $$\
DECLARE pd text = 'function';    \
BEGIN\
    RETURN concat(cid,pd);\
END; $$\
LANGUAGE PLPGSQL"
    tree = pglast.parser.parse_plpgsql_json(sql)
    print(tree)