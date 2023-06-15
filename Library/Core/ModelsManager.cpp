#include "ModelsManager.hpp"

#include <vector>
#include <numbers>
#include <cmath>

namespace st::core
{

	ModelsManager::ModelsManager()
	{

	}

	void ModelsManager::addModel(Model&& model)
	{ 
		m_models.emplace_back(std::make_unique <Model>(std::move(model)));
	}


}