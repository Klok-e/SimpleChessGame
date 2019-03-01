#include <SFML/Graphics.hpp>
#include "Constants.hpp"
#include "Board.hpp"
#include "ClassicalArrangement.hpp"
#include <iostream>

auto main(int argc, char* argv[])->int
{
	using namespace Engine;
	using namespace Game;

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	window.setFramerateLimit(Consts::FPS);

	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	Board<12, 12, ClassicalChessRules, ClassicalArrangement> mainBoard;
	mainBoard.ConstructAvailableMovesForTurn();
	mainBoard.DoTurn(0);
	mainBoard.GetState();

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
		window.draw(shape);
		window.display();
	}
}
