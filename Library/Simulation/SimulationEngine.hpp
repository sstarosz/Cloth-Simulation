
#ifndef SIMULATION_SIMULATIONENGINE_HPP
#define SIMULATION_SIMULATIONENGINE_HPP


#include <Geometry/Vector4.hpp>
#include <Geometry/Vector3.hpp>


namespace st::simulation
{
	using namespace geometry;

	class SimulationEngine 
	{

	public:
		SimulationEngine();


		void initialize();
		void releaseResources();


		void update(float deltaD);


		struct SimulationParams
		{
			float particleMass;
			float springStiffness;
			float damping;
			float restDistH;
			float restDistV;
			float restDistD;
			float sphereRadius;
			Vector3 spherePosition;
			Vector3 gravity;
			uint32_t particleCountX;
			uint32_t particleCountY;	
		};

		SimulationParams m_simulationParams;

		struct Particle
		{
			Vector3 position;
			Vector3 velocity;
			Vector3 uv;
			Vector3 normal;
			float pinned;

		};

		Particle m_particle;


		private:
		Vector3 springForce(Vector3 firstParticle, Vector3 secondParticle, float restDist);
	};

} //!namespace st::simulation
#endif // !SIMULATION_SIMULATIONENGINE_HPP
