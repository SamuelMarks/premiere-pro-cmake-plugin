get_filename_component(LIBRARY_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "_" LIBRARY_NAME "${LIBRARY_NAME}")

include(GenerateExportHeader)

set(Header_Files "logger.h")
if (PLUGIN_MODE)
  list(APPEND "${ADOBE_PP_SDK}/Examples/Headers/PrSDKErrorSuite.h")
  list(APPEND "${ADOBE_PP_SDK}/Examples/Headers/PrSDKTypes.h")
  set(_pp_headers "${ADOBE_PP_SDK}/Examples/Headers")
endif()

source_group("Header Files" FILES "${Header_Files}")

set(Source_Files "logger.cpp")
source_group("Source Files" FILES "${Source_Files}")

add_library("${LIBRARY_NAME}" "${LIBRARY_TYPE_FLAG}" "${Header_Files}" "${Source_Files}")

target_include_directories(
        "${LIBRARY_NAME}"
        INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:include>"
        #"${_pp_headers}"
)
if (PLUGIN_MODE)
  target_include_directories(
          "${LIBRARY_NAME}"
          PUBLIC
          "${_pp_headers}"
  )
endif()

target_link_libraries("${LIBRARY_NAME}" PUBLIC "${PROJECT_LOWER_NAME}_compiler_flags")

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  find_library(CORE_FOUNDATION_LIBRARY CoreFoundation ONLY)
  target_link_libraries("${LIBRARY_NAME}" PUBLIC "${CORE_FOUNDATION_LIBRARY}")
endif ()

set_target_properties(
        "${LIBRARY_NAME}"
        PROPERTIES
        LINKER_LANGUAGE
        CXX
)

set(_export_file "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}_export.h")
generate_export_header("${LIBRARY_NAME}" EXPORT_FILE_NAME "${_export_file}")

# setup the version numbering
set_property(TARGET "${LIBRARY_NAME}" PROPERTY VERSION "1.0.0")
set_property(TARGET "${LIBRARY_NAME}" PROPERTY SOVERSION "1")

# install rules
set(installable_libs "${LIBRARY_NAME}" "${PROJECT_LOWER_NAME}_compiler_flags")
if (TARGET "${DEPENDANT_LIBRARY}")
  list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif ()
install(TARGETS ${installable_libs}
        DESTINATION "bin"
        EXPORT "${LIBRARY_NAME}Targets")
install(FILES "${_export_file}" "logger.h" DESTINATION "include")
