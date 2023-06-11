#include "StaticBody.hpp"


namespace st::geometry
{


	BodyBase::BodyType StaticBody::getBodyType() const
	{
		return BodyType::eSimulatedBody;
	}
}

