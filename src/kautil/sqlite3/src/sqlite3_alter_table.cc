

#include "kautil/sqlite3/sqlite3.h"
#include "sqlite3.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

namespace kautil{
namespace database {


struct Sqlite3AlterTableInternal{
    Sqlite3AlterTableInternal(Sqlite3AlterTable * self,sqlite3* db,const char * table) : self_(self),db_(db),table_(table){}
    int add_column(const char  name[], const char definition[]);
    int step(const char  q[]);
    int delete_column(const char  name[]);
    int rename_column(std::vector<std::pair<std::string_view,std::string_view>> const& from_to);
    int update_table();

    Sqlite3AlterTable * self_ = nullptr;
    sqlite3 * db_;
    std::string table_;
    std::unordered_map<std::string,int32_t> col_;
};


struct RenameElementInternal{ std::vector<std::pair<std::string_view,std::string_view>> from_to; };
RenameElement::~RenameElement(){ if(m_)delete m_; }
RenameElement::RenameElement() : m_(new RenameElementInternal){}
RenameElement::RenameElement(const char * from,const char * to) : m_(new RenameElementInternal){
    m_->from_to.push_back({from,to});
}
RenameElement RenameElement::make_pair(const char * from,const char * to){ return RenameElement(from,to); }
RenameElement& RenameElement::operator()(const char * from,const char * to){
    m_->from_to.push_back({from,to});
    return *this;
}


Sqlite3AlterTable::~Sqlite3AlterTable(){ if(m_) delete m_; }
Sqlite3AlterTable::Sqlite3AlterTable(void * db,const char table[]) : m_(new Sqlite3AlterTableInternal(this,(sqlite3*)db,table)) {
    m_->update_table();
}
int Sqlite3AlterTable::UpdateTable(){ return m_->update_table(); }
int Sqlite3AlterTable::AddColumn(const char  name[], const char definition[]){ return m_->add_column(name,definition); }
int Sqlite3AlterTable::DeleteColumn(const char  name[]){ return m_->delete_column(name); }
int Sqlite3AlterTable::RenameColumns(RenameElement const& pair){
    return m_->rename_column(pair.m_->from_to);
}


int Sqlite3AlterTableInternal::add_column(const char  name[], const char definition[]){
    if(!col_.count(name)){
        auto const& q = std::string{"alter table "} + table_ + " add " + name + " " + definition;
        auto const& res = step(q.data());
        if(res == SQLITE_DONE ) col_.insert({name,col_.size()});
        return res;
    }
    return SQLITE_DONE ;
}


int Sqlite3AlterTableInternal::step(const char  q[]){
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


int Sqlite3AlterTableInternal::delete_column(const char  name[]){
    if(col_.count(name)){
        auto const& q = std::string{"alter table "} + table_ + " drop " + name;
        auto const& res = step(q.data());
        if(res == SQLITE_DONE )col_.erase(col_.find(name));
        return res;
    }
    return SQLITE_DONE;
}



 struct colum_already_exists : std::exception{};
int Sqlite3AlterTableInternal::rename_column(std::vector<std::pair<std::string_view,std::string_view>> const& from_to){
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
                 throw colum_already_exists{};
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
        if(res != SQLITE_DONE) { return res; }
    }

    {
        auto res = step(create_new_tb_base.data());
        if(res != SQLITE_DONE) { return res; }
    }

    { // insert
        auto stmt = (sqlite3_stmt * ) 0;
        auto const& q = std::string{"insert into "} + table_ + "(" + new_cols_str + ")" + " select "+old_cols_str+" from " + pre_resurved;
        auto res = step(q.data());
        if(res != SQLITE_DONE) return res;
    }

    {
        auto const& q = std::string{"drop table if exists "} + " " + pre_resurved;
        auto res = step(q.data());
        if(res != SQLITE_DONE) return res;
    }
    return SQLITE_DONE;
}

int Sqlite3AlterTableInternal::update_table(){
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
    }
    return res;
}




} //namespace kautil{
} //namespace database {



int kautil_sqlite3_add_column(sqlite3 * db,const char * table,const char  name[], const char definition[]){
    auto const& q = std::string{"alter table "} + table + " add " + name + " " + definition;
    auto stmt = (sqlite3_stmt * )0;
    auto res = sqlite3_prepare_v2(db, q.data(), -1, &stmt, nullptr);
    if(res != SQLITE_OK){
        if(stmt) sqlite3_finalize(stmt);
        return res;
    }
    res = sqlite3_step(stmt);
    if(stmt) sqlite3_finalize(stmt);
    return res;
}

