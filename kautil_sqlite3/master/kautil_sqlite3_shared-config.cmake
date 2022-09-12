
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was kautil_sqlite3-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../../" ABSOLUTE)

####################################################################################
if(kautil_sqlite3_FOUND)
	return()
endif()


if(KAUTIL_USE_STATC_LIBS)
	set(__target_name kautil_sqlite3-targets.cmake)
else()
	set(__target_name kautil_sqlite3_shared-targets.cmake)
endif()

if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/${__target_name}")
	include ("${CMAKE_CURRENT_LIST_DIR}/${__target_name}")
	set(kautil_sqlite3_FOUND True)
else()
	set(kautil_sqlite3_FOUND False)
endif()
