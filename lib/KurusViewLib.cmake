message("Lib CMakeLists")
message("----------------------------------------------------------")

if (${ENABLE_TESTS})
    ####################### Only load non-QT/VTK classes for tests #######################
    # Include files
    SET(INC_SOURCES include/Material.h
            include/Matrix.h
            include/MVector.h
            include/Model.h
            include/MCell.h
            include/Cells/MHexahedron.h
            include/Cells/MPyramid.h
            include/Cells/MTetrahedron.h)
    # Include directories
    SET(INC_DIRS include include/Cells)
    # Add the include directory to prevent the need to provide the full path when including library files
    include_directories(${INC_DIRS})

    # Define the library sources
    file(LIB_SOURCES lib/Material.cpp
            lib/Matrix.cpp
            lib/MVector.cpp
            lib/Model.cpp
            lib/MCell.cpp
            lib/Cells/MHexahedron.cpp
            lib/Cells/MPyramid.cpp
            lib/Cells/MTetrahedron.cpp)

    # Define the library, without any QT Stuffs
    add_library(${PROJECT_NAME}_LIB ${LIB_SOURCES} ${INC_SOURCES})
else ()
    # This allows CMake to run one of Qt's build tools called moc
    # if it is needed. moc.exe can be found in Qt's bin directory.
    # We'll look at what moc does later.
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    # Find the Qt widgets package. This locates the relevant include and
    # lib directories, and the necessary static libraries for linking.
    find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

    file(GLOB_RECURSE UIS "lib/*.ui" PUBLIC)
    qt5_wrap_ui(UI_Srcs ${UIS})

    # TODO Change name from icons, since it doesn't only contain icons
    set(ICONS Icons/icons.qrc)
    qt5_add_resources(QRC_Srcs ${ICONS})

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

    # Tell CMake that the library depends on the Qt::Widget and VTK libraries.
    target_link_libraries(${PROJECT_NAME}_LIB Qt5::Widgets ${VTK_LIBRARIES})
endif ()

message("----------------------------------------------------------")
