project(tests_ViewPort)

set(TEST_NAME ViewPortTests)
set(TEST_LIBS ViewPort)

set(TEST_SOURCES
    "ModelsMenager.cpp"
    "VulkanWindowTests.cpp"
)

include(BuildTests)


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