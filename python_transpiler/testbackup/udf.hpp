#include "duckdb.hpp"
void discount_price(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);
void discount_taxprice(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);
void profit_amount(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);
void isShippedBefore(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);
void checkDate(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);
void q3conditions(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);
void q5Conditions(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);
void q6conditions(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);
void q7conditions(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);
void q10conditions(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);
void line_count(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);
void q12conditions(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);
void promo_disc(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);
void q19conditions(duckdb::DataChunk &args, duckdb::ExpressionState &state, duckdb::Vector &result);