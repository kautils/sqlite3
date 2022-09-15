
cmake_minimum_required(VERSION 3.20)
project(Kautil
        LANGUAGES C CXX
        VERSION 0.0.0.0
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)



if(DEFINED CMAKE_DST)
    set(__cmake_dst ${CMAKE_DST}/kautil_sqlite3)
else()
    set(__cmake_dst ${CMAKE_CURRENT_SOURCE_DIR}/cmake/kautil_sqlite3)
endif()

include(FetchContent)
if(NOT DEFINED kautil_cmake_POPULATED)
    FetchContent_Declare(kautil_cmake
            GIT_REPOSITORY /mnt/nvme0n1/git.local/current/c-and-cc/kautil_cmake/
            GIT_TAG        master
            GIT_SHALLOW True
            UPDATE_COMMAND ""
            SOURCE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/cmake/kautil_cmake
            )
    FetchContent_MakeAvailable(kautil_cmake)
    list(APPEND fetch_content kautil_cmake)
endif()

include(${kautil_cmake_SOURCE_DIR}/functions/functions.cmake)
set(KAUTIL_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/projects)
KautilInclude(PROJECT_HEADER)


set(tag_sqlite 3.39.2)
if(NOT DEFINED sqlite_sqlite_${tag_sqlite}_POPULATED)
    FetchContent_Declare(sqlite_sqlite_${tag_sqlite}
        GIT_REPOSITORY ${__kautil_git_url_prefix_c_and_cc}/sqlite
        GIT_TAG ${tag_sqlite}
        GIT_SHALLOW True
        UPDATE_COMMAND ""
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${__kautil_fetche_content_source_dir}/sqlite/${tag_sqlite}
        )
    FetchContent_MakeAvailable(sqlite_sqlite_${tag_sqlite})
endif()


if(MINGW)
else()
    if( EXISTS ${sqlite_sqlite_${tag_sqlite}_SOURCE_DIR}/install/lib/libsqlite3.a OR
        EXISTS ${sqlite_sqlite_${tag_sqlite}_SOURCE_DIR}/install/lib/libsqlite3.so 
            )
        set(__depends "")
    else()
        set(__depends sqlite_sqlite_${tag_sqlite}_build)
    endif()
endif()


file(GLOB __sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
KautilExportableLib(
    COMPONENT sqlite3
    NAMESPACE kautil
    TARGET sqlite3
    OUTPUT_NAME sqlite3
    TARGET_SOURCES ${__sources}
    TARGET_LINK_LIBRARIES sqlite_sqlite_shared_${tag_sqlite}
    INCLUDE_BUILD_INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
    INSTALL_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
    INCLUDE_DST include/kautil/sqlite3
    CMAKE_DST ${__cmake_dst}
    DEPENDS "${__depends}"
)

return()





