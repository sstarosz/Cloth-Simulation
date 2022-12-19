project(GUI
		VERSION 0.0.1
		DESCRIPTION "Graphical User Interface of the application"
		LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
	"SettingsView.cpp"
	"OutlinerView.cpp"
	"GuiElements/Slider.cpp"
	"GuiElements/WidgetBox.cpp"
	"GuiElements/Slider.cpp"
	"TransformationView.cpp")

set(HEADERS
	"SettingsView.hpp"
	"OutlinerView.hpp"
	"GuiElements/Slider.hpp"
	"GuiElements/WidgetBox.hpp"
	"GuiElements/Slider.hpp"
	"TransformationView.hpp")


#TODO
#GUI Elements
# [o]- slider (improve)
# [ ]- conteiner for other widgets (tree like)
# [ ]- vertical and horizontal line


add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
target_include_directories(${PROJECT_NAME} INTERFACE ${PARENT_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})

target_link_libraries(${PROJECT_NAME} 
						PRIVATE Qt6::Core
								Qt6::Gui
								Qt6::Widgets
)
