message("Tests CMakeLists")
message("----------------------------------------------------------")

# TODO: Move submodules to extern/
add_subdirectory("${PROJECT_SOURCE_DIR}/googletest" "googletest")

# Keeps the cache cleaner
mark_as_advanced(
        BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
        gmock_build_tests gtest_build_samples gtest_build_tests
        gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

# Uncomment when moved ot extern/
#set_target_properties(gtest PROPERTIES FOLDER extern)
#set_target_properties(gtest_main PROPERTIES FOLDER extern)
#set_target_properties(gmock PROPERTIES FOLDER extern)
#set_target_properties(gmock_main PROPERTIES FOLDER extern)

macro(package_add_test TESTNAME)
    # Create an exectuable in which the test will be stored
    add_executable(${TESTNAME} ${ARGN} ${INC_SOURCES})
    target_include_directories(${TESTNAME} PUBLIC ${INC_DIRS})
    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_link_libraries(${TESTNAME} gtest gmock gtest_main ${PROJECT_NAME}_LIB)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
            # set a working directory so your project root so that you can find test data via paths relative to the project root
            WORKING_DIRECTORY ${PROJECT_DIR}
            PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
            )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

# Find all cpp files in current (tests) directory
file(GLOB_RECURSE GTEST_SOURCES "*.cpp")

# Run the macro defined above on all the tests
foreach (GTEST_SOURCE ${GTEST_SOURCES})
    get_filename_component(TEST_NAME ${GTEST_SOURCE} NAME_WE)
    package_add_test(${TEST_NAME} ${GTEST_SOURCE})
endforeach ()

message("----------------------------------------------------------")
