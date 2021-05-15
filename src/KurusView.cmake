message("Src CMakeLists")
message("----------------------------------------------------------")

# Place Library outputs in a lib directory within the build directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/KurusView/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/KurusView/lib)
# Place executable output in a bin directory within the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/KurusView/bin)

set(APP_ICON_RESOURCE_WINDOWS Icons/kurusview.rc)

# Define the executable and it's sources.
file(GLOB_RECURSE EXE_SOURCES "src/*.cpp")
# Add WIN32 to prevent cmd from opening
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${EXE_SOURCES} ${INC_SOURCES} ${QRC_Srcs} ${APP_ICON_RESOURCE_WINDOWS})

target_include_directories(${PROJECT_NAME} PUBLIC ${INC_DIRS})

# Tell CMake that the executable depends on the Qt::Widget and KurusView libraries.
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_LIB)

qt5_use_modules(${PROJECT_NAME} Widgets Core Gui)

####################### Installation #######################
# Gather VTK DLLs and add to install as VTK Component
file( GLOB VTK_DLLS ${VTK_RUNTIME_LIBRARY_DIRS}/*.dll )
if ( VTK_DLLS )
    foreach ( VTK_Library ${VTK_DLLS})
        # Add it to the list of 'desired' vtk-libraries for later installation
        list( APPEND VTK_Install_Libraries ${VTK_Library} )
    endforeach ()
    list(REMOVE_DUPLICATES VTK_Install_Libraries)
    install(FILES ${VTK_Install_Libraries}
            DESTINATION bin
            COMPONENT VTK)
endif ()

foreach (Qt_library ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES})
    # With QT_USE_IMPORTED_TARGETS, we should extract the dll info
    # from the target properties
    get_target_property( Qt_lib_name ${Qt_library} IMPORTED_LOCATION )
    get_target_property( Qt_lib_name_debug ${Qt_library} IMPORTED_LOCATION_DEBUG )
    get_target_property( Qt_lib_name_release ${Qt_library} IMPORTED_LOCATION_RELEASE )

    # Initially assume the DEBUG dlls should be installed, but
    # fall back to release if necessary
    if ( Qt_lib_name_debug AND EXISTS ${Qt_lib_name_debug} )
        set( Qt_library_location ${Qt_lib_name_debug} )
    elseif ( Qt_lib_name_release AND EXISTS ${Qt_lib_name_release} )
        set( Qt_library_location ${Qt_lib_name_release} )
    elseif ( Qt_lib_name AND EXISTS ${Qt_lib_name} )
        set( Qt_library_location ${Qt_lib_name} )
    endif ( Qt_lib_name_debug AND EXISTS ${Qt_lib_name_debug} )

    set(QT_BINARY_DIR "C://Qt//5.15.1//msvc2019_64//bin")

    # Extract the filename part, without the lib-prefix or the .a or ..lib suffix
    get_filename_component( Qt_library_name ${Qt_library_location} NAME_WE )
    string( REGEX REPLACE "^lib(.*)" "\\1" Qt_library_name ${Qt_library_name} )

    set( Qt_shared_library ${QT_BINARY_DIR}/${Qt_library_name}.dll )
    if ( EXISTS ${Qt_shared_library})
        # Add it to the list of 'desired' qt-libraries for later installation
        list( APPEND Qt_Install_Libraries ${Qt_shared_library} )
    else ( EXISTS ${Qt_shared_library} )
        message( WARNING "    could not find ${Qt_shared_library}" )
    endif ( EXISTS ${Qt_shared_library} )
endforeach ()

list( REMOVE_DUPLICATES Qt_Install_Libraries )
install( FILES ${Qt_Install_Libraries}
        DESTINATION bin
        COMPONENT Qt )


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        # Copy the models directory and its contents to the build folder
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/models $<TARGET_FILE_DIR:${PROJECT_NAME}>/models
        # Create a save_models directory in the build folder to store any models the program saves
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/save_models)

#FILE(GLOB_RECURSE model_files "models/*")
#install(FILES ${model_files}
#        DESTINATION example_models
#        COMPONENT "Example Models")

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_LIB
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        COMPONENT KurusView)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(packaging)
endif ()
message("----------------------------------------------------------")
