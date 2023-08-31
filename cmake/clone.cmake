

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

unset(__module)
set(__module c11_string_allocator)

if(NOT DEFINED KAUTIL_THREAD_MAX)
    set(KAUTIL_THREAD_MAX 1)
endif()

if(NOT DEFINED KAUTIL_THIRD_PARTY_DIR)
    set(KAUTIL_THIRD_PARTY_DIR ${CMAKE_CURRENT_BINARY_DIR}/third_party)
endif()

if(NOT DEFINED KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG)
    set(KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG v0.0.1)
endif()

if(NOT DEFINED KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY)
    set(KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY https://github.com/kautils/c11_string_allocator.git)
endif()


set(target_module KautilC11StringAllocator.0.0.1.static)
set(target_repo ${KAUTIL_THIRD_PARTY_DIR}/${target_module})
set(${target_module}_DIR ${target_repo}/lib/cmake/${target_module})
if(NOT EXISTS "${target_repo}/repository/.git")
    file(MAKE_DIRECTORY ${target_repo}/repository)
    execute_git_command(DIR ${target_repo}/repository COMMANDS git init)
    execute_git_command(DIR ${target_repo}/repository COMMANDS git remote add origin ${KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY})
    execute_git_command(DIR ${target_repo}/repository COMMANDS git fetch origin --tags ${KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG} --depth=1)
    execute_git_command(DIR ${target_repo}/repository COMMANDS git checkout tags/${KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG})
    file(MAKE_DIRECTORY ${target_repo}/build)
    execute_git_command(DIR ${target_repo}/repository COMMANDS cmake -S . -B ${target_repo}/build)
    execute_git_command(DIR ${target_repo}/repository COMMANDS cmake --build ${target_repo}/build -j ${KAUTIL_THREAD_MAX})
    execute_git_command(DIR ${target_repo}/repository COMMANDS cmake --install ${target_repo}/build --prefix ${target_repo})
endif()

find_package(${target_module} REQUIRED)
unset(target_repo)
