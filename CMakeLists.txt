cmake_minimum_required(VERSION 3.20)
project(KautilSqlite3
        LANGUAGES C CXX
        VERSION 2.0.1.0
        )
set(CMAKE_CXX_STANDARD 17)
include(cmake/clone.cmake)
include(sqlite3/sqlite3.cmake)
include(extern.cmake)




