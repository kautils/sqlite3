### outline
* mingw on windows10 
* two internal shared libarary refere the same libsqlit3.a independently.
* then make two library have the same definition of a class function
* cause access violation (0xc0000005)
### detail
* link libsqlite.a to kautil_sqlite_stmt_static
* link libsqlite.a to kautil_sqlite3_shared
* then define int kautil::database::Sqlite3Stmt::step(bool reset ) const noexcept in each library;
* will get 0xc0000005 (access violation) when run executable generated linking "kautil_sqlite_stmt_static" and "kautil_sqlite3_shared"   
### note 
* this was resolved by linking libsqlite-0.dll to kautil_sqlite3_shared instead of libsqlite.a    

### ref
*[https://github.com/tensorflow/tensorboard/issues/1467](https://github.com/tensorflow/tensorboard/issues/1467)

