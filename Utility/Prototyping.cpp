#include <type_traits>
#include <iostream>
#include <string>


class Boo
{
public:
		uint32_t idx;
		std::string name;
};

typedef struct Boo* myBoo;



int main()
{
	myBoo b = {};

	b->idx;

	
	return 0;
}