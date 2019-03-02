#pragma once

#include <SFML/Graphics.hpp>
#include "Constants.hpp"

namespace Game
{
	class Resources
	{
	public:
		static auto Inst()->Resources const&
		{
			static Resources i;
			return i;
		}

		Resources& operator=(Resources const&) = delete;
		Resources& operator=(Resources&&) = delete;
		Resources(Resources const&) = delete;
		Resources(Resources&&) = delete;

		//PYTHON_INSERT_MEMBERS{

	public:
		auto get_b_bishop_png_shadow_256px_png()const->sf::Texture const&
		{
			return b_bishop_png_shadow_256px_png;
		}
		auto get_b_king_png_shadow_256px_png()const->sf::Texture const&
		{
			return b_king_png_shadow_256px_png;
		}
		auto get_b_knight_png_shadow_256px_png()const->sf::Texture const&
		{
			return b_knight_png_shadow_256px_png;
		}
		auto get_b_pawn_png_shadow_256px_png()const->sf::Texture const&
		{
			return b_pawn_png_shadow_256px_png;
		}
		auto get_b_queen_png_shadow_256px_png()const->sf::Texture const&
		{
			return b_queen_png_shadow_256px_png;
		}
		auto get_b_rook_png_shadow_256px_png()const->sf::Texture const&
		{
			return b_rook_png_shadow_256px_png;
		}
		auto get_square_brown_dark_png_shadow_256px_png()const->sf::Texture const&
		{
			return square_brown_dark_png_shadow_256px_png;
		}
		auto get_square_brown_light_png_shadow_256px_png()const->sf::Texture const&
		{
			return square_brown_light_png_shadow_256px_png;
		}
		auto get_square_gray_dark__png_shadow_256px_png()const->sf::Texture const&
		{
			return square_gray_dark__png_shadow_256px_png;
		}
		auto get_square_gray_light__png_shadow_256px_png()const->sf::Texture const&
		{
			return square_gray_light__png_shadow_256px_png;
		}
		auto get_w_bishop_png_shadow_256px_png()const->sf::Texture const&
		{
			return w_bishop_png_shadow_256px_png;
		}
		auto get_w_king_png_shadow_256px_png()const->sf::Texture const&
		{
			return w_king_png_shadow_256px_png;
		}
		auto get_w_knight_png_shadow_256px_png()const->sf::Texture const&
		{
			return w_knight_png_shadow_256px_png;
		}
		auto get_w_pawn_png_shadow_256px_png()const->sf::Texture const&
		{
			return w_pawn_png_shadow_256px_png;
		}
		auto get_w_queen_png_shadow_256px_png()const->sf::Texture const&
		{
			return w_queen_png_shadow_256px_png;
		}
		auto get_w_rook_png_shadow_256px_png()const->sf::Texture const&
		{
			return w_rook_png_shadow_256px_png;
		}
	private:
		sf::Texture b_bishop_png_shadow_256px_png;
		sf::Texture b_king_png_shadow_256px_png;
		sf::Texture b_knight_png_shadow_256px_png;
		sf::Texture b_pawn_png_shadow_256px_png;
		sf::Texture b_queen_png_shadow_256px_png;
		sf::Texture b_rook_png_shadow_256px_png;
		sf::Texture square_brown_dark_png_shadow_256px_png;
		sf::Texture square_brown_light_png_shadow_256px_png;
		sf::Texture square_gray_dark__png_shadow_256px_png;
		sf::Texture square_gray_light__png_shadow_256px_png;
		sf::Texture w_bishop_png_shadow_256px_png;
		sf::Texture w_king_png_shadow_256px_png;
		sf::Texture w_knight_png_shadow_256px_png;
		sf::Texture w_pawn_png_shadow_256px_png;
		sf::Texture w_queen_png_shadow_256px_png;
		sf::Texture w_rook_png_shadow_256px_png;
		Resources()
		{
			b_bishop_png_shadow_256px_png.loadFromFile("./Sprites256px/b_bishop_png_shadow_256px.png");
			b_king_png_shadow_256px_png.loadFromFile("./Sprites256px/b_king_png_shadow_256px.png");
			b_knight_png_shadow_256px_png.loadFromFile("./Sprites256px/b_knight_png_shadow_256px.png");
			b_pawn_png_shadow_256px_png.loadFromFile("./Sprites256px/b_pawn_png_shadow_256px.png");
			b_queen_png_shadow_256px_png.loadFromFile("./Sprites256px/b_queen_png_shadow_256px.png");
			b_rook_png_shadow_256px_png.loadFromFile("./Sprites256px/b_rook_png_shadow_256px.png");
			square_brown_dark_png_shadow_256px_png.loadFromFile("./Sprites256px/square brown dark_png_shadow_256px.png");
			square_brown_light_png_shadow_256px_png.loadFromFile("./Sprites256px/square brown light_png_shadow_256px.png");
			square_gray_dark__png_shadow_256px_png.loadFromFile("./Sprites256px/square gray dark _png_shadow_256px.png");
			square_gray_light__png_shadow_256px_png.loadFromFile("./Sprites256px/square gray light _png_shadow_256px.png");
			w_bishop_png_shadow_256px_png.loadFromFile("./Sprites256px/w_bishop_png_shadow_256px.png");
			w_king_png_shadow_256px_png.loadFromFile("./Sprites256px/w_king_png_shadow_256px.png");
			w_knight_png_shadow_256px_png.loadFromFile("./Sprites256px/w_knight_png_shadow_256px.png");
			w_pawn_png_shadow_256px_png.loadFromFile("./Sprites256px/w_pawn_png_shadow_256px.png");
			w_queen_png_shadow_256px_png.loadFromFile("./Sprites256px/w_queen_png_shadow_256px.png");
			w_rook_png_shadow_256px_png.loadFromFile("./Sprites256px/w_rook_png_shadow_256px.png");
		}

		//PYTHON_INSERT_MEMBERS}
	};
}