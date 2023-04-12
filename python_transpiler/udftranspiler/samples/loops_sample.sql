CREATE FUNCTION if_func(cid integer) RETURNS text AS $$
DECLARE pd text = 'function'; 
declare i int = 0;
BEGIN
    LOOP
        IF i > 9 THEN
            EXIT;  -- exit loop
        else
            cid = cid + 1
        END IF;
    END LOOP;
    RETURN concat(cid,pd);
END; $$
LANGUAGE PLPGSQL;


-- LOOP
--     -- some computations
--     EXIT WHEN count > 0;
-- END LOOP;

CREATE FUNCTION if_func(cid integer) RETURNS text AS $$
DECLARE pd text = 'function'; 
-- declare i int = 0;
BEGIN
    FOR i IN 1..10 LOOP
        cid = cid + 1
    END LOOP;
    RETURN concat(cid,pd);
END; $$
LANGUAGE PLPGSQL;

CREATE FUNCTION if_func(cid integer) RETURNS text AS $$
DECLARE pd text = 'function'; 
declare i int = 0;
BEGIN
    while i < 10 LOOP
        cid = cid + 1
    END LOOP;
    RETURN concat(cid,pd);
END; $$
LANGUAGE PLPGSQL;