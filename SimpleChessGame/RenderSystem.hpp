#pragma once

#include "ISystem.hpp"
#include "Components.hpp"

namespace Game
{
	class RenderSystem :
		public ISystem<std::false_type, Components::Renderable, Components::Position2D>
	{
	public:
		void Update(delta_t deltaTime, std::tuple<ComponentArray<Components::Renderable>, ComponentArray<Components::Position2D>>& components)
		{
			auto&[rend, pos2d] = components;
			for (auto& pos : pos2d)
			{
				std::cout << pos.x << " " << pos.y << "\n";
			}
		}
	};

	class CoordPrinterSystem :
		public ISystem<std::true_type, Components::Position2D>
	{
	public:
		void Update(ComponentArray<Entity>& entities, delta_t deltaTime, std::tuple<ComponentArray<Components::Position2D>>& components)
		{
			auto&[pos2d] = components;
			for (size_t i = 0; i < entities.ElementCount(); i++)
			{
				std::cout << "x: " << pos2d[i].x << " y: " << pos2d[i].y << " ent id: " << entities[i].GetIndex() << "\n";
			}
		}
	};
}