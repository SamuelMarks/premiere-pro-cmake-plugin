get_filename_component(LIBRARY_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "_" LIBRARY_NAME "${LIBRARY_NAME}")
string(TOUPPER "${LIBRARY_NAME}" LIBRARY_UPPER_NAME)

######################
# Overridable config #
######################

set(PP_EXE "" CACHE PATH "Location of Premiere Pro program")
if ((NOT DEFINED PP_EXE OR PP_EXE STREQUAL "") AND MSVC)
    set(PP_EXE "$ENV{ProgramFiles}\\Adobe\\Adobe Premiere Pro 2021\\Adobe Premiere Pro.exe")
endif()

set(ADOBE_PP_SDK "Premiere Pro CC 13.0 Mac SDK" CACHE PATH "Location of Premiere Pro SDK")
if (NOT IS_DIRECTORY "${ADOBE_PP_SDK}")
    message(FATAL_ERROR "ADOBE_PP_SDK must be set to the directory of the Premiere Pro SDK")
endif ()

#################
# Source groups #
#################

set(Adobe_SDK_Header_Files
        "${ADOBE_PP_SDK}/Examples/Headers/PrSDKErrorSuite.h"
        "${ADOBE_PP_SDK}/Examples/Headers/PrSDKExportController.h"
        "${ADOBE_PP_SDK}/Examples/Headers/PrSDKExportControllerSuite.h"
        "${ADOBE_PP_SDK}/Examples/Headers/PrSDKSequenceInfoSuite.h"
        "${ADOBE_PP_SDK}/Examples/Headers/PrSDKTimeSuite.h"
        "${ADOBE_PP_SDK}/Examples/Headers/PrSDKTypes.h"
)

set(Header_Files
        "ExportController.h"
)
source_group("Header Files" FILES "${Header_Files}")
source_group("Adobe SDK Header Files" FILES "${Adobe_SDK_Header_Files}")

set(Source_Files "ExportController.cpp")
source_group("Source Files" FILES "${Source_Files}")

set(Utility_Files
        "${ADOBE_PP_SDK}/Examples/Utils/SDK_File.cpp"
        "${ADOBE_PP_SDK}/Examples/Utils/SDK_File.h"
)
source_group("Utility Files" FILES "${Utility_Files}")

set(ALL_FILES
        "${Adobe_SDK_Header_Files}"
        "${Header_Files}"
        "${Source_Files}"
        "${Utility_Files}"
)

##########
# Target #
##########

add_library("${LIBRARY_NAME}" SHARED "${ALL_FILES}")

###############
# Target name #
###############

set_target_properties(
        "${LIBRARY_NAME}" PROPERTIES
        TARGET_NAME_DEBUG   "${LIBRARY_NAME}$ENV{TargetExt}"
        TARGET_NAME_RELEASE "${LIBRARY_NAME}$ENV{TargetExt}"
        TARGET_NAME         "${LIBRARY_NAME}$ENV{TargetExt}"
)

####################
# Output directory #
####################

#set_target_properties(
#        "${LIBRARY_NAME}" PROPERTIES
#
#        OUTPUT_DIRECTORY_DEBUG           "$ENV{PREMSDKBUILDPATH}"
#        OUTPUT_DIRECTORY_RELEASE         "$ENV{PREMSDKBUILDPATH}"
#        OUTPUT_DIRECTORY                 "$ENV{PREMSDKBUILDPATH}"
#
#        ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "$ENV{PREMSDKBUILDPATH}"#"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
#        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "$ENV{PREMSDKBUILDPATH}"#"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
#        ARCHIVE_OUTPUT_DIRECTORY         "$ENV{PREMSDKBUILDPATH}"
#
#        RUNTIME_OUTPUT_DIRECTORY_DEBUG   "$ENV{PREMSDKBUILDPATH}"#"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
#        RUNTIME_OUTPUT_DIRECTORY_RELEASE "$ENV{PREMSDKBUILDPATH}"#"${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
#        RUNTIME_OUTPUT_DIRECTORY         "$ENV{PREMSDKBUILDPATH}"
#
#        LIBRARY_OUTPUT_DIRECTORY_DEBUG   "$ENV{PREMSDKBUILDPATH}"
#        LIBRARY_OUTPUT_DIRECTORY_RELEASE "$ENV{PREMSDKBUILDPATH}"
#        LIBRARY_OUTPUT_DIRECTORY         "$ENV{PREMSDKBUILDPATH}"
#)

#######################
# Include directories #
#######################

set(_include_dirs
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${ADOBE_PP_SDK}/Examples/Utils"
        "${ADOBE_PP_SDK}/Examples/Headers")
set(_libs "")

set_target_properties(
        "${LIBRARY_NAME}"
        PROPERTIES
        LINKER_LANGUAGE
        CXX
)

if (MSVC)
    set_target_properties("${LIBRARY_NAME}" PROPERTIES LINK_FLAGS /NODEFAULTLIB:\"MSVCRT\")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_library(CORE_FOUNDATION_LIBRARY CoreFoundation ONLY)
    list(APPEND _libs "${CORE_FOUNDATION_LIBRARY}")
    find_library(APPLICATION_SERVICES_LIBRARY ApplicationServices ONLY)
    list(APPEND _libs "${APPLICATION_SERVICES_LIBRARY}")
endif ()

find_package(ZLIB REQUIRED)
list(APPEND _libs "ZLIB::ZLIB")

find_package(OpenSSL REQUIRED)
list(APPEND _libs "OpenSSL::SSL")
list(APPEND _libs "OpenSSL::Crypto")

target_include_directories(
        "${LIBRARY_NAME}"
        PUBLIC
        "${_include_dirs}"
)

target_link_libraries(
        "${LIBRARY_NAME}"
        LINK_PUBLIC
        "${_libs}"
        "${PP_LIBRARIES}"
)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Out")

if (MSVC)
    set_target_properties (
            "${LIBRARY_NAME}" PROPERTIES
            VS_DEBUGGER_COMMAND "${PP_EXE}"
    )
endif()

# Mostly reverse-engineered from Premiere Pro sample config:
if (MSVC)
    set_target_properties("${LIBRARY_NAME}" PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${LIBRARY_NAME} PRIVATE
                $<$<CONFIG:Debug>:
                /DEBUG
                >
                $<$<CONFIG:Release>:
                /DEBUG:FULL;
                /OPT:REF;
                /OPT:ICF
                >
                /MACHINE:X64
                )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${LIBRARY_NAME} PRIVATE
                $<$<CONFIG:Debug>:
                /DEBUG
                >
                $<$<CONFIG:Release>:
                /DEBUG:FULL;
                /OPT:REF;
                /OPT:ICF
                >
                )
    endif ()
endif (MSVC)

####################################
# Shared and static library config #
####################################

set(_export_file "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}_export.h")
generate_export_header("${LIBRARY_NAME}" EXPORT_FILE_NAME "${_export_file}")
target_compile_definitions(
        "${LIBRARY_NAME}"
        PUBLIC
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,STATIC_LIBRARY>:"${LIBRARY_NAME}"_STATIC_DEFINE=1>
)

# setup the version numbering
set_property(TARGET "${LIBRARY_NAME}" PROPERTY VERSION "${${PROJECT_NAME}_VERSION}")
set_property(TARGET "${LIBRARY_NAME}" PROPERTY SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}")

###########
# Install #
###########

# install rules
set(installable_libs "${LIBRARY_NAME}" "${PROJECT_LOWER_NAME}_compiler_flags")
foreach (_lib ${PP_LIBRARIES})
    if (TARGET "${DEPENDANT_LIBRARY}")
        list(APPEND installable_libs "${_lib}")
    endif ()
endforeach ()

install(TARGETS ${installable_libs}
        DESTINATION "${INSTALL_DEST}"
        EXPORT "${LIBRARY_NAME}Targets")
install(FILES "${_export_file}" "${Header_Files}" DESTINATION "include")
