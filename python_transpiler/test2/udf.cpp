/* GLOBAL MACROS */
#include <stdexcept>
#include <vector>
#include <iostream>
#include "udf.hpp"

duckdb::DuckDB db(nullptr);
thread_local duckdb::Connection trans_con(db);

std::vector<duckdb::Value> const_vector_gen(duckdb::Value val)
{
  // generates vector_size copies of val to store in the vector
  std::vector<duckdb::Value> cons;
  cons.reserve(2048);
  for (int i = 0; i < 2048; i++)
  {
    cons.emplace_back(val);
  }
  return cons;
}

template <typename T>
std::vector<duckdb::Value> const_vector_gen_decimal(T raw_int, int8_t width, int8_t scale)
{
  // generates vector_size copies of val to store in the vector
  std::vector<duckdb::Value> cons;
  cons.reserve(2048);
  for (int i = 0; i < 2048; i++)
  {
    cons.emplace_back(duckdb::Value::DECIMAL(raw_int, width, scale));
  }
  return cons;
}
/* GLOBAL VARIABLES */
/* GLOBAL FUNCTIONS */
void if_func(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result)
{
  duckdb::UnifiedVectorFormat vdata;
  std::vector<bool> returns1(args.size());
  std::vector<duckdb::Value> return_values(args.size());
  for (size_t i = 0; i < args.size(); i++)
  {
    returns1[i] = false;
  }
  std::vector<bool> active2(args.size());
  for (size_t i = 0; i < args.size(); i++)
  {
    active2[i] = true;
  }
  /** FUNCTION ARGS **/
  args.data[0].ToUnifiedFormat(args.size(), vdata);
  std::vector<duckdb::Value> p_partkey(args.size());
  for (idx_t tempvar1 = 0; tempvar1 < args.size(); tempvar1++)
  {
    if (vdata.validity.GetValidityEntry(tempvar1))
    {
      p_partkey[tempvar1] = args.data[0].GetValue(tempvar1);
    }
  }
  args.data[1].ToUnifiedFormat(args.size(), vdata);
  std::vector<duckdb::Value> ps_partkey(args.size());
  for (idx_t tempvar2 = 0; tempvar2 < args.size(); tempvar2++)
  {
    if (vdata.validity.GetValidityEntry(tempvar2))
    {
      ps_partkey[tempvar2] = args.data[1].GetValue(tempvar2);
    }
  }
  args.data[2].ToUnifiedFormat(args.size(), vdata);
  std::vector<duckdb::Value> s_suppkey(args.size());
  for (idx_t tempvar3 = 0; tempvar3 < args.size(); tempvar3++)
  {
    if (vdata.validity.GetValidityEntry(tempvar3))
    {
      s_suppkey[tempvar3] = args.data[2].GetValue(tempvar3);
    }
  }
  args.data[3].ToUnifiedFormat(args.size(), vdata);
  std::vector<duckdb::Value> ps_suppkey(args.size());
  for (idx_t tempvar4 = 0; tempvar4 < args.size(); tempvar4++)
  {
    if (vdata.validity.GetValidityEntry(tempvar4))
    {
      ps_suppkey[tempvar4] = args.data[3].GetValue(tempvar4);
    }
  }
  args.data[4].ToUnifiedFormat(args.size(), vdata);
  std::vector<duckdb::Value> s_nationkey(args.size());
  for (idx_t tempvar5 = 0; tempvar5 < args.size(); tempvar5++)
  {
    if (vdata.validity.GetValidityEntry(tempvar5))
    {
      s_nationkey[tempvar5] = args.data[4].GetValue(tempvar5);
    }
  }
  args.data[5].ToUnifiedFormat(args.size(), vdata);
  std::vector<duckdb::Value> n_nationkey(args.size());
  for (idx_t tempvar6 = 0; tempvar6 < args.size(); tempvar6++)
  {
    if (vdata.validity.GetValidityEntry(tempvar6))
    {
      n_nationkey[tempvar6] = args.data[5].GetValue(tempvar6);
    }
  }
  args.data[6].ToUnifiedFormat(args.size(), vdata);
  std::vector<duckdb::Value> n_regionkey(args.size());
  for (idx_t tempvar7 = 0; tempvar7 < args.size(); tempvar7++)
  {
    if (vdata.validity.GetValidityEntry(tempvar7))
    {
      n_regionkey[tempvar7] = args.data[6].GetValue(tempvar7);
    }
  }
  args.data[7].ToUnifiedFormat(args.size(), vdata);
  std::vector<duckdb::Value> r_regionkey(args.size());
  for (idx_t tempvar8 = 0; tempvar8 < args.size(); tempvar8++)
  {
    if (vdata.validity.GetValidityEntry(tempvar8))
    {
      r_regionkey[tempvar8] = args.data[7].GetValue(tempvar8);
    }
  }
  args.data[8].ToUnifiedFormat(args.size(), vdata);
  std::vector<duckdb::Value> r_name(args.size());
  for (idx_t tempvar9 = 0; tempvar9 < args.size(); tempvar9++)
  {
    if (vdata.validity.GetValidityEntry(tempvar9))
    {
      r_name[tempvar9] = args.data[8].GetValue(tempvar9);
    }
  }

  /** INITIALIZE VARIABLES **/

  /** FUNCTION BODY **/
  /** RETURN **/
  auto result_data = duckdb::FlatVector::GetData<int32_t>(result);
  duckdb::Value const1("EUROPE");
  std::vector<duckdb::Value> tempvar11 = const_vector_gen(1);
  for (size_t i = 0; i < args.size(); i++)
  {
    if (active2[i] && !returns1[i])
    {
      // // note that this lane has returned
      // returns1[tempvar10] = true;
      // // set its return value
      // return_values[tempvar10] = tempvar11[tempvar10];
      if (p_partkey[i] == ps_partkey[i] &&
        s_suppkey[i] == ps_suppkey[i] &&
        s_nationkey[i] == n_nationkey[i] &&
        n_regionkey[i] == r_regionkey[i] &&
        r_name[i] == const1)
      // result.SetValue(i, return_values[i]);
        result_data[i] = 1;
      else
        result_data[i] = 0;
    }
  }
  /** SET RETURN VALUE **/

  for (idx_t i = 0; i < args.size(); i++)
  {
    // the condition
    
  }
}
