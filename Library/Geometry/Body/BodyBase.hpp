#ifndef GEOMETRY_BODY_BODYBASE_HPP
#define GEOMETRY_BODY_BODYBASE_HPP

#include "Math/Math.hpp"
#include "../Primitives/ShapeBase.hpp"

namespace st::geometry
{

	class BodyBase
	{

	public:
		enum class BodyType
		{
			eStaticBody,
			eSimulatedBody
		};


		BodyBase(math::Vector3 position, math::Vector3 orientation): m_position(position), m_orientation(orientation) {};

		virtual ~BodyBase() = default;
		virtual BodyType getBodyType() const = 0;


		math::Vector3 m_position;
		math::Vector3 m_orientation;
	};


};

#endif
