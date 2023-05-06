 
-- Query 1  
select 	l_returnflag, 	l_linestatus, 	sum(l_quantity) as sum_qty, 	sum(l_extendedprice) as sum_base_price, 	sum(l_extendedprice * (1 - l_discount)) as sum_disc_price, 	sum(l_extendedprice * (1 - l_discount) * (1 + l_tax)) as sum_charge, 	avg(l_quantity) as avg_qty, 	avg(l_extendedprice) as avg_price, 	avg(l_discount) as avg_disc, 	count(*) as count_order from 	lineitem where 	l_shipdate <= date '1998-12-01' - interval '90' day group by 	l_returnflag, 	l_linestatus order by 	l_returnflag, 	l_linestatus;  
-- Query 3  
select 	l_orderkey, 	sum(l_extendedprice * (1 - l_discount)) as revenue, 	o_orderdate, 	o_shippriority from 	customer, 	orders, 	lineitem where 	c_mktsegment = 'BUILDING' 	and c_custkey = o_custkey 	and l_orderkey = o_orderkey 	and o_orderdate < date '1995-03-15' 	and l_shipdate > date '1995-03-15' group by 	l_orderkey, 	o_orderdate, 	o_shippriority order by 	revenue desc, 	o_orderdate limit 10; 
-- Query 5  
select 	n_name, 	sum(l_extendedprice * (1 - l_discount)) as revenue from 	customer, 	orders, 	lineitem, 	supplier, 	nation, 	region where 	c_custkey = o_custkey 	and l_orderkey = o_orderkey 	and l_suppkey = s_suppkey 	and c_nationkey = s_nationkey 	and s_nationkey = n_nationkey 	and n_regionkey = r_regionkey 	and r_name = 'ASIA' 	and o_orderdate >= date '1994-01-01' 	and o_orderdate < date '1994-01-01' + interval '1' year group by 	n_name order by 	revenue desc;  
-- Query 6  
select 	sum(l_extendedprice * l_discount) as revenue from 	lineitem where 	l_shipdate >= date '1994-01-01' 	and l_shipdate < date '1994-01-01' + interval '1' year 	and l_discount between .06 - 0.01 and .06 + 0.01 	and l_quantity < 24;  
-- Query 7  
select 	supp_nation, 	cust_nation, 	l_year, 	sum(volume) as revenue from 	( 		select 			n1.n_name as supp_nation, 			n2.n_name as cust_nation, 			extract(year from l_shipdate) as l_year, 			l_extendedprice * (1 - l_discount) as volume 		from 			supplier, 			lineitem, 			orders, 			customer, 			nation n1, 			nation n2 		where 			s_suppkey = l_suppkey 			and o_orderkey = l_orderkey 			and c_custkey = o_custkey 			and s_nationkey = n1.n_nationkey 			and c_nationkey = n2.n_nationkey 			and ( 				(n1.n_name = 'FRANCE' and n2.n_name = 'GERMANY') 				or (n1.n_name = 'GERMANY' and n2.n_name = 'FRANCE') 			) 			and l_shipdate between date '1995-01-01' and date '1996-12-31' 	) as shipping group by 	supp_nation, 	cust_nation, 	l_year order by 	supp_nation, 	cust_nation, 	l_year;  
-- Query 9  
select 	nation, 	o_year, 	sum(amount) as sum_profit from 	( 		select 			n_name as nation, 			extract(year from o_orderdate) as o_year, 			l_extendedprice * (1 - l_discount) - ps_supplycost * l_quantity as amount 		from 			part, 			supplier, 			lineitem, 			partsupp, 			orders, 			nation 		where 			s_suppkey = l_suppkey 			and ps_suppkey = l_suppkey 			and ps_partkey = l_partkey 			and p_partkey = l_partkey 			and o_orderkey = l_orderkey 			and s_nationkey = n_nationkey 			and p_name like '%green%' 	) as profit group by 	nation, 	o_year order by 	nation, 	o_year desc;  
-- Query 10 
select 	c_custkey, 	c_name, 	sum(l_extendedprice * (1 - l_discount)) as revenue, 	c_acctbal, 	n_name, 	c_address, 	c_phone, 	c_comment from 	customer, 	orders, 	lineitem, 	nation where 	c_custkey = o_custkey 	and l_orderkey = o_orderkey 	and o_orderdate >= date '1993-10-01' 	and o_orderdate < date '1993-10-01' + interval '3' month 	and l_returnflag = 'R' 	and c_nationkey = n_nationkey group by 	c_custkey, 	c_name, 	c_acctbal, 	c_phone, 	n_name, 	c_address, 	c_comment order by 	revenue desc limit 20; 
-- Query 11 
select 	ps_partkey, 	sum(ps_supplycost * ps_availqty) as value from 	partsupp, 	supplier, 	nation where 	ps_suppkey = s_suppkey 	and s_nationkey = n_nationkey 	and n_name = 'GERMANY' group by 	ps_partkey having 		sum(ps_supplycost * ps_availqty) > ( 			select 				sum(ps_supplycost * ps_availqty) * 0.0001000000 			from 				partsupp, 				supplier, 				nation 			where 				ps_suppkey = s_suppkey 				and s_nationkey = n_nationkey 				and n_name = 'GERMANY' 		) order by 	value desc;  
-- Query 12 
select 	l_shipmode, 	sum(case 		when o_orderpriority = '1-URGENT' 			or o_orderpriority = '2-HIGH' 			then 1 		else 0 	end) as high_line_count, 	sum(case 		when o_orderpriority <> '1-URGENT' 			and o_orderpriority <> '2-HIGH' 			then 1 		else 0 	end) as low_line_count from 	orders, 	lineitem where 	o_orderkey = l_orderkey 	and l_shipmode in ('MAIL', 'SHIP') 	and l_commitdate < l_receiptdate 	and l_shipdate < l_commitdate 	and l_receiptdate >= date '1994-01-01' 	and l_receiptdate < date '1994-01-01' + interval '1' year group by 	l_shipmode order by 	l_shipmode;  
-- Query 14 
select 	100.00 * sum(case 		when p_type like 'PROMO%' 			then l_extendedprice * (1 - l_discount) 		else 0 	end) / sum(l_extendedprice * (1 - l_discount)) as promo_revenue from 	lineitem, 	part where 	l_partkey = p_partkey 	and l_shipdate >= date '1995-09-01' 	and l_shipdate < date '1995-09-01' + interval '1' month;  
-- Query 19 
select 	sum(l_extendedprice* (1 - l_discount)) as revenue from 	lineitem, 	part where 	( 		p_partkey = l_partkey 		and p_brand = 'Brand#12' 		and p_container in ('SM CASE', 'SM BOX', 'SM PACK', 'SM PKG') 		and l_quantity >= 1 and l_quantity <= 1 + 10 		and p_size between 1 and 5 		and l_shipmode in ('AIR', 'AIR REG') 		and l_shipinstruct = 'DELIVER IN PERSON' 	) 	or 	( 		p_partkey = l_partkey 		and p_brand = 'Brand#23' 		and p_container in ('MED BAG', 'MED BOX', 'MED PKG', 'MED PACK') 		and l_quantity >= 10 and l_quantity <= 10 + 10 		and p_size between 1 and 10 		and l_shipmode in ('AIR', 'AIR REG') 		and l_shipinstruct = 'DELIVER IN PERSON' 	) 	or 	( 		p_partkey = l_partkey 		and p_brand = 'Brand#34' 		and p_container in ('LG CASE', 'LG BOX', 'LG PACK', 'LG PKG') 		and l_quantity >= 20 and l_quantity <= 20 + 10 		and p_size between 1 and 15 		and l_shipmode in ('AIR', 'AIR REG') 		and l_shipinstruct = 'DELIVER IN PERSON' 	);  
-- Query 22 
select 	cntrycode, 	count(*) as numcust, 	sum(c_acctbal) as totacctbal from 	( 		select 			substring(c_phone from 1 for 2) as cntrycode, 			c_acctbal 		from 			customer 		where 			substring(c_phone from 1 for 2) in 				('13', '31', '23', '29', '30', '18', '17') 			and c_acctbal > ( 				select 					avg(c_acctbal) 				from 					customer 				where 					c_acctbal > 0.00 					and substring(c_phone from 1 for 2) in 						('13', '31', '23', '29', '30', '18', '17') 			) 			and not exists ( 				select 					* 				from 					orders 				where 					o_custkey = c_custkey 			) 	) as custsale group by 	cntrycode order by 	cntrycode;  