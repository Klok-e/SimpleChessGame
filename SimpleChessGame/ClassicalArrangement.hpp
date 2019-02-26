#pragma once

#include <optional>
#include "Array2D.hpp"
#include "ChessPiece.hpp"

namespace Game
{
	template<Types::ui32 rows, Types::ui32 columns>
	class ClassicalArrangement
	{
	public:
		void Arrange(Array2D<std::optional<ChessPiece>, rows, columns> state)
		{
			Types::ui32 count = 0;
			for (Types::ui32 col = 0; col < columns; col++)
				for (Types::ui32 row = 0; row < rows; row++)
				{
					switch (row)
					{
						case 0:
							state.at(row, col) = std::make_optional<ChessPiece>(ChessPiece::PieceType::Pawn, count++, false);
							break;
						case 1:
							state.at(row, col) = std::make_optional<ChessPiece>(ChessPiece::PieceType::Pawn, count++, false);
							break;
						case rows - 1:
							state.at(row, col) = std::make_optional<ChessPiece>(ChessPiece::PieceType::Pawn, count++, true);
							break;
						case rows - 2:
							state.at(row, col) = std::make_optional<ChessPiece>(ChessPiece::PieceType::Pawn, count++, true);
							break;
					}
				}
		}
	};
}