cmake_minimum_required(VERSION 3.24)

project(MainWindow
		VERSION 0.0.1
		DESCRIPTION "Main Window of Aplication"
		LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
	"MainWindows.cpp"
	"NavigationBar.cpp")

set(HEADERS
	"MainWindows.hpp"
	"NavigationBar.hpp")


add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(${PROJECT_NAME} INTERFACE ${PARENT_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})

target_link_libraries(${PROJECT_NAME} 
						PRIVATE Qt6::Core
								Qt6::Gui
								Qt6::Widgets
								ViewPort
								GUI
					)
