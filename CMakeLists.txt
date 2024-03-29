# CMakeLists.txt -----------------------------------------
# This is the minimum cmake version needed for Qt
cmake_minimum_required(VERSION 3.1.0)

foreach (p CMP0071) # 3.10: Let AUTOMOC and AUTOUIC process GENERATED files
    if (POLICY ${p})
        cmake_policy(SET ${p} NEW)
    endif ()
endforeach ()

# https://stackoverflow.com/a/10055571
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message([STATUS] " Using GCC " ...)
    add_compile_options("-Wall" "-Wextra" "-pedantic")
endif ()

# Name of the project
project(KurusView
        LANGUAGES C CXX
        VERSION 1.0.0
        DESCRIPTION "3D Visualisation Tool"
        HOMEPAGE_URL "https://KurusView.github.io")

# The CMake build process might generate some new files in the current
# directory. This makes sure they can be found.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(lib/KurusViewLib.cmake)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
#qt_finalize_executable(${PROJECT_NAME})

if (NOT DEFINED ENABLE_TESTS OR NOT ENABLE_TESTS)
    message("ENABLE_TESTS is not defined")
    message("Building with QT and VTK, tests disabled")
    message("-------------------------------------")
    option(PACKAGE_TESTS "Build the tests" OFF)
    include(src/KurusView.cmake)
elseif (ENABLE_TESTS)
    message("ENABLE_TESTS is defined")
    message("Building without QT or VTK, tests enabled")
    message("-------------------------------------")
    option(PACKAGE_TESTS "Build the tests" ON)
    enable_testing()
    include(GoogleTest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(tests)
endif ()
# --------------------------------------------------------------------
