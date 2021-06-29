get_filename_component(LIBRARY_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "_" LIBRARY_NAME "${LIBRARY_NAME}")
string(TOUPPER "${LIBRARY_NAME}" LIBRARY_UPPER_NAME)

######################
# Library definition #
######################

add_library(
        "${LIBRARY_NAME}"
        "${LIBRARY_TYPE_FLAG}"
        "logger.cpp"
)

set(_include "${CMAKE_CURRENT_SOURCE_DIR}")

generate_export_header("${LIBRARY_NAME}" BASE_NAME "lib${LIBRARY_NAME}_and_static")
set_target_properties("${LIBRARY_NAME}" PROPERTIES
        COMPILE_FLAGS "-DLIB${LIBRARY_UPPER_NAME}SHARED_AND_STATIC_STATIC_DEFINE")
list(APPEND _include "${CMAKE_CURRENT_BINARY_DIR}")

if (DEFINED PLUGIN_MODE)
    list(APPEND _include "${ADOBE_PP_SDK}/Examples/Headers")
endif ()

target_include_directories (
        "${LIBRARY_NAME}"
        PUBLIC
        "${_include}"
)

set_target_properties(
        "${LIBRARY_NAME}"
        PROPERTIES
        LINKER_LANGUAGE
        CXX
)

#####################################################################
# Set flags to overcome C++ exception handling issues for C interop #
#####################################################################

if (MSVC)
    set_target_properties("${LIBRARY_NAME}" PROPERTIES COMPILE_FLAGS "/EHsc")
endif ()
