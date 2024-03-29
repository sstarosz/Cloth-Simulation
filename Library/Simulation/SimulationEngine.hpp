
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
		SimulationEngine(viewport::ModelsMenager& modelMenager);


		void initialize();
		void updateRecourses();
		void releaseResources();


		void update(float deltaTime);
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
			Vector3 acceleration;
			float radius;
			float mass;
		};


		//std::vector<Vector3> m_particlesPositions;
		//std::vector<Vector3> m_particlesVelocity;
		//std::vector<Vector3> m_particlesUv;



		private:
		void applyForce(Particle& particle, const Vector3 force);
		void updateMesh(Particle& particle, float deltaTime);

		std::vector<Particle> m_particle;

		viewport::ModelsMenager& m_modelMenager;
	};

} //!namespace st::simulation
#endif // !SIMULATION_SIMULATIONENGINE_HPP
