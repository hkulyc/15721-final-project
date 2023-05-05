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
bool equals(const duckdb::Value &v1, const duckdb::Value &v2){
  if(v1.IsNull() || v2.IsNull())
    return false;
  // else if(v1.IsNull() != v2.IsNull())
  //   return false;
  else
    return v1 == v2;
}

bool smallerthan(const duckdb::Value &v1, const duckdb::Value &v2){
  if(v1.IsNull() || v2.IsNull())
    return false;
  // else if(v1.IsNull() != v2.IsNull())
  //   return false;
  else
    return v1 < v2;
}

bool greaterorequalthan(const duckdb::Value &v1, const duckdb::Value &v2){
  if(v1.IsNull() || v2.IsNull())
    return false;
  // else if(v1.IsNull() != v2.IsNull())
  //   return false;
  else
    return v1 >= v2;
}

// /* GLOBAL FUNCTIONS */
// void checkDate(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result) {
//   duckdb::UnifiedVectorFormat vdata;
//   std::vector<bool> returns1(args.size());
//   std::vector<duckdb::Value> return_values(args.size());
//   for(size_t i = 0; i < args.size() ; i++) {
//     returns1[i] = false;
//   }
//   std::vector<bool> active2(args.size());
//   for(size_t i = 0; i < args.size() ; i++) {
//     active2[i] = true;
//   }
//   /** FUNCTION ARGS **/
//   args.data[0].ToUnifiedFormat(args.size(), vdata);
// std::vector<duckdb::Value> l_orderkey(args.size());
// for (idx_t tempvar1 = 0; tempvar1 < args.size(); tempvar1++) {
//   // if (vdata.validity.GetValidityEntry(tempvar1)) {
//     l_orderkey[tempvar1] = args.data[0].GetValue(tempvar1);
//   // }
// }
// args.data[1].ToUnifiedFormat(args.size(), vdata);
// std::vector<duckdb::Value> o_orderkey(args.size());
// for (idx_t tempvar2 = 0; tempvar2 < args.size(); tempvar2++) {
//   // if (vdata.validity.GetValidityEntry(tempvar2)) {
//     o_orderkey[tempvar2] = args.data[1].GetValue(tempvar2);
//   // }
// }
// args.data[2].ToUnifiedFormat(args.size(), vdata);
// std::vector<duckdb::Value> l_commitdate(args.size());
// for (idx_t tempvar3 = 0; tempvar3 < args.size(); tempvar3++) {
//   // if (vdata.validity.GetValidityEntry(tempvar3)) {
//     l_commitdate[tempvar3] = args.data[2].GetValue(tempvar3);
//   // }
// }
// args.data[3].ToUnifiedFormat(args.size(), vdata);
// std::vector<duckdb::Value> l_receiptdate(args.size());
// for (idx_t tempvar4 = 0; tempvar4 < args.size(); tempvar4++) {
//   // if (vdata.validity.GetValidityEntry(tempvar4)) {
//     l_receiptdate[tempvar4] = args.data[3].GetValue(tempvar4);
//   // }
// }
/* GLOBAL FUNCTIONS */
void checkDate(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result) {
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
//   args.data[0].ToUnifiedFormat(args.size(), vdata);
// std::vector<duckdb::Value> l_orderkey(args.size());
// for (idx_t tempvar1 = 0; tempvar1 < args.size(); tempvar1++) {
//   // if (vdata.validity.GetValidityEntry(tempvar1)) {
//     l_orderkey[tempvar1] = args.data[0].GetValue(tempvar1);
//   // }
// }
// args.data[1].ToUnifiedFormat(args.size(), vdata);
// std::vector<duckdb::Value> o_orderkey(args.size());
// for (idx_t tempvar2 = 0; tempvar2 < args.size(); tempvar2++) {
//   // if (vdata.validity.GetValidityEntry(tempvar2)) {
//     o_orderkey[tempvar2] = args.data[1].GetValue(tempvar2);
//   // }
// }

  /** INITIALIZE VARIABLES **/
  
  /** FUNCTION BODY **/
  /** RETURN **/
auto result_data = duckdb::FlatVector::GetData<int32_t>(result);
duckdb::Value const1 = duckdb::Value::DATE(1993,7,1);
duckdb::Value const2 = duckdb::Value::DATE(1993,10,1);
// std::vector<duckdb::Value> tempvar6 = const_vector_gen(1);
for(size_t tempvar5 = 0 ; tempvar5 < args.size() ; tempvar5++) {
  if(active2[tempvar5] && !returns1[tempvar5]) {
    // note that this lane has returned
    // returns1[tempvar5] = true;
    // set its return value
    if(greaterorequalthan(args.data[0].GetValue(tempvar5), const1) && smallerthan(args.data[0].GetValue(tempvar5), const2))
      result_data[tempvar5] = 1;
    else
      result_data[tempvar5] = 0;
  }
}
  // /** SET RETURN VALUE **/
  // for (idx_t i = 0; i < args.size(); i++) {
  //   result.SetValue(i, return_values[i]);
  // }
}


