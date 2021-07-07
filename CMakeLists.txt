cmake_minimum_required(VERSION 3.16)

project(Cloth_Simulation 
        VERSION 0.0.1 
        DESCRIPTION  "Application for cloth simulation" 
        LANGUAGES  CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Qt Utility
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(CMAKE_PREFIX_PATH "C:/Qt/6.1.1/msvc2019_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)



add_subdirectory(Application)
