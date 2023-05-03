-- Query 1
SELECT L_RETURNFLAG, L_LINESTATUS, SUM(L_QUANTITY) AS SUM_QTY,
SUM(L_EXTENDEDPRICE) AS SUM_BASE_PRICE,
SUM(discount_price(L_EXTENDEDPRICE, L_DISCOUNT))
AS SUM_DISC_PRICE,
SUM(discount_taxprice(L_EXTENDEDPRICE, L_DISCOUNT, L_TAX))
AS SUM_CHARGE,
AVG(L_QUANTITY) AS AVG_QTY,
AVG(L_EXTENDEDPRICE) AS AVG_PRICE, AVG(L_DISCOUNT) AS AVG_DISC,
COUNT(*) AS COUNT_ORDER
FROM LINEITEM
WHERE isShippedBefore(L_SHIPDATE, -90, '1998-12-01') = 1
GROUP BY L_RETURNFLAG, L_LINESTATUS
ORDER BY L_RETURNFLAG,L_LINESTATUS

-- Query 3
SELECT TOP 10 L_ORDERKEY,
SUM(discount_price(L_EXTENDEDPRICE, L_DISCOUNT)) AS REVENUE,
O_ORDERDATE, O_SHIPPRIORITY
FROM CUSTOMER, ORDERS, LINEITEM
WHERE C_CUSTKEY = O_CUSTKEY AND L_ORDERKEY = O_ORDERKEY
AND q3conditions(C_MKTSEGMENT, O_ORDERDATE, L_SHIPDATE) = 1
GROUP BY L_ORDERKEY, O_ORDERDATE, O_SHIPPRIORITY
ORDER BY REVENUE DESC, O_ORDERDATE

-- Query 5
SELECT N_NAME,
SUM(discount_price(L_EXTENDEDPRICE, L_DISCOUNT)) AS REVENUE
FROM CUSTOMER, ORDERS, LINEITEM, SUPPLIER, NATION, REGION
WHERE C_CUSTKEY = O_CUSTKEY AND L_ORDERKEY = O_ORDERKEY
AND L_SUPPKEY = S_SUPPKEY AND C_NATIONKEY = S_NATIONKEY
AND S_NATIONKEY = N_NATIONKEY AND N_REGIONKEY = R_REGIONKEY
AND q5Conditions(R_NAME, O_ORDERDATE) = 1
GROUP BY N_NAME
ORDER BY REVENUE DESC

-- Query 6
SELECT SUM(L_EXTENDEDPRICE*L_DISCOUNT) AS REVENUE
FROM LINEITEM
WHERE q6conditions(L_SHIPDATE, L_DISCOUNT, L_QUANTITY) = 1;

-- Query 7
SELECT SUPP_NATION, CUST_NATION, L_YEAR, SUM(VOLUME) AS REVENUE
FROM ( SELECT N1.N_NAME AS SUPP_NATION, N2.N_NAME AS CUST_NATION,
datepart(yy, L_SHIPDATE) AS L_YEAR,
L_EXTENDEDPRICE*(1-L_DISCOUNT) AS VOLUME
FROM SUPPLIER, LINEITEM, ORDERS, CUSTOMER, NATION N1, NATION N2
WHERE S_SUPPKEY = L_SUPPKEY AND O_ORDERKEY = L_ORDERKEY
AND C_CUSTKEY = O_CUSTKEY
AND S_NATIONKEY = N1.N_NATIONKEY
AND C_NATIONKEY = N2.N_NATIONKEY
AND q7conditions(N1.N_NAME, N2.N_NAME, L_SHIPDATE) = 1 )
AS SHIPPING
GROUP BY SUPP_NATION, CUST_NATION, L_YEAR
ORDER BY SUPP_NATION, CUST_NATION, L_YEAR

-- Query 9
SELECT NATION, O_YEAR, SUM(AMOUNT) AS SUM_PROFIT
FROM (SELECT N_NAME AS NATION,
datepart(yy, O_ORDERDATE) AS O_YEAR,
profit_amount(L_EXTENDEDPRICE, L_DISCOUNT, PS_SUPPLYCOST, L_QUANTITY) AS AMOUNT
FROM PART, SUPPLIER, LINEITEM, PARTSUPP, ORDERS, NATION
WHERE S_SUPPKEY = L_SUPPKEY AND PS_SUPPKEY= L_SUPPKEY
AND PS_PARTKEY = L_PARTKEY AND P_PARTKEY= L_PARTKEY
AND O_ORDERKEY = L_ORDERKEY AND S_NATIONKEY = N_NATIONKEY AND
P_NAME LIKE '%%green%%') AS PROFIT
GROUP BY NATION, O_YEAR
ORDER BY NATION, O_YEAR DESC

-- Query 10
SELECT TOP 20 C_CUSTKEY, C_NAME,
SUM(discount_price(L_EXTENDEDPRICE, L_DISCOUNT)) AS REVENUE,
C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT
FROM CUSTOMER, ORDERS, LINEITEM, NATION
WHERE C_CUSTKEY = O_CUSTKEY AND L_ORDERKEY = O_ORDERKEY AND
q10conditions(O_ORDERDATE, L_RETURNFLAG) = 1
AND C_NATIONKEY = N_NATIONKEY
GROUP BY C_CUSTKEY, C_NAME, C_ACCTBAL, C_PHONE,
N_NAME, C_ADDRESS, C_COMMENT
ORDER BY REVENUE DESC

-- Query 11
SELECT PS_PARTKEY, SUM(PS_SUPPLYCOST*PS_AVAILQTY) AS VALUE
FROM PARTSUPP, SUPPLIER, NATION
WHERE PS_SUPPKEY = S_SUPPKEY AND S_NATIONKEY = N_NATIONKEY
AND N_NAME = 'GERMANY'
GROUP BY PS_PARTKEY
HAVING SUM(PS_SUPPLYCOST*PS_AVAILQTY) >  (SELECT SUM(PS_SUPPLYCOST*PS_AVAILQTY) * 0.0001000000
FROM PARTSUPP, SUPPLIER, NATION
WHERE PS_SUPPKEY = S_SUPPKEY
AND S_NATIONKEY = N_NATIONKEY AND N_NAME = 'GERMANY')
ORDER BY VALUE DESC

-- Query 12
SELECT L_SHIPMODE,
SUM(line_count(O_ORDERPRIORITY, 'high')) AS HIGH_LINE_COUNT,
SUM(line_count(O_ORDERPRIORITY, 'low')) AS LOW_LINE_COUNT
FROM ORDERS, LINEITEM
WHERE O_ORDERKEY = L_ORDERKEY AND
q12conditions(L_SHIPMODE, L_COMMITDATE,
L_RECEIPTDATE, L_SHIPDATE) = 1
GROUP BY L_SHIPMODE
ORDER BY L_SHIPMODE

-- Query 14
SELECT 100.00 *
SUM(promo_disc(P_TYPE, L_EXTENDEDPRICE, L_DISCOUNT))
/ SUM(discount_price(L_EXTENDEDPRICE,L_DISCOUNT))
AS PROMO_REVENUE
FROM LINEITEM, PART
WHERE L_PARTKEY = P_PARTKEY AND L_SHIPDATE >= '1995-09-01'
AND L_SHIPDATE < (DATE '1995-09-01') + (INTERVAL '1 MONTH') -- dateadd(mm, 1, '1995-09-01')

-- Query 19
SELECT SUM(discount_price(L_EXTENDEDPRICE, L_DISCOUNT))
AS REVENUE
FROM LINEITEM join PART on L_PARTKEY = P_PARTKEY
WHERE q19conditions(P_CONTAINER, L_QUANTITY, P_SIZE,
L_SHIPMODE, L_SHIPINSTRUCT, P_BRAND ) = 1;

-- Query 22
SELECT CNTRYCODE,
COUNT(*) AS NUMCUST, SUM(C_ACCTBAL) AS TOTACCTBAL
FROM (SELECT SUBSTRING(C_PHONE,1,2) AS CNTRYCODE, C_ACCTBAL
FROM CUSTOMER WHERE SUBSTRING(C_PHONE,1,2)
IN ('13', '31', '23', '29', '30', '18', '17')
AND C_ACCTBAL > (SELECT AVG(C_ACCTBAL) FROM CUSTOMER
WHERE C_ACCTBAL > 0.00
AND SUBSTRING(C_PHONE,1,2)
IN ('13', '31', '23', '29', '30', '18', '17'))
AND NOT EXISTS ( SELECT * FROM ORDERS
WHERE O_CUSTKEY = C_CUSTKEY)) AS CUSTSALE
GROUP BY CNTRYCODE
ORDER BY CNTRYCODE
