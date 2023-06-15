#ifndef GEOMETRY_BODY_BODYBASE_HPP
#define GEOMETRY_BODY_BODYBASE_HPP

#include "../Vector3.hpp"
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


		BodyBase(Vector3 position, Vector3 orientation): m_position(position), m_orientation(orientation) {};

		virtual ~BodyBase() = default;
		virtual BodyType getBodyType() const = 0;


		Vector3 m_position;
		Vector3 m_orientation;
	};


};

#endif
