#ifndef CORE_MODELSMENAGER_HPP
#define CORE_MODELSMENAGER_HPP

#include <Geometry/Vector3.hpp>
#include <Geometry/Vertex.hpp>
#include <Geometry/Body/BodyBase.hpp>

#include <vector>
#include <memory>

namespace st::core
{
	using namespace geometry;




	class ModelsMenager
	{

	public:
		ModelsMenager();


		//add model


		//get models for simulation
		//get models for rendering

		//High lever structure that can contain meshes, textures, etc (usually data that come's from 3d format files)
		void addModel(BodyBase&& model);
		
		//Add Geometry that contain only mesh data
		void addGeometry();

		void addResource() {};

		const std::vector<std::unique_ptr<BodyBase>>& getModelsToRender() const
		{
			return m_models;
		}

		std::vector<std::unique_ptr<BodyBase>>& getModelsToSimulate()
		{
			return m_models;
		}

	private:
		// List of all models

		std::vector<std::unique_ptr<BodyBase>> m_models;

		std::vector<std::unique_ptr<BodyBase>> m_modelsToRender;
		std::vector<std::unique_ptr<BodyBase>> m_modelsToSimulate;


	};



	//one class
	//array of vertixes 

	//second class reference to element in vector and meta-data
};

#endif // VIEWPORT_MODELSMENAGER_HPP
