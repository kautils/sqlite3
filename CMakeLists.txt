cmake_minimum_required(VERSION 3.20)
project(KautilSqlite3
        LANGUAGES C CXX
        VERSION 2.0.1.0
        )
set(CMAKE_CXX_STANDARD 17)

macro(CMakeFetchKautilModule prfx)
    
    cmake_parse_arguments(${prfx} "VERBOSE;FORCE_UPDATE" "GIT;REMOTE;TAG" "" ${ARGV})
    list(APPEND unsetter_prfx ${prfx}_FORCE_UPDATE ${prfx}_GIT ${prfx}_REMOTE ${prfx}_TAG)
    
    list(APPEND unsetter __force_option __verbose_option __uri __remote __tag)
    set(__uri ${${prfx}_GIT})
    set(__remote ${${prfx}_REMOTE})
    set(__tag ${${prfx}_TAG})
    if(${${prfx}_FORCE_UPDATE}) 
        string(APPEND __force_option FORCE_UPDATE)
    endif()
    if(${${prfx}_VERBOSE})
        string(APPEND __verbose_option VERBOSE)
        include(CmakePrintHelpers)
        foreach(__var ${unsetter})
            cmake_print_variables(${__var})
        endforeach()
    endif()
    
    
    if(NOT DEFINED KAUTIL_THIRD_PARTY_DIR)
        set(KAUTIL_THIRD_PARTY_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party)
        file(MAKE_DIRECTORY "${KAUTIL_THIRD_PARTY_DIR}")
    endif()
    
    if(NOT DEFINED KAUTIL_THREAD_MAX)
        set(KAUTIL_THREAD_MAX 1)
    endif()
        
    if(NOT EXISTS ${KAUTIL_THIRD_PARTY_DIR}/cmake/CMakeGitCloneMinimal.cmake)
        file(DOWNLOAD https://raw.githubusercontent.com/kautils/CMakeGitCloneMinimal/v0.0.1/CMakeGitCloneMinimal.cmake "${KAUTIL_THIRD_PARTY_DIR}/cmake/CMakeGitCloneMinimal.cmake")
    endif()
    
    include("${KAUTIL_THIRD_PARTY_DIR}/cmake/CMakeGitCloneMinimal.cmake")
    
    CMakeGitCloneMinimal( ${prfx} 
            REPOSITORY_NAME ${prfx} 
            REPOSITORY_REMOTE ${__remote}  
            REPOSITORY_URI ${__uri}
            DESTINATION "${KAUTIL_THIRD_PARTY_DIR}/repository"
            TAG ${__tag}
            ${__force_option}
            ${__verbose_option}
            )
    
    if((NOT ${__force_option} STREQUAL "") OR (NOT DEFINED CACHE{${prfx}_build_is_done}))
        file(MAKE_DIRECTORY ${${prfx}}/build)
        CMakeExecuteCommand(execgit DIR ${${prfx}} COMMAND cmake -S . -B ${${prfx}}/build -DKAUTIL_THIRD_PARTY_DIR='${KAUTIL_THIRD_PARTY_DIR}' ASSERT)
        CMakeExecuteCommand(execgit DIR ${${prfx}} COMMAND cmake --build ${${prfx}}/build -j ${KAUTIL_THREAD_MAX} ASSERT)
        CMakeExecuteCommand(execgit DIR ${${prfx}} COMMAND cmake --install ${${prfx}}/build --prefix ${KAUTIL_THIRD_PARTY_DIR} ASSERT)
        set(${prfx}_build_is_done TRUE CACHE STRING "")
        
        # update module path
        file(GLOB mod_paths ${KAUTIL_THIRD_PARTY_DIR}/lib/cmake/*)
        list(APPEND CMAKE_PREFIX_PATH ${mod_paths})
        list(REMOVE_DUPLICATES  mod_paths)
    endif()

    foreach(__var ${unsetter})
        unset(${__var})
    endforeach()
    unset(unsetter)
    foreach(__var ${unsetter_prfx})
        unset(${__var})
    endforeach()
    unset(unsetter_prfx)

endmacro()

CMakeFetchKautilModule(c11_string_allocator 
        GIT https://github.com/kautils/c11_string_allocator.git 
        REMOTE origin 
        TAG v0.0.1
        #FORCE_UPDATE
        #VERBOSE
        )


find_package(KautilC11StringAllocator.0.0.1.static REQUIRED)
include(extern.cmake)




