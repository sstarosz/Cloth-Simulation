#Global Configuration CMake File
cmake_minimum_required(VERSION 3.20)

project(Cloth_Simulation 
        VERSION 0.0.1 
        DESCRIPTION  "Cloth simulation project" 
        LANGUAGES  CXX)



#Project option
option(Build_Unit_Test "Determine if build unit tests" OFF)

#Cmake Options
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/Cmake" ${CMAKE_MODULE_PATH})


#C++ Definitions
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED True)

#Qt Utility
set(CMAKE_PREFIX_PATH "C:/Qt/6.1.2/msvc2019_64")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#Project Dependicies
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        CoreTools
        REQUIRED)


#Windeployqt for coping qt dll liblary on windows
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS ${QT6_INSTALL_PREFIX})


#Include project structures
add_subdirectory(Library)
add_subdirectory(Application)
if(Build_Unit_Test)
    add_subdirectory(Unittest)
endif()

