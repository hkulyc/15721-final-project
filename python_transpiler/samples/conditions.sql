CREATE FUNCTION if_func(cid integer) RETURNS VARCHAR AS $$
DECLARE pd VARCHAR = 'function'; 
BEGIN
    if cid < 3 then 
        cid := cid + 1;
    else
        cid := cid - 1;
    end if;
    RETURN concat(cid,pd);
END; $$
LANGUAGE PLPGSQL;