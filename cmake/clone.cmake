 
if(NOT DEFINED KAUTIL_THIRD_PARTY_DIR)
    set(KAUTIL_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
endif()

if(NOT DEFINED KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG)
    set(KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG v0.0.1)
endif()

if(NOT DEFINED KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY)
    set(KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY "D:/arrange/origin/c11_string_allocator")
endif()

set(target_repo ${KAUTIL_THIRD_PARTY_DIR}/c11_string_allocator/${KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY_TAG})

if(NOT EXISTS "${target_repo}/.git")
    execute_process(
        COMMAND git clone ${KAUTIL_C11_STRING_ALLOCATOR_REPOSITORY} ${target_repo} 
        RESULT_VARIABLE exit_code 
        # OUTPUT_FILE nul ERROR_FILE nul # want to discard message 
    )
    if(${exit_code} EQUAL 128)
        message(FATAL_ERROR "fail to clone c11_string_allocator")
    endif()
endif()

add_subdirectory(${target_repo})