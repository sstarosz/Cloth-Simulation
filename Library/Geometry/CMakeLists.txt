project(Geometry
		VERSION 0.0.1
		DESCRIPTION "Library that hold the geometry"
		LANGUAGES CXX)



set(SOURCES
	"Vertex.cpp"
	"Point3D.cpp"
	"Object3D.cpp"
	"Matrix4x4.cpp")

set(HEADERS
	"Vertex.hpp"
	"Point3D.hpp"
	"Object3D.hpp"
	"Matrix4x4.hpp"
)


add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(${PROJECT_NAME} INTERFACE ${PARENT_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})


target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)


#generate_documentation(TargetName)
