

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
    set(KAUTIL_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
endif()

if(NOT DEFINED KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG)
    set(KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG v0.0.1)
endif()

if(NOT DEFINED KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY)
    set(KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY https://github.com/kautils/c11_string_allocator.git)
endif()


set(target_module KautilC11StringAllocator.0.0.1)
set(target_repo ${KAUTIL_THIRD_PARTY_DIR}/c11_string_allocator/${KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG})
set(target_module_install_dir ${CMAKE_CURRENT_BINARY_DIR}/${__module})
set(${target_module}_DIR ${target_module_install_dir}/install/lib/cmake/${target_module})
if(NOT EXISTS "${target_repo}/.git")
    file(MAKE_DIRECTORY ${target_repo})
    execute_git_command(DIR ${target_repo} COMMANDS git init)
    execute_git_command(DIR ${target_repo} COMMANDS git remote add origin ${KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY})
    execute_git_command(DIR ${target_repo} COMMANDS git fetch origin --tags ${KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG} --depth=1)
    execute_git_command(DIR ${target_repo} COMMANDS git checkout tags/${KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG})
    file(MAKE_DIRECTORY ${target_module_install_dir}/install)
    file(MAKE_DIRECTORY ${target_module_install_dir}/build)
    execute_git_command(DIR ${target_repo} COMMANDS cmake -S . -B ${CMAKE_CURRENT_BINARY_DIR}/${__module}/build)
    execute_git_command(DIR ${target_repo} COMMANDS cmake --build ${CMAKE_CURRENT_BINARY_DIR}/${__module}/build -j ${KAUTIL_THREAD_MAX})
    execute_git_command(DIR ${target_repo} COMMANDS cmake --install ${CMAKE_CURRENT_BINARY_DIR}/${__module} --prefix ${CMAKE_CURRENT_BINARY_DIR}/${__module}/install)
endif()
find_package(${target_module} REQUIRED)
unset(target_repo)
