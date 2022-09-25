project(IO
		VERSION 0.0.1
		DESCRIPTION "Library to perform operation on files, network comunication, etc"
		LANGUAGES CXX)



set(SOURCES
   "OBJImporter.cpp"
)

set(HEADERS
	"OBJImporter.hpp"
	)


add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(${PROJECT_NAME} INTERFACE ${PARENT_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})



#generate_documentation(TargetName)
