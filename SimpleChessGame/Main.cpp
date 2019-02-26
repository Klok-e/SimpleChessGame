#include <SFML/Graphics.hpp>
#include "Constants.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
	using namespace Game;

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

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

		auto elapsed = timer.restart();
		if (Consts::MS_PER_FRAME > elapsed)
			sf::sleep(Consts::MS_PER_FRAME - elapsed);
	}
}
