#include <type_traits>
#include <iostream>
#include <string>
#include <vector>



class ResourcePool
{
public:
	ResourcePool(uint32_t poolSize) noexcept: m_poolSize(poolSize) 
	{}


	void allocateSet() const;


	private:
	uint32_t m_poolSize;
};



void simulate()
{
	// Simulation have to check if mesh is simulated or not

	// Have access to topology (vertex position)


}


void render()
{



	// Vertex Buffer
	// 
	// In vertex shader one single vertex have memory layout 
		//layout(location = 0) in vec3 inPosition;
		//layout(location = 1) in vec2 inTexCoord;
		//layout(location = 2) in vec3 inColor;
		//layout(location = 3) in vec3 inNormal;
		// 
		// vertex buffer have to stick to this memory layout
		// 
		// 
		// Global uniform buffer
		// Uniform per buffer
	//	
	// 
	// 
	// 
	// 
		// create Vertex Buffer when new mesh is added
		// get Vertex Buffer when is rendered
		// remove Vertex Buffer when is not longer needed


	// Index Buffer

	// Descriptor

	//

}



int main()
{

	/*[2], [5], [6], [0], [4]*/
	const uint32_t numberOfSteps { 3 };
	std::vector<float> m_points;


	float startValue{ -0.5 };
	float dx { 1.0 / 3.0 };

	for (size_t i = 0; i <= numberOfSteps; i++)
	{
		const float value = startValue + dx * i;
		m_points.push_back(value);
	}


	 for (auto const& i : m_points)
	{
		std::cout << std::round(i * 1000.0f) / 1000.0f << " ";
	}

	
	return 0;
}