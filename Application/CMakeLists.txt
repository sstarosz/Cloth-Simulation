cmake_minimum_required(VERSION 3.24)

project(Cloth_Simulation_App 
        VERSION 0.0.1
 		DESCRIPTION "Main application which is the entry point of aplication" 
        LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES "main.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})


target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE ${Compiler_Flags})
target_link_libraries(${PROJECT_NAME}
                        Qt6::Core
                        Qt6::Gui
                        Qt6::Widgets
                        MainWindow)


#Ship Qt dynamic library
if(WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
         add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                            COMMAND ${WINDEPLOYQT_EXECUTABLE} 
                            --debug 
                            --no-translations
                            --no-virtualkeyboard
                            --no-system-d3d-compiler
                            --no-opengl-sw 
                            $<TARGET_FILE:${PROJECT_NAME}>
                            DEPENDS ${PROJECT_NAME}
                            COMMENT "Running windeployqt... on $<TARGET_FILE:${PROJECT_NAME}"
                            VERBATIM)
    endif()
endif()

