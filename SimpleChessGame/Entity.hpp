#pragma once

#include "Types.hpp"

namespace Game
{
	class Entity
	{
		Engine::Types::u32 ind = 0;

	public:
		Entity(Engine::Types::u32 ind) :
			ind(ind)
		{
		}

		Engine::Types::u32 GetIndex()
		{
			return ind;
		}

		bool operator==(Entity const& other)const
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