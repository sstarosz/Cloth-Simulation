#include <type_traits>
#include <iostream>
#include <string>
#include <vector>
#include <utility>


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


class Object
{
public:
	Object() :
		m_name("")
	{
		std::cout << "Default constructor" << " " << m_name << std::endl;
	}

	Object(std::string name): m_name(name)
	{
		std::cout << "Default constructor" << " " << m_name << std::endl;
	}

	Object(const Object& other): m_name(other.m_name)
	{ 
		std::cout << "Copy constructor" << " " << m_name << std::endl;
	}

	Object(Object&& other): m_name(std::move(other.m_name))
	{
		std::cout << "Move constructor" << " " << m_name << std::endl;
	}

	Object& operator=(const Object& other)
	{
		std::cout << "Copy assignment  constructor" << " " << m_name << std::endl;
		if (this != &other)
			m_name = other.m_name;
		return *this;
	}

	Object& operator=(Object&& other)
	{
		std::cout << "Move assignment  constructor" << " " << m_name << std::endl;
		if (this != &other)
		{
			m_name = std::move(other.m_name);
			other.m_name.clear();
		}
		return *this;
	}


private:
	std::string m_name;

};


class ObjectList
{
public:
	ObjectList()
	{
		m_objectlists.reserve(1000);
	};

	void addObject(Object object)
	{
		std::cout << "Added object that is pass by value using push back" << std::endl;
		m_objectlists.push_back(object);
	}

	void addObjectEmplaceBack(Object object)
	{
		std::cout << "Added object that is pass by value using emplace back" << std::endl;
		m_objectlists.emplace_back(object);
	}

	void addObjectByReference(const Object& object)
	{
		std::cout << "Added object that is pass by value using push back" << std::endl;
		m_objectlists.push_back(object);
	}

	void addObjectByReferenceEmplaceBack(const Object& object)
	{
		std::cout << "Added object that is pass by value using push back" << std::endl;
		m_objectlists.emplace_back(object);
	}


	void addObjectEmplaceBackMove(Object object)
	{
		std::cout << "Added object that is pass by value using push back" << std::endl;
		m_objectlists.emplace_back(std::move(object));
	}

	private:
	std::vector<Object> m_objectlists;
};


int main()
{

	ObjectList objectList;
	objectList.addObject(Object("test1"));


	std::cout << std::endl;
	Object test2("test2");
	objectList.addObject(test2);

	std::cout << std::endl;
	objectList.addObject(Object { "test3" });


	std::cout << std::endl;
	objectList.addObjectEmplaceBack(Object { "test4" });


	std::cout << std::endl;
	objectList.addObjectByReference(Object { "test5" });
	
	std::cout << std::endl;
	Object test6("test6");
	objectList.addObjectByReference(test6);


	std::cout << std::endl;
	objectList.addObjectByReferenceEmplaceBack(Object { "test7" });

	std::cout << std::endl;
	Object test8("test8");
	objectList.addObjectByReferenceEmplaceBack(test8);


	std::cout << std::endl;
	objectList.addObjectEmplaceBackMove(Object { "test9" });

	return 0;
}