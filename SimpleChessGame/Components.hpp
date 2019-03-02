#pragma once

#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "Types.hpp"

namespace Game::Components
{
	struct Renderable :
		public IComponent
	{
		sf::Sprite sprite;
		Renderable(sf::Sprite sprite) :
			sprite(sprite)
		{
		}
	};

	struct Position2D :
		public IComponent
	{
		f32 x, y;

		Position2D(f32 x, f32 y) :
			x(x),
			y(y)
		{
		}
	};
}