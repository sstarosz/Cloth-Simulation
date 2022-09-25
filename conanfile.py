from conans import ConanFile, CMake


class ClothSimulation(ConanFile):
	name = "ClothSimulation"
	version = "0.0.1"
	settings = "cppstd", "os", "compiler", "build_type", "arch"
	generators = "CMakeDeps", "CMakeToolchain"
	requires = "gtest/1.12.1", "glfw/3.3.4", "glm/0.9.9.5", "glad/0.1.35", "benchmark/1.6.2"
