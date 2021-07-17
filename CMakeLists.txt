cmake_minimum_required(VERSION 3.13)
cmake_policy(SET CMP0048 NEW)
project(pp VERSION 0.0.0 LANGUAGES C CXX)
string(TOLOWER "${PROJECT_NAME}" PROJECT_LOWER_NAME)

if (MSVC)
    set(CMAKE_CXX_STANDARD 11)
else ()
    set(CMAKE_CXX_STANDARD 17)
endif ()

##################
# Compiler flags #
##################

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

add_library("${PROJECT_LOWER_NAME}_compiler_flags" INTERFACE)
target_compile_features("${PROJECT_LOWER_NAME}_compiler_flags" INTERFACE "cxx_std_11")

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like "$<COMPILE_LANG_AND_ID:C,CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc "$<COMPILE_LANG_AND_ID:C,CXX,MSVC>")
# -g -fsanitize=address -fno-omit-frame-pointer
target_compile_options(
        "${PROJECT_LOWER_NAME}_compiler_flags"
        INTERFACE
        "$<${gcc_like}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
        "$<${msvc}:$<BUILD_INTERFACE:-W3;-WX;-Zi;-permissive->>"
)

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

if (NOT DEFINED PLUGIN_MODE AND NOT DEFINED STANDALONE_MODE)
    set(PLUGIN_MODE 1)
endif ()

# Instead of `BUILD_SHARED_LIBS`
if (PLUGIN_MODE)
    set(LIBRARY_TYPE_FLAG "SHARED")
else ()
    set(LIBRARY_TYPE_FLAG "STATIC")
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
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(ENV{PREMSDKBUILDPATH} "/Library/Application Support/Adobe/Common/Plug-ins/7.0/MediaCore/${PROJECT_NAME}/")
    else()
        set(ENV{PREMSDKBUILDPATH} "ENV{ProgramFiles}\\Adobe\\Common\\Plug-ins\\7.0\\MediaCore\\${PROJECT_NAME}\\")
    endif()
    message(STATUS \$ENV{PREMSDKBUILDPATH} = "$ENV{PREMSDKBUILDPATH}")
endif()
if(NOT IS_DIRECTORY "${PREMSDKBUILDPATH}")
    set(PREMSDKBUILDPATH "$ENV{PREMSDKBUILDPATH}")
endif()
if(PLUGIN_MODE AND NOT IS_DIRECTORY "${PREMSDKBUILDPATH}")
    file(MAKE_DIRECTORY "${PREMSDKBUILDPATH}")
endif()
# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
elseif(UNIX)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

# configure a header file to pass the version number only
configure_file(
        "${PROJECT_NAME}Config.h.in"
        "${PROJECT_NAME}Config.h"
)

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

#################################
# Testing configuration (CPack) #
#################################
include(CTest)

###########################
# Build these subprojects #
###########################
include(GenerateExportHeader)

set(PP_LIBRARIES "common" "versions")
set(PP_INCLUDE_DIRS "")
foreach (_lib ${PP_LIBRARIES})
    add_subdirectory("src/${_lib}")
    list(APPEND PP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/${_lib}")
    message(STATUS "Built ${_lib} at ${CMAKE_CURRENT_BINARY_DIR}")
endforeach ()

if (PLUGIN_MODE)
    set(PROJ_NAME "pp-plugin")
    add_subdirectory("src/${PROJ_NAME}")
    set_target_properties("${PROJ_NAME}" PROPERTIES SUFFIX ".prm")
else ()
    set(PROJ_NAME "standalone")
    add_subdirectory("src/${PROJ_NAME}")
endif()

if (MSVC)
    set_property (DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT "${PROJ_NAME}")
endif ()

################################################################################
# Compile and link options
################################################################################

#####################################
# Compiler, library, and exec flags #
#####################################

# Most of these are reverse-engineered from Premiere Pro
#
#if (MSVC)
#    add_compile_options(
#            /WX
#            /permissive-
#            /Oi
#            /Ob0  # Disable in release (at some future point)
#            /GL
#            /Gy
#            /FC
#            "$<$<CONFIG:DEBUG>:-/MDd>"
#            "$<$<CONFIG:RELEASE>:-/MD>"
#    )
#    add_link_options(
#            /LTCG
#            /Zi
#    )
#    add_compile_definitions(
#            _WINDLL
#    )
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB")
#else ()
#    add_compile_options(
#            "$<$<CONFIG:DEBUG>:-Wall;-Werror;-pedantic;-Wno-missing-braces>"
#            "$<$<CONFIG:RELEASE>:-O3>"
#    )
#
#    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
#        add_compile_options("$<$<CONFIG:DEBUG>:-pedantic>")
#    else ()
#        add_compile_options("$<$<CONFIG:DEBUG>:-Wpedantic>")
#    endif ()
#
#    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
#        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#        set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#    endif()
#endif ()


###################################
# Installer configuration (CPack) #
###################################

if (PLUGIN_MODE)
    set(INSTALL_DEST "lib")
else ()
    set(INSTALL_DEST "bin")
endif ()

# add the install targets

install(
        TARGETS "${PROJ_NAME}"
        DESTINATION "${INSTALL_DEST}"
)
install(
        FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h"
        DESTINATION "include"
)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")

include(CPack)
include(CMakePackageConfigHelpers)

foreach (LIBRARY_NAME_0 ${PP_LIBRARIES})
    # install the configuration targets
    install(
            EXPORT "${LIBRARY_NAME_0}Targets"
            FILE "${LIBRARY_NAME_0}Targets.cmake"
            DESTINATION "lib/cmake/${LIBRARY_NAME_0}"
    )

    # generate the config file that is includes the exports
    configure_package_config_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_0}Config.cmake"
            INSTALL_DESTINATION "lib/cmake/example"
            NO_SET_AND_CHECK_MACRO
            NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )

    # generate the version file for the config file
    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_0}ConfigVersion.cmake"
            VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}"
            COMPATIBILITY AnyNewerVersion
    )

    # install the configuration file
    install(
            FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_0}Config.cmake"
            DESTINATION "lib/cmake/${LIBRARY_NAME_0}"
    )

    # generate the export targets for the build tree
    # needs to be after the install(TARGETS ) command
    export(
            EXPORT "${LIBRARY_NAME_0}Targets"
            FILE "${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME_0}Targets.cmake"
    )
endforeach ()
