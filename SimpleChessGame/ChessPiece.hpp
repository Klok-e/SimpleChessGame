#pragma once

#include "Types.hpp"

namespace Game
{
	struct ChessPiece
	{
		enum class PieceType
		{
			Undefined = 0,
			Pawn = 1,
			Rook = 2,
			Knight = 3,
			Bishop = 4,
			Queen = 5,
			King = 6,
		} _type = PieceType::Undefined;

		Types::ui32 _id = 0;

		bool _player = false;

		ChessPiece(PieceType type, Types::ui32 id, bool player) :
			_type(type),
			_id(id),
			_player(player)
		{
		}

		ChessPiece()
		{
		}
	};
}