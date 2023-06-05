cmake_minimum_required(VERSION 3.24)


project(Core
		VERSION 0.0.1
		DESCRIPTION "Core part of the application"
		LANGUAGES CXX)



set(SOURCES
	"Scene.cpp"
	"ModelsManager.cpp")

set(HEADERS
	"Scene.hpp"
	"ModelsManager.hpp")


add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(${PROJECT_NAME} INTERFACE ${PARENT_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})

target_link_libraries(${PROJECT_NAME} Geometry)


#generate_documentation(TargetName)
