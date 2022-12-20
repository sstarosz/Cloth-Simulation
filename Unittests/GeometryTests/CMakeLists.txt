project(tests_Geometry)

set(TEST_NAME GeometryTests)
set(TEST_LIBS Geometry)

set(TEST_SOURCES
    "Matrix4x4Tests.cpp"
)

include(BuildTests)
