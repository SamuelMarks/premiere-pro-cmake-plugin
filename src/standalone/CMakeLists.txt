get_filename_component(EXEC_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "_" EXEC_NAME ${EXEC_NAME})

#################
# Create target #
#################

# find_package(google_cloud_cpp_common CONFIG REQUIRED)

add_executable("${EXEC_NAME}" "main.c")
#target_include_directories(
#        "${PROJECT_NAME}"
#        PRIVATE
#)
target_link_libraries("${EXEC_NAME}" LINK_PUBLIC "common" "versions")

#if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
#    list(APPEND _LIBRARIES Shlwapi)
#endif ()

# target_link_libraries("${PROJECT_NAME}" PRIVATE "${_LIBRARIES}")

if (MSVC)
    set_target_properties("${PROJ_NAME}" PROPERTIES LINK_FLAGS /NODEFAULTLIB:\"MSVCRT\")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_library(CORE_FOUNDATION_LIBRARY CoreFoundation ONLY)
    find_library(APPLICATION_SERVICES_LIBRARY ApplicationServices ONLY)
    target_link_libraries("${PROJ_NAME}" PUBLIC "${CORE_FOUNDATION_LIBRARY}" "${APPLICATION_SERVICES_LIBRARY}")
endif ()
