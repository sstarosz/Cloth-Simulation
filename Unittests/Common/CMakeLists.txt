
project(unittests_common)

set(SOURCES
    "Helpers.cpp"
)

set(HEADERS
    "Helpers.hpp"
)


add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

