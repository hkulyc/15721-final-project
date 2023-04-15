CREATE FUNCTION if_func(cid integer) RETURNS text AS $$
DECLARE pd text = 'function'; 
BEGIN
    if cid < 3 then 
        cid := cid + 1;
    else
        cid := cid - 1;
    end if;
    RETURN concat(cid,pd);
END; $$
LANGUAGE PLPGSQL;