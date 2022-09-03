project(Geometry
		VERSION 0.0.1
		DESCRIPTION "Library that hold the geometry"
		LANGUAGES CXX)



set(SOURCES
	"Vertex.cpp"
)

set(HEADERS
	"Vertex.hpp"
)


add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(${PROJECT_NAME} INTERFACE ${PARENT_DIR})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)


target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)
