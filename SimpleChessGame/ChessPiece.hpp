#pragma once

#include "Rect.hpp"

namespace Game
{
	class ChessPiece
	{
	public:
		enum class PieceType
		{
			Pawn = 0,
			Rook = 1,
			Knight = 2,
			Bishop = 3,
			Queen = 4,
			King = 5,
		} const _type;

		ChessPiece(PieceType type) :
			_type(type)
		{
		}

		void GetMoves(Vector2<int> pos)
		{
			switch (_type)
			{
				case PieceType::Pawn:
					break;
				case PieceType::Rook:
					break;
				case PieceType::Knight:
					break;
				case PieceType::Bishop:
					break;
				case PieceType::Queen:
					break;
				case PieceType::King:
					break;
			}
		}

	private:

	};
}