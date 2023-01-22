#Global Configuration CMake File
cmake_minimum_required(VERSION 3.20)

project(Cloth_Simulation 
        VERSION 0.0.1 
        DESCRIPTION  "Cloth simulation project" 
        LANGUAGES  CXX)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)

#Project option
option(Build_Unit_Test "Determine if build unit tests" ON)


set(Compiler_Flags "/permissive-")
set(USE_FOLDERS True)


#Project Dependicies
find_package(Qt6 CONFIG REQUIRED
             COMPONENTS Core
                        Gui
                        Widgets
                        CoreTools)



#Windeployqt for coping qt dll liblary on windows
if(MSVC)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS ${QT6_INSTALL_PREFIX})
endif()


#Include project structures
add_subdirectory(Library)
add_subdirectory(Application)
add_subdirectory(Assets)
#add_subdirectory(Docs)

add_subdirectory(Benchmarks)
if(Build_Unit_Test)
    add_subdirectory(Unittests)
endif()

add_subdirectory(Utility)
