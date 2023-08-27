

#include "alter.h"
#include "sqlite3.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>



namespace kautil{
namespace database {
namespace sqlite3{


struct AlterInternal{
    AlterInternal(Alter * self, ::sqlite3* db, const char * table) :  db_(db), table_(table){}
    bool add_column(const char  name[], const char definition[]);
    int step(const char  q[]);
    bool delete_column(const char  name[]);
    bool rename_column();
    bool update_table_info();

    ::sqlite3 * db_;
    std::string table_;
    std::unordered_map<std::string,int32_t> col_;
    std::vector<std::pair<std::string_view,std::string_view>> from_to; 
    
};




Alter::~Alter(){ if(m_) delete m_; }

Alter::Alter(::sqlite3 * db, const char table[]) : m_(new AlterInternal(this, (::sqlite3*)db, table)) {
    m_->update_table_info();
}
bool Alter::update_table_info(){ return m_->update_table_info(); }
bool Alter::add_column(const char  name[], const char definition[]){ return m_->add_column(name, definition); }
bool Alter::delete_column(const char  name[]){ return m_->delete_column(name); }
void Alter::rename_columns_push(const char *from, const char *to) { m_->from_to.push_back({from,to}); }
bool Alter::rename_columns() { return m_->rename_column(/*m_->from_to*/); }
void Alter::release(){ delete this; }



bool AlterInternal::add_column(const char  name[], const char definition[]){
    if(!col_.count(name)){
        auto const& q = std::string{"alter table "} + table_ + " add " + name + " " + definition;
        auto const& res = step(q.data());
        if(res == SQLITE_DONE ){
            col_.insert({name,col_.size()});
            return true;
        }
        return false;
    }
    return true ;
}


int AlterInternal::step(const char  q[]){
    auto stmt = (sqlite3_stmt * )0;
    auto res = sqlite3_prepare_v2(db_, q, -1, &stmt, nullptr);
    if(res != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return res;
    }
    res = sqlite3_step(stmt);
    if(stmt) sqlite3_finalize(stmt);
    return res;
};


bool AlterInternal::delete_column(const char  name[]){
    if(col_.count(name)){
        auto const& q = std::string{"alter table "} + table_ + " drop " + name;
        auto const& res = step(q.data());
        if(res == SQLITE_DONE ){
            col_.erase(col_.find(name));
            return true;
        }
        return false;
    }
    return true;
}



struct alter_exception : std::exception{
    std::string msg;
    alter_exception(int size , const char * fmt ,...){
        msg.resize(size);
        va_list l;
        va_start(l,fmt);
        __mingw_vsprintf(msg.data(),fmt,l);
        va_end(l);
    }
    const char * what() const noexcept override{ return msg.data(); } 
};
bool AlterInternal::rename_column(/*std::vector<std::pair<std::string_view,std::string_view>> const& from_to*/){
    for(auto & p : from_to){
        auto find_f = false;
        auto find_t = false;
        for(auto & c : col_){
             if(c.first == p.first){
                 find_f = true;
             }
        }
        if(!find_f)continue;
        for(auto & c : col_){
             if(c.first == p.second){
                 throw alter_exception{1024,"specified col \"%s\" is already exists[%s(%d)]",c.first.data(),__FILE__,__LINE__};
             }
        }
    }

    static auto pre_resurved = "rename_column_kautil_alter_buf";
    auto create_new_tb_base = std::string{};
    {
        auto createtb = std::string{"select sql from"} + " sqlite_master where name='"+table_+"' limit 1";
        auto stmt = (sqlite3_stmt*) 0;
        auto res = int(0);
        if((res = sqlite3_prepare_v2(db_,createtb.data(),-1,&stmt,nullptr)) == SQLITE_OK){
            if((res = sqlite3_step(stmt)) == SQLITE_ROW){
                create_new_tb_base = (char *) sqlite3_column_text(stmt,0);
                sqlite3_finalize(stmt);
            }else{
                sqlite3_finalize(stmt);
                return res;
            }
        }else{
            return res;
        }
    }

    auto buf_col = col_;
    auto find = false;
    for(auto & p : from_to){
        auto itr = col_.find(p.first.data());
        if(itr != col_.end()){
            if(!find)find = true;
            col_.insert({std::string{p.second},itr->second});
            col_.erase(itr);
            auto const& to = std::string{"["} + p.second.data() + "] ";
            for(auto from : {
                std::string{"["} + p.first.data() + "]"
                ,std::string{" "} + p.first.data() + " "
            }){
                auto at =  create_new_tb_base.find(from);
                if(at != std::string::npos){
                    create_new_tb_base.replace( at,from.size(),to);
                    break;
                }
            }
        }
    }
    
    
    from_to.clear();
    if(!find) return SQLITE_DONE;


    auto get_col_string = [](auto & res , auto & um){
        if(res.size()) res.resize(0);
        auto vec = std::vector<std::pair<std::string,int>>{um.begin(), um.end()};
        std::sort(vec.begin(), vec.end(), [](auto const& l, auto const& r){ return l.second < r.second; });
        for(auto & v : vec){
            res.append(v.first).push_back(',');
        };
        if(!res.empty()) res.resize(res.size() -1);
    };

    auto old_cols_str = std::string{};
    get_col_string(old_cols_str,buf_col);

    auto new_cols_str = std::string{};
    get_col_string(new_cols_str,col_);

    {
        auto const& q = std::string{"alter table "} + table_ + " rename to " + pre_resurved;
        auto res = step(q.data());
        if(res != SQLITE_DONE) { return false; }
    }

    {
        auto res = step(create_new_tb_base.data());
        if(res != SQLITE_DONE) { return false; }
    }

    { // insert
        auto stmt = (sqlite3_stmt * ) 0;
        auto const& q = std::string{"insert into "} + table_ + "(" + new_cols_str + ")" + " select "+old_cols_str+" from " + pre_resurved;
        auto res = step(q.data());
        if(res != SQLITE_DONE) return false;
    }

    {
        auto const& q = std::string{"drop table if exists "} + " " + pre_resurved;
        auto res = step(q.data());
        if(res != SQLITE_DONE) return false;
    }
    return true;
}

bool AlterInternal::update_table_info(){
    auto const& q =  std::string{"select * from "} + table_ + " limit 1";
    auto stmt = (sqlite3_stmt*) nullptr;
    auto res = 0;
    if ((res =  sqlite3_prepare_v2(db_,q.data(),-1,&stmt,nullptr)) == SQLITE_OK){
        if( (res = sqlite3_step(stmt)) >= SQLITE_ROW ){
            col_.clear();
            auto n= sqlite3_column_count(stmt);
            for(auto i =0 ; i < n ; ++i){
                col_.insert({sqlite3_column_name(stmt,i),i});
            }
        }
        sqlite3_finalize(stmt);
        return true;
    }
    return false;
}

Alter * sqlite3_alter(::sqlite3 * db, const char table[]){
    auto res = new Alter(db,table);
    return res;
}


}//namespace sqlite3{
} //namespace kautil{
} //namespace database {


int tmain_kautil_sqlite3_alter_static(sqlite3 * con){
//    while(true){
//        for(auto i = 0; i < 10000 ; ++i){
        auto alt = kautil::database::sqlite3::sqlite3_alter(con,"test");
        if(alt->add_column("[data8]","blob default x'0000000000000000'")){
        }else fprintf(stderr, "%s\n", "fail");
        
        try{
            alt->rename_columns_push("data5","data_123");
            alt->rename_columns();
        }catch(std::exception & e){
            fprintf(stderr,"%s\n",e.what());
        }
        alt->release();
//        }
//    }
    
    return 0;
}

