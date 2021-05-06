message("Src CMakeLists")
message("----------------------------------------------------------")

if (WIN32)
    set(CPACK_GENERATOR "NSIS")
else ()
    set(CPACK_GENERATOR "ZIP")
endif ()
include(CPack)

# Place Library outputs in a lib directory within the build directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/KurusView/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/KurusView/lib)

# Place executable output in a bin directory within the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/KurusView/bin)

set(APP_ICON_RESOURCE_WINDOWS ../Icons/kurusview.rc)


# Same as previously: define the executable and it's sources.
file(GLOB_RECURSE EXE_SOURCES "*.cpp")
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${EXE_SOURCES} ${INC_SOURCES} ${QRC_Srcs} ${APP_ICON_RESOURCE_WINDOWS})

target_include_directories(${PROJECT_NAME} PUBLIC ${INC_DIRS})

# Tell CMake that the executable depends on the Qt::Widget and KurusView libraries.
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_LIB)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        # Copy the models directory and its contents to the build folder
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/models $<TARGET_FILE_DIR:${PROJECT_NAME}>/models
        # Create a save_models directory in the build folder to store any models the program saves
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/save_models)

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_LIB
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

add_custom_target(package_${PROJECT_NAME}
        "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target package
        DEPENDS ${PROJECT_NAME} ${PROJECT_NAME}_LIB
        COMMENT "Packaging ${PROJECT_NAME}...")
