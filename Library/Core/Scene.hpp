#ifndef CORE_SCENE_HPP
#define CORE_SCENE_HPP

#include <string>
#include <vector>

#include <Geometry/Body/BodyBase.hpp>

namespace st::core
{

class Scene
{
public:
	void initialize();


private:
	
	std::vector<geometry::BodyBase> m_bodies;
    std::string name;


};

}




#endif // !CORE_SCENESTATUS_HPP