project(Renderer
		VERSION 0.0.1
		DESCRIPTION "Vulkan Renderer"
		LANGUAGES CXX)

configure_file(Config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/Config.hpp @ONLY)

set(SOURCES
	"DebugMessenger/DebugMessenger.cpp"
	"Extensions.cpp"
	"Renderer.cpp"
	"ValidationLayers.cpp"
	"Surface.cpp"
	"PhysicalDevice.cpp"
	"QueueFamily.cpp"
	"SwapChainSupport.cpp"
	"Instance.cpp" "LogicalDevice.cpp")

set(HEADERS
	"Config.hpp"
	"DebugMessenger/DebugMessenger.hpp"
	"Extensions.cpp"
	"Renderer.hpp"
	"ValidationLayers.hpp"
	"Surface.hpp"
	"PhysicalDevice.hpp"
	"QueueFamily.hpp"
	"SwapChainSupport.hpp"
	"Instance.hpp" "LogicalDevice.cpp")




add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(${PROJECT_NAME} INTERFACE ${PARENT_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})

target_link_libraries(${PROJECT_NAME} 
						PUBLIC 
							Geometry
							Vulkan::Vulkan
					)
