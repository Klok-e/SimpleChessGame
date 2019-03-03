#pragma once

#include "Types.hpp"

namespace Game
{
	class Entity
	{
		Engine::Types::u32 ind = 0;

		template<typename ComponentTypesTuple, typename SystemsTuple>
		friend class World;

	public:
		Entity(Engine::Types::u32 ind) :
			ind(ind)
		{
		}

		bool operator==(Entity const& other)
		{
			return ind == other.ind;
		}
	};

	struct IComponent
	{
		using f32 = Engine::Types::f32;
		using f64 = Engine::Types::f64;
		using i32 = Engine::Types::i32;
		using u32 = Engine::Types::u32;
	};
}