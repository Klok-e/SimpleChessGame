#define DOCTEST_CONFIG_IMPLEMENT

#ifndef DOCTEST_CONFIG_IMPLEMENT
#define DOCTEST_CONFIG_DISABLE
#endif

#include <doctest/doctest.h>

#include <SFML/Graphics.hpp>
#include "Constants.hpp"
#include "Board.hpp"
#include "ClassicalArrangement.hpp"
#include <iostream>
#include "Resources.hpp"
#include "World.hpp"
#include "Components.hpp"
#include "RenderSystem.hpp"
#include "ISystem.hpp"

auto main(int argc, char* argv[])->int
{
#pragma region Testing

	doctest::Context context;

	context.setOption("no-breaks", true);

	int res = context.run();

	if (context.shouldExit())
		return res;

#pragma endregion

	using namespace Engine;
	using namespace Game;
	try
	{
		sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
		window.setFramerateLimit(Consts::FPS);

		sf::CircleShape shape(100.f);
		shape.setFillColor(sf::Color::Green);

		Board<12, 12, ClassicalChessRules, ClassicalArrangement> mainBoard;
		mainBoard.ConstructAvailableMovesForTurn();
		mainBoard.DoTurn(0);
		mainBoard.GetState();

		//RenderSystem x;
		//decltype(x)::componentsRequired;

		World<std::tuple<Components::Renderable, Components::Position2D>,
			std::tuple<RenderSystem>> world;

		world.CreateEntity(Components::Renderable(sf::Sprite(Resources::Inst().get_b_bishop_png_shadow_256px_png())));

		world.Update();

		sf::Clock timer;
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear();
			//window.draw(x);
			window.display();
		}
	}
	catch (std::exception e)
	{
		std::cerr << e.what();
		return -1;
	}
}
