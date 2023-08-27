


#include "kautil/sqlite3/sqlite3.h"

int main(){

    auto sql = kautil::database::Sqlite3{":memory:"};
    sql.SendQuery("create table if not exists kautil_alter_demo([id] integer primary key)");
    auto alter = kautil::database::Sqlite3AlterTable{sql.database(),"kautil_alter_demo"};
    alter.AddColumn("abc","blob");
    alter.AddColumn("pqr","blob");
    alter.RenameColumns(kautil::database::sqlite3_alter_col_arg("abc", "efg"));
    alter.RenameColumns(kautil::database::sqlite3_alter_col_arg("efg", "hij")("can_rename_multiple_at_once","and_drop_this"));
    alter.DeleteColumn("and_drop_this");
    return 0;

}