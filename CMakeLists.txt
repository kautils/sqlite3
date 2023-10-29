cmake_minimum_required(VERSION 3.20)
set(__version 2.0.1.0)
project(KautilSqlite3.${__version}
        LANGUAGES C CXX
        VERSION ${__version}
        )
set(${PROJECT_NAME}.version 2.0.1.0)
unset(__version)

include(${CMAKE_CURRENT_LIST_DIR}/extern.cmake)
unset(${PROJECT_NAME}.version)


file(MAKE_DIRECTORY sqlite3/include/kautil)
file(CREATE_LINK "${CMAKE_CURRENT_LIST_DIR}/sqlite3" "${CMAKE_CURRENT_LIST_DIR}/sqlite3/include/kautil/sqlite3" SYMBOLIC)
 