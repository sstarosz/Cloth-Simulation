#include <type_traits>
#include <iostream>
#include <string>




class ResourcePool
{
public:
	ResourcePool(uint32_t poolSize) noexcept: m_poolSize(poolSize) 
	{}


	void allocateSet() const;


	private:
	uint32_t m_poolSize;
};





int main()
{

	/*[2], [5], [6], [0], [4]*/



	
	return 0;
}