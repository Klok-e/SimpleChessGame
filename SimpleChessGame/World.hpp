#pragma once

#include <vector>
#include <memory>
#include "ISystem.hpp"

namespace Game::ECS
{
	class World
	{
	public:
		void Update()
		{
			for (size_t i = 0; i < _systems.size();)
			{
				_systems[i]->Update();
			}
		}

		void Build()
		{
			for (size_t i = 0; i < _systems.size(); i++)
			{
				_systems[i]->OnCreate();
			}
		}

		World& AddSystem(std::unique_ptr<ISystem> system)
		{
			_systems.push_back(system);
			return *this;
		}

		World& RegisterComponent()
		{

		}

	private:
		std::vector<std::unique_ptr<ISystem>> _systems;
	};
}