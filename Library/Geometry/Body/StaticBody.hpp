#ifndef GEOMETRY_BODY_STATICBODY_HPP
#define GEOMETRY_BODY_STATICBODY_HPP

#include "BodyBase.hpp"

namespace st::geometry
{

	class StaticBody : public BodyBase
	{

	public:
		StaticBody(Vector3 position, Vector3 orientation): BodyBase(position, orientation) {};
		virtual ~StaticBody() = default;


		virtual BodyType getBodyType() const override;
	};


};



#endif