#include "SimulationEngine.hpp"

#include "Geometry/Vector3.hpp"

#include <vector>

namespace st::simulation
{

	using namespace geometry;


	SimulationEngine::SimulationEngine(viewport::ModelsMenager& modelMenager): m_modelMenager(modelMenager)
	{ }

	void SimulationEngine::initialize()
	{
		//Generate sphere

	}




	void SimulationEngine::update(float deltaTime)
	{
		auto& modelToRender = m_modelMenager.getModelsToSimulate().at(1);


		//Update particles
		if (m_particle.size() != modelToRender.m_mesh.m_vertices.size())
		{
			for (const auto& vertex : modelToRender.m_mesh.m_vertices)
			{ 
				m_particle.emplace_back( Particle{
					vertex.m_pos,
					{ 0.0f,  0.0f, 0.0f},
					{ 0.0f, 0.0f, 0.0f},
					0.1f,
					1.0f
                });
			}
		}
		else
		{
			for (size_t i = 0; i < modelToRender.m_mesh.m_vertices.size(); i++)
			{
				m_particle.at(i).position = modelToRender.m_mesh.m_vertices[i].m_pos;
			}	
		}


		//Updatet simulation


		for ( auto& particle : m_particle)
		{
			updateMesh(particle, deltaTime);
		}


		for (size_t i = 0; i < modelToRender.m_mesh.m_vertices.size(); i++)
		{
			modelToRender.m_mesh.m_vertices[i].m_pos = m_particle.at(i).position;
			//modelToRender.m_mesh.m_vertices[i].m_normal = m_particle.at(i).normal;
		}



	}

	void SimulationEngine::applyForce(Particle& particle, const Vector3 force)
	{
		particle.acceleration = particle.acceleration + force / particle.mass;
	}


	void SimulationEngine::updateMesh(Particle& particle, float deltaTime)
	{
		const float Gravity = 0.1f;
		const float Ground_Level = 0.0f;

		// Apply gravity
		const Vector3 gravity(0, -Gravity * particle.mass, 0);
		applyForce(particle,gravity);

		// Update velocity
		particle.velocity = particle.velocity + particle.acceleration * deltaTime;
		particle.position = particle.position + particle.velocity * deltaTime;

		// Reset acceleration
		particle.acceleration = Vector3(0, 0, 0);

		// Check for collision with the ground
		if (particle.position.Y - particle.radius < Ground_Level)
		{
			// Calculate the normal force and friction
			const Vector3 normal(0, 1, 0);
			Vector3 friction = particle.velocity - Vector3::dotProduct(particle.velocity, normal) * normal;
			const double coefficientOfRestitution = 0.8; // The "bounciness" of the ball

			// Apply the collision response
			const Vector3 impulse = -2.0 * Vector3::dotProduct(particle.velocity, normal) * normal;
			particle.velocity = particle.velocity + impulse;
			particle.velocity *= coefficientOfRestitution;

			// Apply friction
			friction *= particle.mass * Gravity;
			applyForce(particle ,- friction);

			// Move the ball out of the ground
			particle.position.Y = Ground_Level + particle.radius;
		}
	}


} //!namespace st::simulation