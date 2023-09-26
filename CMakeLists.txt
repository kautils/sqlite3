cmake_minimum_required(VERSION 3.20)
set(__version 2.0.1.0)
project(KautilSqlite3.${__version}
        LANGUAGES C CXX
        VERSION ${__version}
        )
set(${PROJECT_NAME}.version 2.0.1.0)
unset(__version)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeKautilHeader.cmake)
    file(DOWNLOAD https://raw.githubusercontent.com/kautils/CMakeKautilHeader/v0.0.1/CMakeKautilHeader.cmake ${CMAKE_BINARY_DIR}/CMakeKautilHeader.cmake)
endif()
include(${CMAKE_BINARY_DIR}/CMakeKautilHeader.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/extern.cmake)
unset(${PROJECT_NAME}.version)

