#pragma once

#include <SFML/Main.hpp>

namespace Game::Consts
{
	constexpr double FPS = 60;

	const sf::Time MS_PER_FRAME = sf::microseconds((1. / FPS) * 1000000);
}