from conans import ConanFile, CMake


class ClothSimulation(ConanFile):
	name = "ClothSimulation"
	version = "0.0.1"
	settings = "cppstd", "os", "compiler", "build_type", "arch"
	generators = "CMakeDeps", "CMakeToolchain"
	requires = "gtest/1.10.0"
