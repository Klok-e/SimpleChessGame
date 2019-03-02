#pragma once

#include "Types.hpp"

namespace Game
{
	class Entity
	{
		Engine::Types::ui32 ind = 0;

		template<typename ComponentTypesTuple, typename SystemsTuple>
		friend class World;
	};

	struct IComponent
	{
		Entity entity;

		using f32 = Engine::Types::f32;
		using f64 = Engine::Types::f64;
		using i32 = Engine::Types::i32;
		using ui32 = Engine::Types::ui32;
	};
}