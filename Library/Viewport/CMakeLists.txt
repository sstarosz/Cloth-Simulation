cmake_minimum_required(VERSION 3.24)


project(ViewPort3D
		VERSION 0.0.1
		DESCRIPTION "3D Viewport of aplication"
		LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)



configure_file(Config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/Config.hpp @ONLY)

set(SOURCES
	"ViewportWidget.cpp"
	"VulkanWindow.cpp"
	)

set(HEADERS
	"ViewportWidget.hpp"
	"VulkanWindow.hpp"
	"Config.hpp"
	)


add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(${PROJECT_NAME} INTERFACE ${PARENT_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})

target_link_libraries(${PROJECT_NAME} 
						PUBLIC
						Renderer
						Simulation
						Vulkan::Vulkan
						Qt6::Core
						Qt6::Gui
						Qt6::Widgets
					)
