cmake_minimum_required(VERSION 3.20)
project(KautilSqlite3
        LANGUAGES C CXX
        VERSION 0.0.0.0
        )
set(CMAKE_CXX_STANDARD 17)

if(NOT DEFINED KAUTIL_LIBSQLITE3_LIBDIR)
    set(KAUTIL_LIBSQLITE3_LIBDIR ${CMAKE_CURRENT_LIST_DIR}/third_party/mingw-w64-x86_64-sqlite3_3.42.0/bin)
endif()

if(NOT DEFINED KAUTIL_LIBSQLITE3_INCLUDE_DIR)
    set(KAUTIL_LIBSQLITE3_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/third_party/mingw-w64-x86_64-sqlite3_3.42.0/include")
endif()

if(NOT DEFINED KAUTIL_LIBSQLITE3_LIBNAME)
    set(KAUTIL_LIBSQLITE3_LIBNAME sqlite3)
endif()

include(sqlite3/sqlite3.cmake)
include(extern.cmake)

