/* GLOBAL MACROS */
#include <stdexcept>
#include <vector>
#include <iostream>
#include "udf.hpp"

duckdb::DuckDB db(nullptr);
thread_local duckdb::Connection trans_con(db);

std::vector<duckdb::Value> const_vector_gen(duckdb::Value val){
    // generates vector_size copies of val to store in the vector
    std::vector<duckdb::Value> cons;
    cons.reserve(2048);
    for(int i=0;i<2048;i++){
        cons.emplace_back(val);
    }
    return cons;
}

template <typename T>
std::vector<duckdb::Value> const_vector_gen_decimal(T raw_int, int8_t width, int8_t scale){
    // generates vector_size copies of val to store in the vector
    std::vector<duckdb::Value> cons;
    cons.reserve(2048);
    for (int i = 0; i < 2048; i++){
        cons.emplace_back(duckdb::Value::DECIMAL(raw_int, width, scale));
    }
    return cons;
}
/* GLOBAL VARIABLES */
/* GLOBAL FUNCTIONS */
void if_func(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result) {
  duckdb::UnifiedVectorFormat vdata;
  std::vector<bool> returns1(args.size());
  std::vector<duckdb::Value> return_values(args.size());
  for(size_t i = 0; i < args.size() ; i++) {
    returns1[i] = false;
  }
  std::vector<bool> active2(args.size());
  for(size_t i = 0; i < args.size() ; i++) {
    active2[i] = true;
  }
  /** FUNCTION ARGS **/
  args.data[0].ToUnifiedFormat(args.size(), vdata);
std::vector<duckdb::Value> cid(args.size());
for (idx_t tempvar1 = 0; tempvar1 < args.size(); tempvar1++) {
  if (vdata.validity.GetValidityEntry(tempvar1)) {
    cid[tempvar1] = args.data[0].GetValue(tempvar1);
  }
}
args.data[1].ToUnifiedFormat(args.size(), vdata);
std::vector<duckdb::Value> cid2(args.size());
for (idx_t tempvar2 = 0; tempvar2 < args.size(); tempvar2++) {
  if (vdata.validity.GetValidityEntry(tempvar2)) {
    cid2[tempvar2] = args.data[1].GetValue(tempvar2);
  }
}

  /** INITIALIZE VARIABLES **/
  
  /** FUNCTION BODY **/
  /** RETURN **/
std::vector<duckdb::Value> tempvar4 = const_vector_gen_decimal<duckdb::hugeint_t>(5600, 18, 3);
for(size_t tempvar3 = 0 ; tempvar3 < args.size() ; tempvar3++) {
  if(active2[tempvar3] && !returns1[tempvar3]) {
    // note that this lane has returned
    returns1[tempvar3] = true;
    // set its return value
    return_values[tempvar3] = tempvar4[tempvar3];
  }
}
  /** SET RETURN VALUE **/
  for (idx_t i = 0; i < args.size(); i++) {
    result.SetValue(i, return_values[i]);
  }
}


