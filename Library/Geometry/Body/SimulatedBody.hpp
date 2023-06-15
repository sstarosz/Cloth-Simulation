#ifndef GEOMETRY_BODY_SIMULATEDBODY_HPP
#define GEOMETRY_BODY_SIMULATEDBODY_HPP

#include "BodyBase.hpp"

namespace st::geometry
{

	class SimulatedBody : public BodyBase
	{

	public:
		SimulatedBody() = default;
		SimulatedBody(Vector3 position, Vector3 orientation): BodyBase(position, orientation) {};
		virtual ~SimulatedBody() = default;


		virtual BodyType getBodyType() const override;


	};


};

#endif
