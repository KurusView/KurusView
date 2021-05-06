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
option(PACKAGE_TESTS "Build the tests" ON)

enable_testing()
include(GoogleTest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(tests)
# --------------------------------------------------------------------
