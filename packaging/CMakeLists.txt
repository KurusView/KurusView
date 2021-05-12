# Archive: ZIP, tarball, etc.
# GUI Installer: NSIS, Windows MSI, etc
# Command Line installers: DEB, RPM
# Can specify generator specific options through cmake variables
# arbitrary code can also be invoked based on generator
# Customized by using CPACK_ARCHIVE_xxx variables
# By default, single archive, no components
# To use components
# - set variable to enable
# - per component variable for archive name
# - must add more code to set expected directory structure for unpacking

####### Qt Installer Framework (IFW)
# Cross platform GUI installers for Windows, Mac and Linux
# Customized by CPACK_IFW_xxx variables
# Easy localisation of component AND GROUP DISPLAY NAMES
# Very Customizable
# Supports downloading install components
# include(CPackIFW) to get extra commands

# RPM, customized by CPACK_RPM_xxx variables, but requires rpm package tool
# Debian, customized by CPACK_DEB_xxx variables, but no tools required
if (WIN32)
    set(CPACK_GENERATOR "NSIS64")
else ()
    set(CPACK_GENERATOR "ZIP")
endif ()

# Set CPack Variables
set(CPACK_PACKAGE_NAME KurusView)
set(CPACK_PACKAGE_VENDOR "Kurus")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_TWEAK ${PROJECT_VERSION_TWEAK})
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})

# Components are just name
# Used to group installed files
# Can be grouped into a hierarchy with one component dependant on another
# For example (If you want to install header files, you also need library files)
# Not all package generators support components
# COMPONENT keyword or install associated files with components
# cpack_add_component

# Typical install, Advanced install, etc
# cpack_add_component_group

# Can include both debug and release files in a single package
# Advanced method: CMAKE_INSTALL_CMAKE_PROJECTS
# Simpler, less portable: cpack -C "Debug;Release"

# Controls a legacy behaviour, in legacy mode by default
# Forces variables to have their values written out to cpack packaging script
# Instead of variable references. More robust.
set(CPACK_VERBATIM_VARIABLES TRUE)

# Files with descriptive text
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_LIST_DIR}/../Description.txt)
set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_CURRENT_LIST_DIR}/../Welcome.txt)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_LIST_DIR}/../License.txt)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_LIST_DIR}/../README.md)

# Icon not working for some reason
#set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/Icons/resize_kurusview.bmp")

set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)

include(CPack)
include(InstallRequiredSystemLibraries)
include(DeployQt5.cmake)
install_qt5_executable("bin/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}"
        "${Qt5Gui_PLUGINS};" "" "" "plugins" TRUE)


# install(EXPORT <name>) installs <name>.cmake
# install(FILES ...)     install arbitrary files
# install(PROGRAMS ...)  install arbitrary files with the execute bit set
# install(DIRECTORY ...) install arbitrary directories
# Custom Install Logic | Allows for example specifying an ico file for windows and a png for linux
# install(SCRIPT <file> | CODE <string>
#   [COMPONENT comp]
#   [EXCLUDE_FROM_ALL])

add_custom_target(package_${PROJECT_NAME}
        "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target package
        DEPENDS ${PROJECT_NAME} ${PROJECT_NAME}_LIB
        COMMENT "Packaging ${PROJECT_NAME}...")

add_custom_target(install_${PROJECT_NAME}
        "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target install
        DEPENDS ${PROJECT_NAME} ${PROJECT_NAME}_LIB
        COMMENT "Packaging ${PROJECT_NAME}...")

# if (APPLE OR WIN32)
#    include(DeployQt5)
#    install_qt5_executable()
#endif ()
