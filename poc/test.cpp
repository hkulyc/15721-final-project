#include "duckdb.hpp"
// #include <ctime>
#include <chrono>
#include <iostream>
#include <vector>
#include "conn_pool.hpp"
using namespace duckdb;
// #include "../libduckdb-osx-universal/duckdb.h"
#define EPOCH_NUM 1

duckdb::DuckDB db(nullptr);
duckdb::DuckDB db2(nullptr);

// std::unique_ptr<duckdb::PreparedStatement> prepare = con2.Prepare("SELECT ($1+1)/2*1.5");
// int64_t random_op(int64_t a){
//     std::cout<<a<<std::endl;
//     std::cout<<(a+1)/2*1.5<<std::endl;
//     prepare->Execute(a)->
//     return a;
// }

Connection_Pool conn_pool(&db2, 10);

int64_t add_one(int64_t a){
    int id;
    PreparedStatement *state = conn_pool.get_prepared(0, &id);
    if(state == NULL){
        throw std::runtime_error("No connection is free.");
        return 0;
    }
    // conn_pool.release_prepared(id);
    // return id;
    auto res = state->Execute(a);
    if(!res->HasError()){
        int64_t ret = res->Fetch()->GetValue(0,0).GetValue<int64_t>();
        conn_pool.release_prepared(id);
        return ret;
    }
    else {
        throw std::runtime_error("Error when waiting for query result.");
        return 0;
    }
}

thread_local duckdb::Connection con2(db2);
// duckdb::Connection con2(db);
thread_local std::unique_ptr<duckdb::PreparedStatement> prepare = con2.Prepare("SELECT $1+1");
// auto statement = con2.ExtractStatements("SELECT $1+1");
int64_t random_op(int64_t a){
    // duckdb::Connection con2(db);
    // std::cout<<a<<std::endl;
    // std::cout<<(a+1)/2*1.5<<std::endl;
    // std::unique_ptr<duckdb::PreparedStatement> prepare = con2.Prepare("SELECT $1+1");
    auto res = prepare->Execute(a);
    // char *sql;
    // asprintf(&sql, "select %lld+1", a);
    // auto res = con2.Query(sql);
    // res->Print();
    // Executor exe(*con2.context);
    // // client_context 311
    // PhysicalPlanGenerator physical_planner(*con2.context);
    // Planner planner(*con2.context);
    // auto physical_plan = physical_planner.CreatePlan(std::move(plan));
    // exe.Initialize(std::move(physical_plan));
    // exe.ExecuteTask(TaskExecutionMode::PROCESS_ALL);




    if(!res->HasError())
        return res->Fetch()->GetValue(0,0).GetValue<int64_t>();
    // if(res != NULL)
    //     return res->begin().current_row.GetValue<int64_t>(0);
    else return 1;
    // return a;
}

/*
* This vectorized function copies the input values to the result vector
*/
template<typename TYPE>
static void udf_vectorized(DataChunk &args, ExpressionState &state, Vector &result) {
	// set the result vector type
	// result.SetVectorType(VectorType::FLAT_VECTOR);
	// // get a raw array from the result
	// auto result_data = FlatVector::GetData<TYPE>(result);

	// // get the solely input vector
	// auto &input = args.data[0];
	// // now get an orrified vector
	// // VectorData vdata;
    // UnifiedVectorFormat vdata;
	// input.ToUnifiedFormat(args.size(), vdata);

	// // get a raw array from the orrified input
	// auto input_data = (TYPE *)vdata.data;

    // // duckdb::Connection con2(db);
    // // std::unique_ptr<duckdb::PreparedStatement> prepare = con2.Prepare("SELECT $1+1");

	// // handling the data
	// for (idx_t i = 0; i < args.size(); i++) {
	// 	auto idx = vdata.sel->get_index(i);
	// 	// if ((*vdata.nullmask)[idx]) {
	// 	// 	continue;
	// 	// }
    //     TYPE a = input_data[idx];
    //     // a = prepare->Execute(a)->Fetch()->GetValue(0,0).template GetValue<TYPE>();
    //     // std::cout<<a<<std::endl;
    //     auto res = prepare->Execute(a);
    //     if(!res->HasError())
    //         result_data[i] = res->Fetch()->GetValue(0,0).template GetValue<int64_t>();
    //     else result_data[i] = a;
	// }
}


int64_t classify(int64_t a){
    if(a <= 2000) return 0;
    else if(a <= 5000) return 1;
    else return 2;
}

// std::unique_ptr<duckdb::PreparedStatement> prepare = con2.Prepare("SELECT $1");
// double test(double a){
//     // std::cout<<a<<std::endl;
//     std::unique_ptr<duckdb::QueryResult> result = prepare->Execute("12");
//     // auto result = con2.Query("select 1");
//     // result->Print();
//     result->Fetch()->Print();
//     // std::cout<<result->Fetch()->Print()<<std::endl;
//     return a;
// }

void prepare_env(duckdb::Connection *con){
    // std::string table_schema =
    // "CREATE TABLE lineitem ("
    // "l_shipdate date DEFAULT NULL,"
    // "l_orderkey bigint(20) NOT NULL,"
    // "l_discount decimal(19,4) NOT NULL,"
    // "l_extendedprice decimal(19,4) NOT NULL,"
    // "l_suppkey int(11) NOT NULL,"
    // "l_quantity bigint(20) NOT NULL,"
    // "l_returnflag char(1) DEFAULT NULL,"
    // "l_partkey bigint(20) NOT NULL,"
    // "l_linestatus char(1) DEFAULT NULL,"
    // "l_tax decimal(19,4) NOT NULL,"
    // "l_commitdate date DEFAULT NULL,"
    // "l_receiptdate date DEFAULT NULL,"
    // "l_shipmode char(10) DEFAULT NULL,"
    // "l_linenumber bigint(20) NOT NULL,"
    // "l_shipinstruct char(25) DEFAULT NULL,"
    // "l_comment varchar(44) DEFAULT NULL,"
    // "PRIMARY KEY (l_orderkey,l_linenumber))";
    // con->Query(table_schema);
    // con->Query("copy lineitem from 'lineitem.csv' (AUTO_DETECT TRUE)");
    conn_pool.add_prepared("select $1+1");

    con->Query("CREATE TABLE lineitem AS SELECT * FROM read_csv_auto('dataset/lineitem.csv')");
    con->CreateScalarFunction<int64_t, int64_t>("random_op", &random_op);
    con->CreateVectorizedFunction<int64_t, int64_t>("udf_vectorized_int", &udf_vectorized<int64_t>);
    con->CreateScalarFunction<int64_t, int64_t>("add_one", &add_one);
    // con->CreateScalarFunction<int64_t, int64_t>("classify", &classify);
    // con->Query("CREATE TABLE lineitem (id int, CO decimal(4,2) DEFAULT NULL, PRIMARY KEY (id))");
    // con->Query("insert into lineitem values (1,24.5), (2,2.45), (3, 0.12)");
    // con->CreateScalarFunction<double, double>("test", {duckdb::LogicalType::DOUBLE}, duckdb::LogicalType::DOUBLE, &test);
}

double mean(std::vector<double> v){
    double sum = 0;
    for(int i:v){
        sum += i;
    }
    return sum/v.size();
}

void print_res(duckdb::MaterializedQueryResult *res){
    int count = 0;
    for(auto i=res->begin();i!=res->end();i.Next()){
        std::cout<<i.current_row.GetValue<int32_t>(0)<<std::endl;
        count++;
        if(count >= 10) break;
    }
    return;
}

int main(int argc, char const *argv[])
{
    duckdb::Connection con(db);
    prepare_env(&con);
    // auto result1 = con.Query("select random_op(l_quantity) from lineitem");
    // auto result1 = con.Query("select udf_vectorized_int(l_quantity) from lineitem");
    // result1->Print();
    // auto result = con.Query("SELECT 42");
    // std::clock_t    start;
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;

    //========== Test Pair 1 ===========
    std::vector<double> time1;
    for(int i=0;i<EPOCH_NUM;i++){
        // start = std::clock();
        start = std::chrono::steady_clock::now();
        auto result1 = con.Query("select l_quantity+1 from lineitem");
        end = std::chrono::steady_clock::now();
        print_res(result1.get());
        // time1.push_back((std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000));
        time1.push_back(std::chrono::duration_cast<milliseconds>(end - start).count());
    }
    std::cout << "Time 1.1: " << mean(time1) << " ms" << std::endl;
    
    std::vector<double> time2;
    for(int i=0;i<EPOCH_NUM;i++){
        // start = std::clock();
        start = std::chrono::steady_clock::now();
        auto result2 = con.Query("select add_one(l_quantity) from lineitem");
        // auto result2 = con.Query("select udf_vectorized_int(l_quantity) from lineitem");
        end = std::chrono::steady_clock::now();
        print_res(result2.get());
        // time2.push_back((std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000));
        time2.push_back(std::chrono::duration_cast<milliseconds>(end - start).count());
    }
    std::cout << "Time 1.2: " << mean(time2) << " ms" << std::endl;

    // // ========== Test Pair 2 ===========
    // std::vector<double> time3;
    // for(int i=0;i<EPOCH_NUM;i++){
    //     start = std::clock();
    //     auto result3 = con.Query("select case when l_quantity <= 2000 then 0 when l_quantity <= 5000 then 1 else 2 end from lineitem");
    //     print_res(result3.get());
    //     time3.push_back((std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000));
    // }
    // std::cout << "Time 2.1: " << mean(time1) << " ms" << std::endl;
    
    // std::vector<double> time4;
    // for(int i=0;i<EPOCH_NUM;i++){
    //     start = std::clock();
    //     auto result4 = con.Query("select classify(l_quantity) from lineitem");
    //     print_res(result4.get());
    //     time4.push_back((std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000));
    // }
    // std::cout << "Time 2.2: " << mean(time2) << " ms" << std::endl;

    return 0;
}
