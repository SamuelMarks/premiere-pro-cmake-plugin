get_filename_component(LIBRARY_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "_" LIBRARY_NAME "${LIBRARY_NAME}")
string(TOUPPER "${LIBRARY_NAME}" LIBRARY_UPPER_NAME)

######################
# Library definition #
######################

# header only
add_library(
        "${LIBRARY_NAME}"
        "${LIBRARY_TYPE_FLAG}"
        "versions.c"
        #"logger.cpp"
        #INTERFACE
)

set(_include "${CMAKE_CURRENT_SOURCE_DIR}")
set(_libs "")

generate_export_header("${LIBRARY_NAME}" BASE_NAME "lib${LIBRARY_NAME}_and_static")
set_target_properties("${LIBRARY_NAME}" PROPERTIES
        COMPILE_FLAGS "-DLIB${LIBRARY_UPPER_NAME}SHARED_AND_STATIC_STATIC_DEFINE")

list(APPEND _include "${CMAKE_CURRENT_BINARY_DIR}")

find_package(ZLIB REQUIRED)
list(APPEND _libs "ZLIB::ZLIB")

find_package(OpenSSL REQUIRED)
list(APPEND _libs "OpenSSL::SSL")
list(APPEND _libs "OpenSSL::Crypto")

target_include_directories (
        "${LIBRARY_NAME}"
        PUBLIC
        "${_include}"
)

target_link_libraries(
        "${LIBRARY_NAME}"
        PUBLIC
        "${_libs}"
)

set_target_properties(
        "${LIBRARY_NAME}"
        PROPERTIES
        LINKER_LANGUAGE
        C
)

#####################################################################
# Set flags to overcome C++ exception handling issues for C interop #
#####################################################################

if (MSVC)
    set_target_properties("${LIBRARY_NAME}" PROPERTIES COMPILE_FLAGS "/EHsc")
endif ()
