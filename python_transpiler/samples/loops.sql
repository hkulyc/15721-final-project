CREATE FUNCTION simple_loop(x integer) RETURNS INTEGER AS $$
declare i integer = 0;
BEGIN
    LOOP
        IF i > 9 THEN
            EXIT;  -- exit loop
        else
            i = i + 1;
        
        END IF;
    END LOOP;
    RETURN x;
END; $$
LANGUAGE PLPGSQL;


-- LOOP
--     -- some computations
--     EXIT WHEN count > 0;
-- END LOOP;

CREATE FUNCTION for_loop(x integer) RETURNS INTEGER AS $$
declare i integer = 0;
BEGIN
    FOR j IN 1..10 LOOP
        i = i + 1;
    END LOOP;
    RETURN x;
END; $$
LANGUAGE PLPGSQL;

CREATE FUNCTION for_loop_2(x integer) RETURNS INTEGER AS $$
declare i integer = 0;
BEGIN
    FOR j IN 1..10 BY 2 LOOP
        i = i + 1;
    END LOOP;
    RETURN x;
END; $$
LANGUAGE PLPGSQL;

CREATE FUNCTION while_loop(x integer) RETURNS INTEGER AS $$
declare i integer = 0;
BEGIN
    while i < 10 LOOP
        i = i + 1;
    END LOOP;
    RETURN x;
END; $$
LANGUAGE PLPGSQL;