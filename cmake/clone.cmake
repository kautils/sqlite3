 
if(NOT DEFINED KAUTIL_THIRD_PARTY_DIR)
    set(KAUTIL_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
endif()

if(NOT DEFINED KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG)
    set(KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG v0.0.1)
endif()

if(NOT DEFINED KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY)
    set(KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY https://github.com/kautils/c11_string_allocator.git)
endif()

set(target_repo ${KAUTIL_THIRD_PARTY_DIR}/c11_string_allocator/${KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG})


macro(execute_git_command)
    cmake_parse_arguments( EXEC_GIT "" "DIR" "COMMANDS" ${ARGN})
    execute_process(
        COMMAND ${EXEC_GIT_COMMANDS} 
        WORKING_DIRECTORY "${EXEC_GIT_DIR}" 
        RESULT_VARIABLE exit_code
        OUTPUT_FILE nul ERROR_FILE nul # want to discard message 
    )
    if(NOT ${exit_code} EQUAL 0)
        message(FATAL_ERROR "fail to clone c11_string_allocator")
    endif()
    unset(exit_code)
endmacro()

if(NOT EXISTS "${target_repo}/.git")
    file(MAKE_DIRECTORY ${target_repo})
    execute_git_command(DIR ${target_repo} COMMANDS git init)
    execute_git_command(DIR ${target_repo} COMMANDS git remote add origin ${KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY})
    execute_git_command(DIR ${target_repo} COMMANDS git fetch origin --tags ${KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG} --depth=1)
    execute_git_command(DIR ${target_repo} COMMANDS git checkout tags/${KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG})
endif()
add_subdirectory(${target_repo})
unset(target_repo)
