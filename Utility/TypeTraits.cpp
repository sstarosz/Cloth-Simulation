#include <type_traits>
#include <iostream>



struct Vec3
{
		float X;
		float Y;
		float Z;
};

static_assert(sizeof(Vec3) == 12, "Vector3 need to be 12 byte in size");

static_assert(alignof(Vec3) == 4, "Vector3 must be word-aligned");

//static_assert(std::is_floating_point_v <Vec3>, "Vector3 must be a floating type")


static_assert(std::is_trivial_v<Vec3>, "Vector3 must be trivial ");
static_assert(std::is_trivially_copyable_v<Vec3>, "Vector3 must be trivial copyable");
static_assert(std::is_standard_layout_v<Vec3>, "Vector3 must be standard layout");


static_assert(offsetof(Vec3, Vec3::X) == 0, "X Member must be at offset 0 within Vector3");
static_assert(offsetof(Vec3, Vec3::Y) == 4, "Y Member must be at offset 4 within Vector3");
static_assert(offsetof(Vec3, Vec3::Z) == 8, "Z Member must be at offset 8 within Vector3");

//static_assert(std::has_unique_object_representations_v<Vec3>,"Vector3 must have unique object represenations (No pading)");

int main() {

	std::cout << std::has_unique_object_representations_v<Vec3> << std::endl;
	std::cout << alignof(Vec3) << std::endl;
	return 0;
}