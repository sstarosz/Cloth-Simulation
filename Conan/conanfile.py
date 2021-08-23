from conans import ConanFile, Cmake


class ClothSimulation(ConanFile):
	name = "ClothSimulation"
	version = "0.0.1"
	settings = "os", "arch", "compiler", "build_type"
    generators = "cmake"

    requires = "gtest/1.8.1"