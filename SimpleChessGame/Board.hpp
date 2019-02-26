#pragma once

#include <optional>
#include "ChessPiece.hpp"
#include "Array2D.hpp"

namespace Game
{
	template<size_t rows, size_t columns>
	class Board
	{
	public:
		Board()
		{
		}
		Board(Array2D<std::optional<ChessPiece>, rows, columns> state) :
			_state(state)
		{
		}

	private:
		Array2D<std::optional<ChessPiece>, rows, columns> _state;
	};
}