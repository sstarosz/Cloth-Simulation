#ifndef GEOMETRY_BODY_BODYBASE_HPP
#define GEOMETRY_BODY_BODYBASE_HPP

#include "../Vector3.hpp"
#include "../Primitives/ShapeBase.hpp"

namespace st::geometry
{

	class BodyBase
	{

	protected:
		enum class BodyType
		{
			eStaticBody,
			eSimulatedBody
		};

	public:
		virtual ~BodyBase() = default;
		virtual BodyType getBodyType() const = 0;


	protected:
		Vector3 m_position;
		Vector3 m_orientation;
	};


};

#endif
