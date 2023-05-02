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
    con->Query("CREATE TABLE lineitem AS SELECT * FROM read_csv_auto('dataset/lineitem.csv')");
    con->CreateVectorizedFunction("if_func", {duckdb::LogicalType::BIGINT}, duckdb::LogicalType::VARCHAR, &if_func);
}

static const int EPOCH_NUM = 1;
int main(int argc, char const *argv[])
{
    duckdb::Connection con(main_db);
    prepare_env(&con);
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::chrono::time_point<std::chrono::steady_clock> end;
    //========== Test 1 ===========
    std::vector<double> time1;
    for(int i=0;i<EPOCH_NUM;i++){
        // start = std::clock();
        start = std::chrono::steady_clock::now();
        auto result1 = con.Query("select if_func(l_quantity) from lineitem");
        // auto result1 = con.Query("select l_shipdate + 1 from lineitem");
        end = std::chrono::steady_clock::now();
        if(result1->HasError())
            std::cout << result1->GetError() << std::endl;
        // result1-;
        time1.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }
    std::cout << "Time 1: " << mean(time1) << " ms" << std::endl;
    
    return 0;
}