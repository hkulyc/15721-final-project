CREATE FUNCTION if_func(cid integer) RETURNS VARCHAR AS $$
DECLARE pd VARCHAR = 'function'; 
DECLARE pd2 VARCHAR;
BEGIN
    pd2 := pd;
    if cid < 3 then 
        cid := cid + 1;
    else
        cid := cid - 1;
    end if;
    RETURN concat(cid,pd2);
END; $$
LANGUAGE PLPGSQL;