
#ifndef SIMULATION_SIMULATIONENGINE_HPP
#define SIMULATION_SIMULATIONENGINE_HPP


#include <Geometry/Vector4.hpp>
#include <Geometry/Vector3.hpp>
#include <Viewport/ModelsMenager.hpp>

namespace st::simulation
{
	using namespace geometry;

	class SimulationEngine 
	{

	public:
		SimulationEngine(const viewport::ModelsMenager& modelMenager);


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


		std::vector<Vector3> m_particlesPositions;
		std::vector<Vector3> m_particlesVelocity;
		std::vector<Vector3> m_particlesUv;



		Particle m_particle;


		private:
		Vector3 springForce(Vector3 firstParticle, Vector3 secondParticle, float restDist);


		const viewport::ModelsMenager& m_modelMenager;
	};

} //!namespace st::simulation
#endif // !SIMULATION_SIMULATIONENGINE_HPP
