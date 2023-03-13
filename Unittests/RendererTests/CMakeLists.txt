project(tests_Renderer)

set(TEST_NAME RendererTests)
set(TEST_LIBS Renderer)

set(TEST_SOURCES
    "RendererTests.cpp"
    "CameraTests.cpp"
)

include(BuildTests)

