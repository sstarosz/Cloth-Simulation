#include "SimulationEngine.hpp"

#include "Geometry/Vector3.hpp"

#include <vector>

namespace st::simulation
{

	using namespace geometry;


	SimulationEngine::SimulationEngine() { }

	void SimulationEngine::initialize()
	{
		//Generate sphere

		//Generate Plane(Cloth)


		m_simulationParams.particleCountX = 1000;
		m_simulationParams.particleCountY = 1000;

	
	}

	void SimulationEngine::update(float deltaD)
	{

		//For every particle
		std::vector<Particle> particleIn;
		std::vector<Particle> particleOut;




		for (size_t idX = 0; idX < m_simulationParams.particleCountX; idX++)
		{

			for (size_t idY = 0; idY < m_simulationParams.particleCountY; idY++)
			{

				const size_t index = idY * m_simulationParams.particleCountX + idX;

				Vector3 force = m_simulationParams.gravity * m_simulationParams.particleMass;

				const Vector3 position = particleIn[index].position;
				const Vector3 velocity = particleIn[index].velocity;

				//left
				if (idX > 0)
				{
					force = force + springForce(particleIn[index - 1].position, position, m_simulationParams.restDistH);
				}

				//right
				if (idX < m_simulationParams.particleCountX - 1)
				{
					force = force + springForce(particleIn[index + 1].position, position, m_simulationParams.restDistH);
				}


				// upper
				if (idY < m_simulationParams.particleCountY - 1)
				{
					force = force + springForce(particleIn[index + m_simulationParams.particleCountX].position, position, m_simulationParams.restDistV);
				}

				// lower
				if (idY > 0)
				{
					force = force + springForce(particleIn[index - m_simulationParams.particleCountX].position, position, m_simulationParams.restDistV);
				}


				// upper-left
				if ((idX > 0) && (idY < m_simulationParams.particleCountY - 1))
				{
					force = force + springForce(particleIn[index + m_simulationParams.particleCountX - 1].position, position, m_simulationParams.restDistD);
				}

				// lower-left
				if ((idX > 0) && (idY > 0))
				{
					force = force + springForce(particleIn[index - m_simulationParams.particleCountX - 1].position, position, m_simulationParams.restDistD);
				}


				// upper-right
				if ((idX < m_simulationParams.particleCountX - 1) && (idY < m_simulationParams.particleCountY - 1))
				{
					force = force + springForce(particleIn[index + m_simulationParams.particleCountX + 1].position, position, m_simulationParams.restDistD);
				}

				// lower-right
				if ((idX < m_simulationParams.particleCountX - 1) && (idY > 0))
				{
					force = force + springForce(particleIn[index - m_simulationParams.particleCountX + 1].position, position, m_simulationParams.restDistD);
				}


				force = force + (velocity * m_simulationParams.damping);


				// Integrate
				const Vector3 f = force * (1.0 / m_simulationParams.particleMass);
				particleOut[index].position = Vector3 { position + velocity * deltaD + 0.5F * f * deltaD * deltaD };
				particleOut[index].velocity = Vector3 { velocity + f * deltaD };


				// Sphere collision
				const Vector3 sphereDist = particleOut[index].position - m_simulationParams.spherePosition;
				if (Vector3::lenght(sphereDist) < m_simulationParams.sphereRadius + 0.01)
				{
					// If the particle is inside the sphere, push it to the outer radius
					particleOut[index].position = m_simulationParams.spherePosition + Vector3::normalize(sphereDist) * (m_simulationParams.sphereRadius + 0.01);
					// Cancel out velocity
					particleOut[index].velocity = Vector3 {};
				}


				// Normals
				Vector3 normal {};

				Vector3 a;
				Vector3 b;
				Vector3 c;

				if (idY > 0)
				{
					if (idX > 0)
					{
						a = particleIn[index - 1].position - position;
						b = particleIn[index - m_simulationParams.particleCountX - 1].position - position;
						c = particleIn[index - m_simulationParams.particleCountX].position - position;
						normal = normal + Vector3::crossProduct(a, b) + Vector3::crossProduct(b, c);
					}
					if (idX < m_simulationParams.particleCountX - 1)
					{
						a = particleIn[index - m_simulationParams.particleCountX].position - position;
						b = particleIn[index - m_simulationParams.particleCountX + 1].position - position;
						c = particleIn[index + 1].position - position;
						normal = normal + Vector3::crossProduct(a, b) + Vector3::crossProduct(b, c);
					}
				}
				if (idY < m_simulationParams.particleCountY - 1)
				{
					if (idX > 0)
					{
						a = particleIn[index + m_simulationParams.particleCountX].position - position;
						b = particleIn[index + m_simulationParams.particleCountX - 1].position - position;
						c = particleIn[index - 1].position - position;
						normal = normal + Vector3::crossProduct(a, b) + Vector3::crossProduct(b, c);
					}
					if (idX < m_simulationParams.particleCountX - 1)
					{
						a = particleIn[index + 1].position - position;
						b = particleIn[index + m_simulationParams.particleCountX + 1].position - position;
						c = particleIn[index + m_simulationParams.particleCountX].position - position;
						normal = normal + Vector3::crossProduct(a, b) + Vector3::crossProduct(b, c);
					}
				}

				particleOut[index].normal = Vector3(Vector3::normalize(normal));
				
			}
		}



	}

	Vector3 SimulationEngine::springForce(Vector3 firstParticle, Vector3 secondParticle, float restDist)
	{

		Vector3 distant = firstParticle - secondParticle;

		return Vector3::normalize(distant) * m_simulationParams.springStiffness * (Vector3::lenght(distant) - restDist);
	}


} //!namespace st::simulation