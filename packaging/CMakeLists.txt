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
    set(CPACK_GENERATOR "ZIP;NSIS64")
else ()
    set(CPACK_GENERATOR "ZIP;TGZ;DEB")
endif ()

######################## CPack Variables ########################
# Place installers in the packages directory inside build
set(CPACK_OUTPUT_FILE_PREFIX packages)

# Set CPack Variables
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Kurus")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_TWEAK ${PROJECT_VERSION_TWEAK})
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})

# Allows creating DEB packages, as CPACK_DEBIAN_PACKAGE_MAINTAINER,
# which is mandatory, defaults to this variable
set(CPACK_PACKAGE_CONTACT "eeyoa10@nottingham.ac.uk")

# Controls a legacy behaviour, in legacy mode by default
# Forces variables to have their values written out to cpack packaging script
# Instead of variable references. More robust.
set(CPACK_VERBATIM_VARIABLES TRUE)

# Files with descriptive text
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/Description.txt)
set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_SOURCE_DIR}/Welcome.txt)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/License.txt)
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
# Icons
set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\Icons\\\\old_kurusview.bmp")
# Adds start menu icon, given in the format "exe_name;start_menu_name"
set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME};${PROJECT_NAME}")
# Creates Desktop Icon
set(CPACK_CREATE_DESKTOP_LINKS "KurusView")
# Gives page for user to choose to add to path and toggle create desktop icon
set(CPACK_NSIS_MODIFY_PATH ON)
# Adds an option to run the program after installation
set(CPACK_NSIS_MUI_FINISHPAGE_RUN "${PROJECT_NAME}.exe")
# Enables parallel compression of the package if available
# Setting it to 0 uses all available CPU cores
set(CPACK_THREADS 0)

set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)

######################## CPack NSIS Variables ########################
# Installer icon, given as exe that contains icon
set(CPACK_NSIS_INSTALLED_ICON_NAME bin\\\\${PROJECT_NAME}.exe)

# Installer and uninstaller program icon (.ico)
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}\\\\Icons\\\\kurusview.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}\\\\Icons\\\\kurusview.ico")

# Welcome and finish page BMPs
set(CPACK_NSIS_MUI_WELCOMEFINISHPAGE_BITMAP "${CMAKE_SOURCE_DIR}\\\\Icons\\\\welcomefinish.bmp")
set(CPACK_NSIS_MUI_UNWELCOMEFINISHPAGE_BITMAP "${CMAKE_SOURCE_DIR}\\\\Icons\\\\welcomefinish.bmp")
# Header BMP
set(CPACK_NSIS_MUI_HEADERIMAGE "${CMAKE_SOURCE_DIR}\\\\Icons\\\\header.bmp")

# Allows user to uninstall previous version (if exists) before installing
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/KurusView/2020_Group_21")
set(CPACK_NSIS_HELP_LINK "https://KurusView.github.io")

get_filename_component(path_to_plugins "${CMAKE_SOURCE_DIR}" ABSOLUTE)
file(TO_NATIVE_PATH "${path_to_plugins}" path_to_plugins)
string(REPLACE "\\" "/" path_to_plugins "${path_to_plugins}")

# Include FileAssociation.nsh
set(CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS
        "!include \"${path_to_plugins}\\\\packaging\\\\FileAssociation.nsh\"")

# Create association
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
        "\${RegisterExtension} '$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe' '.kurus' 'Kurus_View_Model'")

# Remove association
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS
        "\${UnRegisterExtension} '.kurus' 'Kurus View Model'")

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
