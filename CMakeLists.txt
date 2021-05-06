# hello_qt_cm/CMakeLists.txt -----------------------------------------
# This is the minimum cmake version needed for Qt
cmake_minimum_required(VERSION 3.1.0)

foreach (p CMP0071) # 3.10: Let AUTOMOC and AUTOUIC process GENERATED files
    if (POLICY ${p})
        cmake_policy(SET ${p} NEW)
    endif ()
endforeach ()

# Name of the project
project(KurusView)

# https://stackoverflow.com/a/10055571
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message([STATUS] " Using GCC " ...)
    add_compile_options("-Wall" "-Wextra" "-pedantic")
endif ()

# The CMake build process might generate some new files in the current
# directory. This makes sure they can be found.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Place Library outputs in a lib directory within the build directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/KurusView/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/KurusView/lib)

# Place executable output in a bin directory within the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/KurusView/bin)

# This allows CMake to run one of Qt's build tools called moc
# if it is needed. moc.exe can be found in Qt's bin directory.
# We'll look at what moc does later.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find the Qt widgets package. This locates the relevant include and
# lib directories, and the necessary static libraries for linking.
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

file(GLOB_RECURSE UIS "lib/*.ui")
qt5_wrap_ui(UI_Srcs ${UIS})

set(ICONS Icons/icons.qrc)
qt5_add_resources(QRC_Srcs ${ICONS})

set(APP_ICON_RESOURCE_WINDOWS Icons/kurusview.rc)

# Just for show: This is what find_package has found:
message(STATUS "Qt5Widgets Include Dirs: ${Qt5Widgets_INCLUDE_DIRS}")
message(STATUS "Qt5Widgets Libraries: ${Qt5Widgets_LIBRARIES}")

# Link to VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

####################### Find all include directories #######################
# Find all include files
file(GLOB_RECURSE INC_SOURCES "include/*.h")
# Find all the include directories
#[[Create Empty List]]
SET(INC_DIRS "")
#[[Loop through all the include files and store their directory]]
FOREACH (file_path ${INC_SOURCES})
    GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
    SET(INC_DIRS ${INC_DIRS} ${dir_path})
ENDFOREACH ()
#[[Remove duplicates to end up with a list of all the include directories]]
LIST(REMOVE_DUPLICATES INC_DIRS)
############################################################################

# Add the include directory to prevent the need to provide the full path when including library files
include_directories(${INC_DIRS})

# Define the library and it's sources
file(GLOB_RECURSE LIB_SOURCES "lib/*.cpp")
add_library(${PROJECT_NAME}_LIB ${LIB_SOURCES} ${INC_SOURCES} ${UI_Srcs} ${QRC_Srcs})

# Same as previously: define the executable and it's sources.
file(GLOB_RECURSE EXE_SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${EXE_SOURCES} ${INC_SOURCES} ${QRC_Srcs} ${APP_ICON_RESOURCE_WINDOWS})

# Tell CMake that the library depends on the Qt::Widget and VTK libraries.
target_link_libraries(${PROJECT_NAME}_LIB Qt5::Widgets ${VTK_LIBRARIES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        # Copy the models directory and its contents to the build folder
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/models $<TARGET_FILE_DIR:${PROJECT_NAME}>/models
        # Create a save_models directory in the build folder to store any models the program saves
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/save_models)

# Tell CMake that the executable depends on the Qt::Widget and KurusView libraries.
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_LIB)

option(PACKAGE_TESTS "Build the tests" ON)
enable_testing()
include(GoogleTest)
add_subdirectory(tests)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# --------------------------------------------------------------------
