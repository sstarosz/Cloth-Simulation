project(tests_ViewPort)

set(TEST_NAME ViewPortTests)
set(TEST_LIBS ViewPort)

set(TEST_SOURCES
    "ModelsMenager.cpp"
    "VulkanWindowTests.cpp"
)

include(BuildTests)

