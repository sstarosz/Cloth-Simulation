#ifndef CORE_MODELSMANAGER_HPP
#define CORE_MODELSMANAGER_HPP

#include "Math/Math.hpp"
#include <Geometry/Vertex.hpp>
#include <Geometry/Model.hpp>

#include <vector>
#include <memory>

namespace st::core
{
	using namespace geometry;




	class ModelsManager
	{

	public:
		ModelsManager();


		//add model


		//get models for simulation
		//get models for rendering

		//High lever structure that can contain meshes, textures, etc (usually data that come's from 3d format files)
		void addModel(Model&& model);
		
		//Add Geometry that contain only mesh data
		void addGeometry();

		void addResource() {};

		const std::vector<std::unique_ptr<Model>>& getModelsToRender() const
		{
			return m_models;
		}

		std::vector<std::unique_ptr<Model>>& getModelsToSimulate()
		{
			return m_models;
		}

	private:
		// List of all models

		std::vector<std::unique_ptr<Model>> m_models;

		std::vector<std::unique_ptr<Model>> m_modelsToRender;
		std::vector<std::unique_ptr<Model>> m_modelsToSimulate;


	};



	//one class
	//array of vertixes 

	//second class reference to element in vector and meta-data
};

#endif // VIEWPORT_MODELSMENAGER_HPP
