
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
    set(__cmake_dst ${CMAKE_CURRENT_BINARY_DIR}/cmake/kautil_sqlite3)
endif()


include(FetchContent)
if (NOT DEFINED kautil_cmake_POPULATED)

    set(path_to_cmake_remote /mnt/nvme0n1/git.local/current/c-and-cc/kautil_cmake )
    if(DEFINED KAUTIL_SOURCE_DIR)
        set(path_to_cmake_local ${KAUTIL_SOURCE_DIR}/third_party/projects/kautil_cmake/master)
    else()
        set(path_to_cmake_local ${CMAKE_CURRENT_SOURCE_DIR}/third_party/projects/kautil_cmake/master)
    endif()


    if(EXISTS ${path_to_cmake_local})
        set(__dl_command "")
    else()
        set(__dl_command git clone ${path_to_cmake_remote} ${path_to_cmake_local})
    endif()

    FetchContent_Declare( kautil_cmake
            GIT_REPOSITORY "${path_to_cmake_remote}"
            GIT_TAG master
            GIT_SHALLOW True
            DOWNLOAD_COMMAND ${__dl_command}
            UPDATE_COMMAND ""
            SOURCE_DIR "${path_to_cmake_local}"
            )
    FetchContent_MakeAvailable(kautil_cmake)
endif ()
include(${kautil_cmake_SOURCE_DIR}/functions/functions.cmake)
set(KAUTIL_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/projects)
KautilInclude(PROJECT_HEADER)





unset(fetch_content_list)
set(tag_sqlite 3.39.2)
KautilFetchContentDeclare( fetch_content_list
        GIT_REPOSITORY ${__kautil_git_url_prefix_c_and_cc}/sqlite
        GIT_TAG ${tag_sqlite}
        GIT_SHALLOW True
        UPDATE_COMMAND ""
        SOURCE_DIR_PREFIX ${KAUTIL_SOURCE_DIR}/${__kautil_fetche_content_prefix}
        )
kautil_FetchContent_MakeAvailable("${fetch_content_list}")



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






