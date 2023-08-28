#ifndef GEOMETRY_BODY_STATICBODY_HPP
#define GEOMETRY_BODY_STATICBODY_HPP

#include "BodyBase.hpp"

namespace st::geometry
{

	class StaticBody : public BodyBase
	{

	public:
		StaticBody(math::Vector3 position, math::Vector3 orientation): BodyBase(position, orientation) {};
		virtual ~StaticBody() = default;


		virtual BodyType getBodyType() const override;
	};


};



#endif