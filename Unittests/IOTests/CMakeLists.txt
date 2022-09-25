project(tests_IO)

set(TEST_NAME IOTests)
set(TEST_LIBS IO)

set(TEST_SOURCES
    "ObjImporterTests.cpp"
)

include(BuildTests)


add_custom_command(TARGET tests_IO POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                             ${CMAKE_SOURCE_DIR}/Assets/Models/
                             $<TARGET_FILE_DIR:tests_IO>
                     COMMENT "Copy vertex shader"
                     )