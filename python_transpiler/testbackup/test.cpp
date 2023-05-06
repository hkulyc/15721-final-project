#include "duckdb.hpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include "udf.hpp"

duckdb::DuckDB main_db(nullptr);

double mean(std::vector<double> v){
    double sum = 0;
    for(int i:v){
        sum += i;
    }
    return sum/v.size();
}

void prepare_env(duckdb::Connection *con){
    con->Query("CREATE TABLE NATION  ( N_NATIONKEY  INTEGER NOT NULL, \
                        N_NAME       CHAR(25) NOT NULL, \
                        N_REGIONKEY  INTEGER NOT NULL, \
                        N_COMMENT    VARCHAR(152));\
\
CREATE TABLE REGION  ( R_REGIONKEY  INTEGER NOT NULL, \
                        R_NAME       CHAR(25) NOT NULL, \
                        R_COMMENT    VARCHAR(152)); \
\
CREATE TABLE PART  ( P_PARTKEY     INTEGER NOT NULL, \
                      P_NAME        VARCHAR(55) NOT NULL,  \
                      P_MFGR        CHAR(25) NOT NULL, \
                      P_BRAND       CHAR(10) NOT NULL, \
                      P_TYPE        VARCHAR(25) NOT NULL, \
                      P_SIZE        INTEGER NOT NULL, \
                      P_CONTAINER   CHAR(10) NOT NULL, \
                      P_RETAILPRICE DECIMAL(15,2) NOT NULL, \
                      P_COMMENT     VARCHAR(23) NOT NULL ); \
\
CREATE TABLE SUPPLIER ( S_SUPPKEY     INTEGER NOT NULL, \
                         S_NAME        CHAR(25) NOT NULL, \
                         S_ADDRESS     VARCHAR(40) NOT NULL, \
                         S_NATIONKEY   INTEGER NOT NULL, \
                         S_PHONE       CHAR(15) NOT NULL, \
                         S_ACCTBAL     DECIMAL(15,2) NOT NULL, \
                         S_COMMENT     VARCHAR(101) NOT NULL); \
\
CREATE TABLE PARTSUPP ( PS_PARTKEY     INTEGER NOT NULL, \
                         PS_SUPPKEY     INTEGER NOT NULL, \
                         PS_AVAILQTY    INTEGER NOT NULL, \
                         PS_SUPPLYCOST  DECIMAL(15,2)  NOT NULL, \
                         PS_COMMENT     VARCHAR(199) NOT NULL ); \
\
CREATE TABLE CUSTOMER ( C_CUSTKEY     INTEGER NOT NULL, \
                         C_NAME        VARCHAR(25) NOT NULL, \
                         C_ADDRESS     VARCHAR(40) NOT NULL, \
                         C_NATIONKEY   INTEGER NOT NULL, \
                         C_PHONE       CHAR(15) NOT NULL, \
                         C_ACCTBAL     DECIMAL(15,2)   NOT NULL, \
                         C_MKTSEGMENT  CHAR(10) NOT NULL, \
                         C_COMMENT     VARCHAR(117) NOT NULL); \
\
CREATE TABLE ORDERS  ( O_ORDERKEY       INTEGER NOT NULL, \
                       O_CUSTKEY        INTEGER NOT NULL, \
                       O_ORDERSTATUS    CHAR(1) NOT NULL, \
                       O_TOTALPRICE     DECIMAL(15,2) NOT NULL, \
                       O_ORDERDATE      DATE NOT NULL, \
                       O_ORDERPRIORITY  CHAR(15) NOT NULL,  \
                       O_CLERK          CHAR(15) NOT NULL, \
                       O_SHIPPRIORITY   INTEGER NOT NULL, \
                       O_COMMENT        VARCHAR(79) NOT NULL); \
\
CREATE TABLE LINEITEM ( L_ORDERKEY    INTEGER NOT NULL,\
                         L_PARTKEY     INTEGER NOT NULL,\
                         L_SUPPKEY     INTEGER NOT NULL,\
                         L_LINENUMBER  INTEGER NOT NULL,\
                         L_QUANTITY    INTEGER NOT NULL,\
                         L_EXTENDEDPRICE  DECIMAL(15,2) NOT NULL,\
                         L_DISCOUNT    DECIMAL(15,2) NOT NULL,\
                         L_TAX         DECIMAL(15,2) NOT NULL,\
                         L_RETURNFLAG  CHAR(1) NOT NULL,\
                         L_LINESTATUS  CHAR(1) NOT NULL,\
                         L_SHIPDATE    DATE NOT NULL,\
                         L_COMMITDATE  DATE NOT NULL,\
                         L_RECEIPTDATE DATE NOT NULL,\
                         L_SHIPINSTRUCT CHAR(25) NOT NULL,\
                         L_SHIPMODE     CHAR(10) NOT NULL,\
                         L_COMMENT      VARCHAR(44) NOT NULL);");
    con->Query("INSERT INTO NATION (SELECT * FROM read_csv_auto('dataset/nation.tbl'))");
    con->Query("INSERT INTO REGION (SELECT * FROM read_csv_auto('dataset/region.tbl'))");
    con->Query("INSERT INTO PART (SELECT * FROM read_csv_auto('dataset/part.tbl'))");
    con->Query("INSERT INTO SUPPLIER (SELECT * FROM read_csv_auto('dataset/supplier.tbl'))");
    con->Query("INSERT INTO PARTSUPP (SELECT * FROM read_csv_auto('dataset/partsupp.tbl'))");
    con->Query("INSERT INTO CUSTOMER (SELECT * FROM read_csv_auto('dataset/customer.tbl'))");
    con->Query("INSERT INTO ORDERS (SELECT * FROM read_csv_auto('dataset/orders.tbl'))");
    con->Query("INSERT INTO LINEITEM (SELECT * FROM read_csv_auto('dataset/lineitem.tbl'))");
    con->CreateVectorizedFunction("discount_price", {duckdb::LogicalType::DECIMAL(12,2), duckdb::LogicalType::DECIMAL(12,2)}, duckdb::LogicalType::DECIMAL(18,4), &discount_price);
con->CreateVectorizedFunction("discount_taxprice", {duckdb::LogicalType::DECIMAL(12,2), duckdb::LogicalType::DECIMAL(12,2), duckdb::LogicalType::DECIMAL(12,2)}, duckdb::LogicalType::DECIMAL(18,6), &discount_taxprice);
con->CreateVectorizedFunction("profit_amount", {duckdb::LogicalType::DECIMAL(12,2), duckdb::LogicalType::DECIMAL(12,2), duckdb::LogicalType::DECIMAL(12,2), duckdb::LogicalType::INTEGER}, duckdb::LogicalType::DECIMAL(18,4), &profit_amount);
con->CreateVectorizedFunction("isShippedBefore", {duckdb::LogicalType::DATE, duckdb::LogicalType::INTEGER, duckdb::LogicalType::VARCHAR}, duckdb::LogicalType::INTEGER, &isShippedBefore);
con->CreateVectorizedFunction("checkDate", {duckdb::LogicalType::VARCHAR, duckdb::LogicalType::DATE, duckdb::LogicalType::DATE}, duckdb::LogicalType::INTEGER, &checkDate);
con->CreateVectorizedFunction("q3conditions", {duckdb::LogicalType::VARCHAR, duckdb::LogicalType::DATE, duckdb::LogicalType::DATE}, duckdb::LogicalType::INTEGER, &q3conditions);
con->CreateVectorizedFunction("q5Conditions", {duckdb::LogicalType::VARCHAR, duckdb::LogicalType::DATE}, duckdb::LogicalType::INTEGER, &q5Conditions);
con->CreateVectorizedFunction("q6conditions", {duckdb::LogicalType::DATE, duckdb::LogicalType::DECIMAL(12,2), duckdb::LogicalType::INTEGER}, duckdb::LogicalType::INTEGER, &q6conditions);
con->CreateVectorizedFunction("q7conditions", {duckdb::LogicalType::VARCHAR, duckdb::LogicalType::VARCHAR, duckdb::LogicalType::DATE}, duckdb::LogicalType::INTEGER, &q7conditions);
con->CreateVectorizedFunction("q10conditions", {duckdb::LogicalType::DATE, duckdb::LogicalType::VARCHAR}, duckdb::LogicalType::INTEGER, &q10conditions);
con->CreateVectorizedFunction("line_count", {duckdb::LogicalType::VARCHAR, duckdb::LogicalType::VARCHAR}, duckdb::LogicalType::INTEGER, &line_count);
con->CreateVectorizedFunction("q12conditions", {duckdb::LogicalType::VARCHAR, duckdb::LogicalType::DATE, duckdb::LogicalType::DATE, duckdb::LogicalType::DATE}, duckdb::LogicalType::INTEGER, &q12conditions);
con->CreateVectorizedFunction("promo_disc", {duckdb::LogicalType::VARCHAR, duckdb::LogicalType::DECIMAL(12,2), duckdb::LogicalType::DECIMAL(12,2)}, duckdb::LogicalType::DECIMAL(12,2), &promo_disc);
con->CreateVectorizedFunction("q19conditions", {duckdb::LogicalType::VARCHAR, duckdb::LogicalType::DECIMAL(15,2), duckdb::LogicalType::INTEGER, duckdb::LogicalType::VARCHAR, duckdb::LogicalType::VARCHAR, duckdb::LogicalType::VARCHAR}, duckdb::LogicalType::INTEGER, &q19conditions);
}

static const int EPOCH_NUM = 1;
int main(int argc, char const *argv[])
{
    duckdb::Connection con(main_db);
    std::cout << "Preparing environment..." << std::endl;
    prepare_env(&con);
    std::cout << "Running Query..." << std::endl;
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;
    std::vector<double> time;
    std::unique_ptr<duckdb::MaterializedQueryResult> result1;
    std::unique_ptr<duckdb::MaterializedQueryResult> result2;
    std::string query;
    //========== Query 1 ===========
    query = "Query 1";
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result1 = con.Query("SELECT L_RETURNFLAG, L_LINESTATUS, SUM(L_QUANTITY) AS SUM_QTY, SUM(L_EXTENDEDPRICE) AS SUM_BASE_PRICE, SUM(discount_price(L_EXTENDEDPRICE, L_DISCOUNT)) AS SUM_DISC_PRICE, SUM(discount_taxprice(L_EXTENDEDPRICE, L_DISCOUNT, L_TAX)) AS SUM_CHARGE, AVG(L_QUANTITY) AS AVG_QTY, AVG(L_EXTENDEDPRICE) AS AVG_PRICE, AVG(L_DISCOUNT) AS AVG_DISC, COUNT(*) AS COUNT_ORDER FROM LINEITEM WHERE isShippedBefore(L_SHIPDATE, -90, '1998-12-01') = 1 GROUP BY L_RETURNFLAG, L_LINESTATUS ORDER BY L_RETURNFLAG,L_LINESTATUS");
        end = std::chrono::steady_clock::now();
        if(result1->HasError())
            std::cout << result1->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    std::cout << query << " UDF time: " << mean(time) << " ms" << std::endl;
    time.clear();
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result2 = con.Query("select 	l_returnflag, 	l_linestatus, 	sum(l_quantity) as sum_qty, 	sum(l_extendedprice) as sum_base_price, 	sum(l_extendedprice * (1 - l_discount)) as sum_disc_price, 	sum(l_extendedprice * (1 - l_discount) * (1 + l_tax)) as sum_charge, 	avg(l_quantity) as avg_qty, 	avg(l_extendedprice) as avg_price, 	avg(l_discount) as avg_disc, 	count(*) as count_order from 	lineitem where 	l_shipdate <= date '1998-12-01' - interval '90' day group by 	l_returnflag, 	l_linestatus order by 	l_returnflag, 	l_linestatus;  ");
        end = std::chrono::steady_clock::now();
        if(result2->HasError())
            std::cout << result2->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        
    }
    std::cout << query << " time: " << mean(time) << " ms" << std::endl;
    std::cout << result1->ToString() << "\n--------------------------------------------------------\n" << result2->ToString() << std::endl;
    if (result1->Equals(*result2)) {
        std::cout << query << " Matches!" << std::endl;
    } else {
        std::cout << query << " do NOT match!" << std::endl;
    }
    //========== Query 3 ===========
    query = "Query 3";
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result1 = con.Query("SELECT L_ORDERKEY, SUM(discount_price(L_EXTENDEDPRICE, L_DISCOUNT)) AS REVENUE, O_ORDERDATE, O_SHIPPRIORITY FROM CUSTOMER, ORDERS, LINEITEM WHERE C_CUSTKEY = O_CUSTKEY AND L_ORDERKEY = O_ORDERKEY AND q3conditions(C_MKTSEGMENT, O_ORDERDATE, L_SHIPDATE) = 1 GROUP BY L_ORDERKEY, O_ORDERDATE, O_SHIPPRIORITY ORDER BY REVENUE DESC, O_ORDERDATE  LIMIT 10");
        end = std::chrono::steady_clock::now();
        if(result1->HasError())
            std::cout << result1->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    std::cout << query << " UDF time: " << mean(time) << " ms" << std::endl;
    time.clear();
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result2 = con.Query("select 	l_orderkey, 	sum(l_extendedprice * (1 - l_discount)) as revenue, 	o_orderdate, 	o_shippriority from 	customer, 	orders, 	lineitem where 	c_mktsegment = 'BUILDING' 	and c_custkey = o_custkey 	and l_orderkey = o_orderkey 	and o_orderdate < date '1995-03-15' 	and l_shipdate > date '1995-03-15' group by 	l_orderkey, 	o_orderdate, 	o_shippriority order by 	revenue desc, 	o_orderdate limit 10; ");
        end = std::chrono::steady_clock::now();
        if(result2->HasError())
            std::cout << result2->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        
    }
    std::cout << query << " time: " << mean(time) << " ms" << std::endl;
    std::cout << result1->ToString() << "\n--------------------------------------------------------\n" << result2->ToString() << std::endl;
    if (result1->Equals(*result2)) {
        std::cout << query << " Matches!" << std::endl;
    } else {
        std::cout << query << " do NOT match!" << std::endl;
    }
    //========== Query 5 ===========
    query = "Query 5";
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result1 = con.Query("SELECT N_NAME, SUM(discount_price(L_EXTENDEDPRICE, L_DISCOUNT)) AS REVENUE FROM CUSTOMER, ORDERS, LINEITEM, SUPPLIER, NATION, REGION WHERE C_CUSTKEY = O_CUSTKEY AND L_ORDERKEY = O_ORDERKEY AND L_SUPPKEY = S_SUPPKEY AND C_NATIONKEY = S_NATIONKEY AND S_NATIONKEY = N_NATIONKEY AND N_REGIONKEY = R_REGIONKEY AND q5Conditions(R_NAME, O_ORDERDATE) = 1 GROUP BY N_NAME ORDER BY REVENUE DESC  ");
        end = std::chrono::steady_clock::now();
        if(result1->HasError())
            std::cout << result1->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    std::cout << query << " UDF time: " << mean(time) << " ms" << std::endl;
    time.clear();
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result2 = con.Query("select 	n_name, 	sum(l_extendedprice * (1 - l_discount)) as revenue from 	customer, 	orders, 	lineitem, 	supplier, 	nation, 	region where 	c_custkey = o_custkey 	and l_orderkey = o_orderkey 	and l_suppkey = s_suppkey 	and c_nationkey = s_nationkey 	and s_nationkey = n_nationkey 	and n_regionkey = r_regionkey 	and r_name = 'ASIA' 	and o_orderdate >= date '1994-01-01' 	and o_orderdate < date '1994-01-01' + interval '1' year group by 	n_name order by 	revenue desc;  ");
        end = std::chrono::steady_clock::now();
        if(result2->HasError())
            std::cout << result2->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        
    }
    std::cout << query << " time: " << mean(time) << " ms" << std::endl;
    std::cout << result1->ToString() << "\n--------------------------------------------------------\n" << result2->ToString() << std::endl;
    if (result1->Equals(*result2)) {
        std::cout << query << " Matches!" << std::endl;
    } else {
        std::cout << query << " do NOT match!" << std::endl;
    }
    //========== Query 6 ===========
    query = "Query 6";
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result1 = con.Query("SELECT SUM(L_EXTENDEDPRICE*L_DISCOUNT) AS REVENUE FROM LINEITEM WHERE q6conditions(L_SHIPDATE, L_DISCOUNT, L_QUANTITY) = 1;  ");
        end = std::chrono::steady_clock::now();
        if(result1->HasError())
            std::cout << result1->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    std::cout << query << " UDF time: " << mean(time) << " ms" << std::endl;
    time.clear();
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result2 = con.Query("select 	sum(l_extendedprice * l_discount) as revenue from 	lineitem where 	l_shipdate >= date '1994-01-01' 	and l_shipdate < date '1994-01-01' + interval '1' year 	and l_discount between .06 - 0.01 and .06 + 0.01 	and l_quantity < 24;  ");
        end = std::chrono::steady_clock::now();
        if(result2->HasError())
            std::cout << result2->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        
    }
    std::cout << query << " time: " << mean(time) << " ms" << std::endl;
    std::cout << result1->ToString() << "\n--------------------------------------------------------\n" << result2->ToString() << std::endl;
    if (result1->Equals(*result2)) {
        std::cout << query << " Matches!" << std::endl;
    } else {
        std::cout << query << " do NOT match!" << std::endl;
    }

    //========== Query 7 ===========
    query = "Query 7";
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result1 = con.Query("SELECT SUPP_NATION, CUST_NATION, L_YEAR, SUM(VOLUME) AS REVENUE FROM ( SELECT N1.N_NAME AS SUPP_NATION, N2.N_NAME AS CUST_NATION, date_part('year', L_SHIPDATE) AS L_YEAR, L_EXTENDEDPRICE*(1-L_DISCOUNT) AS VOLUME FROM SUPPLIER, LINEITEM, ORDERS, CUSTOMER, NATION N1, NATION N2 WHERE S_SUPPKEY = L_SUPPKEY AND O_ORDERKEY = L_ORDERKEY AND C_CUSTKEY = O_CUSTKEY AND S_NATIONKEY = N1.N_NATIONKEY AND C_NATIONKEY = N2.N_NATIONKEY AND q7conditions(N1.N_NAME, N2.N_NAME, L_SHIPDATE) = 1 ) AS SHIPPING GROUP BY SUPP_NATION, CUST_NATION, L_YEAR ORDER BY SUPP_NATION, CUST_NATION, L_YEAR  ");
        end = std::chrono::steady_clock::now();
        if(result1->HasError())
            std::cout << result1->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    std::cout << query << " UDF time: " << mean(time) << " ms" << std::endl;
    time.clear();
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result2 = con.Query("select 	supp_nation, 	cust_nation, 	l_year, 	sum(volume) as revenue from 	( 		select 			n1.n_name as supp_nation, 			n2.n_name as cust_nation, 			extract(year from l_shipdate) as l_year, 			l_extendedprice * (1 - l_discount) as volume 		from 			supplier, 			lineitem, 			orders, 			customer, 			nation n1, 			nation n2 		where 			s_suppkey = l_suppkey 			and o_orderkey = l_orderkey 			and c_custkey = o_custkey 			and s_nationkey = n1.n_nationkey 			and c_nationkey = n2.n_nationkey 			and ( 				(n1.n_name = 'FRANCE' and n2.n_name = 'GERMANY') 				or (n1.n_name = 'GERMANY' and n2.n_name = 'FRANCE') 			) 			and l_shipdate between date '1995-01-01' and date '1996-12-31' 	) as shipping group by 	supp_nation, 	cust_nation, 	l_year order by 	supp_nation, 	cust_nation, 	l_year;  ");
        end = std::chrono::steady_clock::now();
        if(result2->HasError())
            std::cout << result2->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        
    }
    std::cout << query << " time: " << mean(time) << " ms" << std::endl;
    std::cout << result1->ToString() << "\n--------------------------------------------------------\n" << result2->ToString() << std::endl;
    if (result1->Equals(*result2)) {
        std::cout << query << " Matches!" << std::endl;
    } else {
        std::cout << query << " do NOT match!" << std::endl;
    }

    //========== Query 9 ===========
    query = "Query 9";
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result1 = con.Query("SELECT NATION, O_YEAR, SUM(AMOUNT) AS SUM_PROFIT FROM (SELECT N_NAME AS NATION, date_part('year', O_ORDERDATE) AS O_YEAR, profit_amount(L_EXTENDEDPRICE, L_DISCOUNT, PS_SUPPLYCOST, L_QUANTITY) AS AMOUNT FROM PART, SUPPLIER, LINEITEM, PARTSUPP, ORDERS, NATION WHERE S_SUPPKEY = L_SUPPKEY AND PS_SUPPKEY= L_SUPPKEY AND PS_PARTKEY = L_PARTKEY AND P_PARTKEY= L_PARTKEY AND O_ORDERKEY = L_ORDERKEY AND S_NATIONKEY = N_NATIONKEY AND P_NAME LIKE '%%green%%') AS PROFIT GROUP BY NATION, O_YEAR ORDER BY NATION, O_YEAR DESC  ");
        end = std::chrono::steady_clock::now();
        if(result1->HasError())
            std::cout << result1->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    std::cout << query << " UDF time: " << mean(time) << " ms" << std::endl;
    time.clear();
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result2 = con.Query("select 	nation, 	o_year, 	sum(amount) as sum_profit from 	( 		select 			n_name as nation, 			extract(year from o_orderdate) as o_year, 			l_extendedprice * (1 - l_discount) - ps_supplycost * l_quantity as amount 		from 			part, 			supplier, 			lineitem, 			partsupp, 			orders, 			nation 		where 			s_suppkey = l_suppkey 			and ps_suppkey = l_suppkey 			and ps_partkey = l_partkey 			and p_partkey = l_partkey 			and o_orderkey = l_orderkey 			and s_nationkey = n_nationkey 			and p_name like '%green%' 	) as profit group by 	nation, 	o_year order by 	nation, 	o_year desc;  ");
        end = std::chrono::steady_clock::now();
        if(result2->HasError())
            std::cout << result2->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        
    }
    std::cout << query << " time: " << mean(time) << " ms" << std::endl;
    std::cout << result1->ToString() << "\n--------------------------------------------------------\n" << result2->ToString() << std::endl;
    if (result1->Equals(*result2)) {
        std::cout << query << " Matches!" << std::endl;
    } else {
        std::cout << query << " do NOT match!" << std::endl;
    }

    //========== Query 10 ===========
    query = "Query 10";
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result1 = con.Query("SELECT C_CUSTKEY, C_NAME, SUM(discount_price(L_EXTENDEDPRICE, L_DISCOUNT)) AS REVENUE, C_ACCTBAL, N_NAME, C_ADDRESS, C_PHONE, C_COMMENT FROM CUSTOMER, ORDERS, LINEITEM, NATION WHERE C_CUSTKEY = O_CUSTKEY AND L_ORDERKEY = O_ORDERKEY AND q10conditions(O_ORDERDATE, L_RETURNFLAG) = 1 AND C_NATIONKEY = N_NATIONKEY GROUP BY C_CUSTKEY, C_NAME, C_ACCTBAL, C_PHONE, N_NAME, C_ADDRESS, C_COMMENT ORDER BY REVENUE DESC  LIMIT 20");
        end = std::chrono::steady_clock::now();
        if(result1->HasError())
            std::cout << result1->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    std::cout << query << " UDF time: " << mean(time) << " ms" << std::endl;
    time.clear();
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result2 = con.Query("select 	c_custkey, 	c_name, 	sum(l_extendedprice * (1 - l_discount)) as revenue, 	c_acctbal, 	n_name, 	c_address, 	c_phone, 	c_comment from 	customer, 	orders, 	lineitem, 	nation where 	c_custkey = o_custkey 	and l_orderkey = o_orderkey 	and o_orderdate >= date '1993-10-01' 	and o_orderdate < date '1993-10-01' + interval '3' month 	and l_returnflag = 'R' 	and c_nationkey = n_nationkey group by 	c_custkey, 	c_name, 	c_acctbal, 	c_phone, 	n_name, 	c_address, 	c_comment order by 	revenue desc limit 20; ");
        end = std::chrono::steady_clock::now();
        if(result2->HasError())
            std::cout << result2->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        
    }
    std::cout << query << " time: " << mean(time) << " ms" << std::endl;
    std::cout << result1->ToString() << "\n--------------------------------------------------------\n" << result2->ToString() << std::endl;
    if (result1->Equals(*result2)) {
        std::cout << query << " Matches!" << std::endl;
    } else {
        std::cout << query << " do NOT match!" << std::endl;
    }

    //========== Query 11 ===========
    query = "Query 11";
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result1 = con.Query("SELECT PS_PARTKEY, SUM(PS_SUPPLYCOST*PS_AVAILQTY) AS VALUE FROM PARTSUPP, SUPPLIER, NATION WHERE PS_SUPPKEY = S_SUPPKEY AND S_NATIONKEY = N_NATIONKEY AND N_NAME = 'GERMANY' GROUP BY PS_PARTKEY HAVING SUM(PS_SUPPLYCOST*PS_AVAILQTY) >  (SELECT SUM(PS_SUPPLYCOST*PS_AVAILQTY) * 0.0001000000 FROM PARTSUPP, SUPPLIER, NATION WHERE PS_SUPPKEY = S_SUPPKEY AND S_NATIONKEY = N_NATIONKEY AND N_NAME = 'GERMANY') ORDER BY VALUE DESC  ");
        end = std::chrono::steady_clock::now();
        if(result1->HasError())
            std::cout << result1->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    std::cout << query << " UDF time: " << mean(time) << " ms" << std::endl;
    time.clear();
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result2 = con.Query("select 	ps_partkey, 	sum(ps_supplycost * ps_availqty) as value from 	partsupp, 	supplier, 	nation where 	ps_suppkey = s_suppkey 	and s_nationkey = n_nationkey 	and n_name = 'GERMANY' group by 	ps_partkey having 		sum(ps_supplycost * ps_availqty) > ( 			select 				sum(ps_supplycost * ps_availqty) * 0.0001000000 			from 				partsupp, 				supplier, 				nation 			where 				ps_suppkey = s_suppkey 				and s_nationkey = n_nationkey 				and n_name = 'GERMANY' 		) order by 	value desc;  ");
        end = std::chrono::steady_clock::now();
        if(result2->HasError())
            std::cout << result2->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        
    }
    std::cout << query << " time: " << mean(time) << " ms" << std::endl;
    std::cout << result1->ToString() << "\n--------------------------------------------------------\n" << result2->ToString() << std::endl;
    if (result1->Equals(*result2)) {
        std::cout << query << " Matches!" << std::endl;
    } else {
        std::cout << query << " do NOT match!" << std::endl;
    }

    //========== Query 12 ===========
    query = "Query 12";
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result1 = con.Query("SELECT L_SHIPMODE, SUM(line_count(O_ORDERPRIORITY, 'high')) AS HIGH_LINE_COUNT, SUM(line_count(O_ORDERPRIORITY, 'low')) AS LOW_LINE_COUNT FROM ORDERS, LINEITEM WHERE O_ORDERKEY = L_ORDERKEY AND q12conditions(L_SHIPMODE, L_COMMITDATE, L_RECEIPTDATE, L_SHIPDATE) = 1 GROUP BY L_SHIPMODE ORDER BY L_SHIPMODE  ");
        end = std::chrono::steady_clock::now();
        if(result1->HasError())
            std::cout << result1->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    std::cout << query << " UDF time: " << mean(time) << " ms" << std::endl;
    time.clear();
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result2 = con.Query("select 	l_shipmode, 	sum(case 		when o_orderpriority = '1-URGENT' 			or o_orderpriority = '2-HIGH' 			then 1 		else 0 	end) as high_line_count, 	sum(case 		when o_orderpriority <> '1-URGENT' 			and o_orderpriority <> '2-HIGH' 			then 1 		else 0 	end) as low_line_count from 	orders, 	lineitem where 	o_orderkey = l_orderkey 	and l_shipmode in ('MAIL', 'SHIP') 	and l_commitdate < l_receiptdate 	and l_shipdate < l_commitdate 	and l_receiptdate >= date '1994-01-01' 	and l_receiptdate < date '1994-01-01' + interval '1' year group by 	l_shipmode order by 	l_shipmode;  ");
        end = std::chrono::steady_clock::now();
        if(result2->HasError())
            std::cout << result2->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        
    }
    std::cout << query << " time: " << mean(time) << " ms" << std::endl;
    std::cout << result1->ToString() << "\n--------------------------------------------------------\n" << result2->ToString() << std::endl;
    if (result1->Equals(*result2)) {
        std::cout << query << " Matches!" << std::endl;
    } else {
        std::cout << query << " do NOT match!" << std::endl;
    }

    //========== Query 14 ===========
    query = "Query 14";
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result1 = con.Query("SELECT 100.00 * SUM(promo_disc(P_TYPE, L_EXTENDEDPRICE, L_DISCOUNT)) / SUM(discount_price(L_EXTENDEDPRICE,L_DISCOUNT)) AS PROMO_REVENUE FROM LINEITEM, PART WHERE L_PARTKEY = P_PARTKEY AND L_SHIPDATE >= '1995-09-01' AND L_SHIPDATE < (DATE '1995-09-01') + (INTERVAL '1 MONTH') -- dateadd(mm, 1, '1995-09-01')  ");
        end = std::chrono::steady_clock::now();
        if(result1->HasError())
            std::cout << result1->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    std::cout << query << " UDF time: " << mean(time) << " ms" << std::endl;
    time.clear();
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result2 = con.Query("select 	100.00 * sum(case 		when p_type like 'PROMO%' 			then l_extendedprice * (1 - l_discount) 		else 0 	end) / sum(l_extendedprice * (1 - l_discount)) as promo_revenue from 	lineitem, 	part where 	l_partkey = p_partkey 	and l_shipdate >= date '1995-09-01' 	and l_shipdate < date '1995-09-01' + interval '1' month;  ");
        end = std::chrono::steady_clock::now();
        if(result2->HasError())
            std::cout << result2->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        
    }
    std::cout << query << " time: " << mean(time) << " ms" << std::endl;
    std::cout << result1->ToString() << "\n--------------------------------------------------------\n" << result2->ToString() << std::endl;
    if (result1->Equals(*result2)) {
        std::cout << query << " Matches!" << std::endl;
    } else {
        std::cout << query << " do NOT match!" << std::endl;
    }

    //========== Query 19 ===========
    query = "Query 19";
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result1 = con.Query("SELECT SUM(discount_price(L_EXTENDEDPRICE, L_DISCOUNT)) AS REVENUE FROM LINEITEM join PART on L_PARTKEY = P_PARTKEY WHERE q19conditions(P_CONTAINER, L_QUANTITY, P_SIZE, L_SHIPMODE, L_SHIPINSTRUCT, P_BRAND ) = 1;  ");
        end = std::chrono::steady_clock::now();
        if(result1->HasError())
            std::cout << result1->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    std::cout << query << " UDF time: " << mean(time) << " ms" << std::endl;
    time.clear();
    for(int i=0;i<EPOCH_NUM;i++){
        start = std::chrono::steady_clock::now();
        result2 = con.Query("select 	sum(l_extendedprice* (1 - l_discount)) as revenue from 	lineitem, 	part where 	( 		p_partkey = l_partkey 		and p_brand = 'Brand#12' 		and p_container in ('SM CASE', 'SM BOX', 'SM PACK', 'SM PKG') 		and l_quantity >= 1 and l_quantity <= 1 + 10 		and p_size between 1 and 5 		and l_shipmode in ('AIR', 'AIR REG') 		and l_shipinstruct = 'DELIVER IN PERSON' 	) 	or 	( 		p_partkey = l_partkey 		and p_brand = 'Brand#23' 		and p_container in ('MED BAG', 'MED BOX', 'MED PKG', 'MED PACK') 		and l_quantity >= 10 and l_quantity <= 10 + 10 		and p_size between 1 and 10 		and l_shipmode in ('AIR', 'AIR REG') 		and l_shipinstruct = 'DELIVER IN PERSON' 	) 	or 	( 		p_partkey = l_partkey 		and p_brand = 'Brand#34' 		and p_container in ('LG CASE', 'LG BOX', 'LG PACK', 'LG PKG') 		and l_quantity >= 20 and l_quantity <= 20 + 10 		and p_size between 1 and 15 		and l_shipmode in ('AIR', 'AIR REG') 		and l_shipinstruct = 'DELIVER IN PERSON' 	);  ");
        end = std::chrono::steady_clock::now();
        if(result2->HasError())
            std::cout << result2->GetError() << std::endl;
        time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        
    }
    std::cout << query << " time: " << mean(time) << " ms" << std::endl;
    std::cout << result1->ToString() << "\n--------------------------------------------------------\n" << result2->ToString() << std::endl;
    if (result1->Equals(*result2)) {
        std::cout << query << " Matches!" << std::endl;
    } else {
        std::cout << query << " do NOT match!" << std::endl;
    }

    //========== Query 22 ===========
    // query = "Query 22";
    // for(int i=0;i<EPOCH_NUM;i++){
    //     start = std::chrono::steady_clock::now();
    //     result1 = con.Query("SELECT CNTRYCODE, COUNT(*) AS NUMCUST, SUM(C_ACCTBAL) AS TOTACCTBAL FROM (SELECT SUBSTRING(C_PHONE,1,2) AS CNTRYCODE, C_ACCTBAL FROM CUSTOMER WHERE SUBSTRING(C_PHONE,1,2) IN ('13', '31', '23', '29', '30', '18', '17') AND C_ACCTBAL > (SELECT AVG(C_ACCTBAL) FROM CUSTOMER WHERE C_ACCTBAL > 0.00 AND SUBSTRING(C_PHONE,1,2) IN ('13', '31', '23', '29', '30', '18', '17')) AND NOT EXISTS ( SELECT * FROM ORDERS WHERE O_CUSTKEY = C_CUSTKEY)) AS CUSTSALE GROUP BY CNTRYCODE ORDER BY CNTRYCODE ");
    //     end = std::chrono::steady_clock::now();
    //     if(result1->HasError())
    //         std::cout << result1->GetError() << std::endl;
    //     time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    // }
    // std::cout << query << " UDF time: " << mean(time) << " ms" << std::endl;
    // time.clear();
    // for(int i=0;i<EPOCH_NUM;i++){
    //     start = std::chrono::steady_clock::now();
    //     result2 = con.Query("select 	cntrycode, 	count(*) as numcust, 	sum(c_acctbal) as totacctbal from 	( 		select 			substring(c_phone from 1 for 2) as cntrycode, 			c_acctbal 		from 			customer 		where 			substring(c_phone from 1 for 2) in 				('13', '31', '23', '29', '30', '18', '17') 			and c_acctbal > ( 				select 					avg(c_acctbal) 				from 					customer 				where 					c_acctbal > 0.00 					and substring(c_phone from 1 for 2) in 						('13', '31', '23', '29', '30', '18', '17') 			) 			and not exists ( 				select 					* 				from 					orders 				where 					o_custkey = c_custkey 			) 	) as custsale group by 	cntrycode order by 	cntrycode;  ");
    //     end = std::chrono::steady_clock::now();
    //     if(result2->HasError())
    //         std::cout << result2->GetError() << std::endl;
    //     time.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        
    // }
    // std::cout << query << " time: " << mean(time) << " ms" << std::endl;
    // std::cout << result1->ToString() << "\n--------------------------------------------------------\n" << result2->ToString() << std::endl;
    // if (result1->Equals(*result2)) {
    //     std::cout << query << " Matches!" << std::endl;
    // } else {
    //     std::cout << query << " do NOT match!" << std::endl;
    // }

    
    
    return 0;
}