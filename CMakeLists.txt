# hello_qt_cm/CMakeLists.txt -----------------------------------------
# This is the minimum cmake version needed for Qt
cmake_minimum_required(VERSION 3.1.0)

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

# Just for show: This is what find_package has found:
message(STATUS "Qt5Widgets Include Dirs: ${Qt5Widgets_INCLUDE_DIRS}")
message(STATUS "Qt5Widgets Libraries: ${Qt5Widgets_LIBRARIES}")

# Add the include directory to prevent the need to provide the full path when including library files
include_directories(include include/Cells)

# Define the library and it's sources
add_library(${PROJECT_NAME} lib/MVector.cpp include/MVector.h lib/MCell.cpp include/MCell.h lib/Material.cpp include/Material.h lib/Model.cpp include/Model.h lib/Matrix.cpp include/Matrix.h lib/ModelRenderer.cpp include/ModelRenderer.h lib/Cells/MTetrahedron.cpp include/Cells/MTetrahedron.h lib/Cells/MHexahedron.cpp include/Cells/MHexahedron.h lib/Cells/MPyramid.cpp include/Cells/MPyramid.h)

# Same as previously: define the executable and it's sources.
add_executable(KurusViewEXE src/main.cpp)

# Tell CMake that the executable depends on the Qt::Widget and KurusView libraries.
target_link_libraries(KurusViewEXE Qt5::Widgets ${PROJECT_NAME})
# --------------------------------------------------------------------
