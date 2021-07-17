get_filename_component(EXEC_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "_" EXEC_NAME ${EXEC_NAME})

#################
# Create target #
#################

set(Source_Files "main.c")
source_group("Source Files" FILES "${Source_Files}")

add_executable("${EXEC_NAME}" "${Source_Files}")

target_link_libraries("${EXEC_NAME}" LINK_PUBLIC "${PP_LIBRARIES}")

if (MSVC)
    set_target_properties("${PROJ_NAME}" PROPERTIES LINK_FLAGS /NODEFAULTLIB:\"MSVCRT\")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    find_library(CORE_FOUNDATION_LIBRARY CoreFoundation ONLY)
    find_library(APPLICATION_SERVICES_LIBRARY ApplicationServices ONLY)
    target_link_libraries("${PROJ_NAME}" PUBLIC "${CORE_FOUNDATION_LIBRARY}" "${APPLICATION_SERVICES_LIBRARY}")
endif ()

###########
# Install #
###########

# install rules
set(installable_libs "${EXEC_NAME}" "${PROJECT_LOWER_NAME}_compiler_flags")
foreach (_lib ${PP_LIBRARIES})
    if (TARGET "${DEPENDANT_LIBRARY}")
        list(APPEND installable_libs "${_lib}")
    endif ()
endforeach ()

install(TARGETS ${installable_libs}
        DESTINATION "${INSTALL_DEST}"
        EXPORT "${EXEC_NAME}Targets")
