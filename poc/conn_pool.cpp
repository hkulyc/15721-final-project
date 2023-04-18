#include "conn_pool.hpp"

Connection_Pool::Connection_Pool(DuckDB *db, int num_thread){
    size = num_thread;
    for(int i=0;i<num_thread;i++){
        conn_pool.push_back(Connection(*db));
        auto lock = std::make_unique<std::mutex>();
        lock->unlock();
        locks.push_back(std::move(lock));
        state_pool.push_back(std::vector<std::unique_ptr<PreparedStatement>>());
    }
}

void Connection_Pool::add_prepared(const std::string &sql){
    for(int i=0;i<size;i++){
        state_pool.at(i).push_back(conn_pool.at(i).Prepare(sql));
    }
}

PreparedStatement *Connection_Pool::get_prepared(int id, int *ret_id){
    for(int i=0;i<size;i++){
        if(locks.at(i)->try_lock()){
            *ret_id = i;
            return state_pool.at(i).at(id).get();
        }
    }
    return NULL;
}

void Connection_Pool::release_prepared(int id){
    locks.at(id)->unlock();
}