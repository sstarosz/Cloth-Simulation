



add_executable(${PROJECT_NAME} ${TEST_SOURCES} ${CMAKE_SOURCE_DIR}/Unittests/TestMain.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_link_libraries(${PROJECT_NAME} ${TEST_LIBS} unittests_common GTest::gtest GTest::gmock)

add_test(NAME ${TEST_NAME} COMMAND ${PROJECT_NAME})
