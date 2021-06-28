cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0048 NEW)
project(pp VERSION 0.0.0 LANGUAGES C CXX)

if (MSVC)
    set(CMAKE_CXX_STANDARD 11)
else ()
    set(CMAKE_CXX_STANDARD 17)
endif ()

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

#############################################
# Configuration file vars for interpolation #
#############################################

if (NOT DEFINED TARGET_ARCH)
    set(TARGET_ARCH ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif ()

################################
# Configure file (interpolate) #
################################

configure_file("src/configure.h.in" "${PROJECT_BINARY_DIR}/configure/configure.h" @ONLY)

####################
# Output directory #
####################

# set(LIBRARY_TYPE_FLAG "SHARED")
set(PREMSDKBUILDPATH "" CACHE PATH "Location of Premiere Pro plugin directory")
if(NOT DEFINED ENV{PREMSDKBUILDPATH})
    if(APPLE)
        set(ENV{PREMSDKBUILDPATH} "/Library/Application Support/Adobe/Common/Plug-ins/7.0/MediaCore/")
        message(STATUS \$ENV{PREMSDKBUILDPATH} = "$ENV{PREMSDKBUILDPATH}")
    else()
        set(ENV{PREMSDKBUILDPATH} "ENV{ProgramFiles}\\Adobe\\Common\\Plug-ins\\7.0\\MediaCore\\")
    endif()
endif()
if(NOT IS_DIRECTORY "${PREMSDKBUILDPATH}")
    set(PREMSDKBUILDPATH "$ENV{PREMSDKBUILDPATH}")
endif()

#    # First for the generic no-config case (e.g. with mingw)
#    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "$ENV{PREMSDKBUILDPATH}" )
#    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "$ENV{PREMSDKBUILDPATH}" )
#    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "$ENV{PREMSDKBUILDPATH}" )
#    # Second, for multi-config builds (e.g. msvc)
#    foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
#        string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
#        set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "$ENV{PREMSDKBUILDPATH}" )
#        set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "$ENV{PREMSDKBUILDPATH}" )
#        set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "$ENV{PREMSDKBUILDPATH}" )
#    endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

################################################################################
# Common utils
################################################################################
#include(CMake/Utils.cmake)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################

include_directories("${CMAKE_BINARY_DIR}/configure")

###########################
# Build these subprojects #
###########################
include(GenerateExportHeader)

set(PROJ_NAME "pp-plugin")
add_subdirectory("src/${PROJ_NAME}")
set_target_properties(
        "${PROJ_NAME}" PROPERTIES SUFFIX ".prm"
)

if (MSVC)
    set_property (DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "${PROJ_NAME}")
endif ()

################################################################################
# Compile and link options
################################################################################

#####################################
# Compiler, library, and exec flags #
#####################################
if (MSVC)
    add_compile_options(
            /WX
            /permissive-
            /Oi
            /Ob0  # Disable in release (at some future point)
            /GL
            /Gy
            /FC
            "$<$<CONFIG:DEBUG>:-/MDd>"
            "$<$<CONFIG:RELEASE>:-/MD>"
    )
    add_link_options(
            /LTCG
            /Zi
    )
    add_compile_definitions(
            _WINDLL
    )
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB")
else ()
    add_compile_options(
            "$<$<CONFIG:DEBUG>:-Wall>"
            "$<$<CONFIG:DEBUG>:-Werror>"
            "$<$<CONFIG:DEBUG>:-pedantic>"
            "$<$<CONFIG:DEBUG>:-Wno-missing-braces>"
            "$<$<CONFIG:RELEASE>:-O3>"
    )

    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_compile_options("$<$<CONFIG:DEBUG>:-pedantic>")
    else ()
        add_compile_options("$<$<CONFIG:DEBUG>:-Wpedantic>")
    endif ()

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
        set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    endif()
endif ()
