#include "SimulatedBody.hpp"


namespace st::geometry
{


	BodyBase::BodyType SimulatedBody::getBodyType() const
	{
		return BodyType::eSimulatedBody;
	}

}