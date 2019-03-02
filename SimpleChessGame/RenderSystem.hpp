#pragma once

#include "ISystem.hpp"
#include "Components.hpp"

namespace Game
{
	class RenderSystem :
		public ISystem<std::false_type, Components::Renderable&, Components::Position2D&>
	{
	public:
		void Update(delta_t deltaTime, std::tuple<Components::Renderable&, Components::Position2D&> components)
		{

		}
	};
}