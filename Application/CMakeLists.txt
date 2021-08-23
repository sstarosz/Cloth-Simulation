#Core Aplication
project(Cloth_Simulation_App 
        VERSION 0.0.1
        LANGUAGES CXX)


set(SOURCES 
    main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE "/permissive-")


#set_target_properties(${PROJECT_NAME} PROPERTIES VS_CA_EXCLUDE_PATH "C:/Qt/6.1.1/msvc2019_64")

target_link_libraries(${PROJECT_NAME}
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        MainWindow
        )


#Ship Qt dynamic library
if(WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
         add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                            COMMAND ${WINDEPLOYQT_EXECUTABLE} 
                            --debug 
                            --no-translations
                            $<TARGET_FILE:${PROJECT_NAME}>
                            DEPENDS ${PROJECT_NAME}
                            COMMENT "Running windeployqt... on $<TARGET_FILE:${PROJECT_NAME}"
                            VERBATIM)
    endif()
endif()

