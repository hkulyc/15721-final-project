#include "duckdb.hpp"
#include <vector>
#include <mutex>
using namespace duckdb;

class Connection_Pool{

public:
    Connection_Pool(DuckDB *db, int num_threads);
    // ~Connection_Pool()=0;
    Connection *get_conn();
    PreparedStatement *get_prepared(int id, int *ret_id);
    // add prepared statements to all the connections
    void add_prepared(const std::string &sql);
    void release_prepared(int id);
private: 
    int size;
    std::vector<Connection> conn_pool;
    std::vector<std::vector<std::unique_ptr<PreparedStatement>>> state_pool;
    std::vector<std::unique_ptr<std::mutex>> locks;
};