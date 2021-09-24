get_filename_component(LIBRARY_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "_" LIBRARY_NAME "${LIBRARY_NAME}")

include(GenerateExportHeader)

set(Header_Files "logger.h")

source_group("Header Files" FILES "${Header_Files}")
if (PLUGIN_MODE)
  set(_pp_headers "${ADOBE_PP_SDK}/Examples/Headers")
endif ()

if (PLUGIN_MODE)
  set(Source_Files "logger.cpp")
else ()
  set(Source_Files "${CMAKE_BINARY_DIR}/logger.c")
  file(READ "logger.cpp" _contents)
  file(WRITE "${Source_Files}" "${_contents}")
endif ()
source_group("Source Files" FILES "${Source_Files}")

add_library("${LIBRARY_NAME}" "${LIBRARY_TYPE_FLAG}" "${Header_Files}" "${Source_Files}")

target_include_directories(
        "${LIBRARY_NAME}"
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
        "$<INSTALL_INTERFACE:include>"
)

if (PLUGIN_MODE)
  target_include_directories(
          "${LIBRARY_NAME}"
          PUBLIC
          "${_pp_headers}"
  )
endif ()

target_link_libraries("${LIBRARY_NAME}" PUBLIC "${PROJECT_LOWER_NAME}_compiler_flags")

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  find_library(CORE_FOUNDATION_LIBRARY CoreFoundation ONLY)
  target_link_libraries("${LIBRARY_NAME}" PUBLIC "${CORE_FOUNDATION_LIBRARY}")
endif ()

if (PLUGIN_MODE)
  set_target_properties(
          "${LIBRARY_NAME}"
          PROPERTIES
          LINKER_LANGUAGE
          CXX
  )
else ()
  set_target_properties(
          "${LIBRARY_NAME}"
          PROPERTIES
          LINKER_LANGUAGE
          C
  )
endif ()

set(_export_file "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}_export.h")
generate_export_header("${LIBRARY_NAME}" EXPORT_FILE_NAME "${_export_file}")
target_compile_definitions(
        "${LIBRARY_NAME}"
        PUBLIC
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,STATIC_LIBRARY>:"${LIBRARY_NAME}"_STATIC_DEFINE=1>
)

# setup the version numbering
set_property(TARGET "${LIBRARY_NAME}" PROPERTY VERSION "1.0.0")
set_property(TARGET "${LIBRARY_NAME}" PROPERTY SOVERSION "1")

# install rules
set(installable_libs "${LIBRARY_NAME}" "${PROJECT_LOWER_NAME}_compiler_flags")
if (TARGET "${DEPENDANT_LIBRARY}")
  list(APPEND installable_libs "${DEPENDANT_LIBRARY}")
endif ()
install(TARGETS ${installable_libs}
        DESTINATION "${INSTALL_DEST}"
        EXPORT "${LIBRARY_NAME}Targets")
install(FILES "${_export_file}" "${Header_Files}" DESTINATION "include")
