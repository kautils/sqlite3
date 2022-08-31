
cmake_minimum_required(VERSION 3.20)
project(Kautil
        LANGUAGES C CXX
        VERSION 0.0.0.0
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)



if(DEFINED CMAKE_DST)
    set(__cmake_dst ${CMAKE_DST})
else()
    set(__cmake_dst ${CMAKE_CURRENT_SOURCE_DIR})
endif()

include(FetchContent)
if(NOT DEFINED kautil_cmake_POPULATED)
    FetchContent_Declare(kautil_cmake
            GIT_REPOSITORY https://source.developers.google.com/p/repos-349514/r/kautil_cmake
            GIT_TAG        master
            GIT_SHALLOW True
            UPDATE_COMMAND ""
            SOURCE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/third_party/projects/kautil_cmake
            )
    FetchContent_MakeAvailable(kautil_cmake)
    list(APPEND fetch_content kautil_cmake)
endif()

include(${kautil_cmake_SOURCE_DIR}/functions/functions.cmake)
set(KAUTIL_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/projects)
KautilInclude(PROJECT_HEADER)



if(NOT DEFINED sqlite_sqlite_POPULATED)
    FetchContent_Declare(sqlite_sqlite
            GIT_REPOSITORY https://source.developers.google.com/p/c-and-cpp/r/sqlite
            GIT_TAG master
            GIT_SHALLOW True
            UPDATE_COMMAND ""
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/projects/sqlite
            )
    list(APPEND fetch_content sqlite_sqlite)
endif()

FetchContent_MakeAvailable(${fetch_content})


