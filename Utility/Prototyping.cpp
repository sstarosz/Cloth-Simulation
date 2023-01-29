#include <type_traits>
#include <iostream>
#include <string>


class Boo
{
		uint32_t idx;
		std::string name;
};

class Foo
{
public:
		Foo(const Boo& boo):
		m_boo(boo) {

			};


private:

	const Boo& m_boo;

};



int main()
{
	Boo b;
	Foo a { b };

	
	return 0;
}