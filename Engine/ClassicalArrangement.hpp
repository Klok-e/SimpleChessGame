#pragma once

#include <optional>
#include "Array2D.hpp"
#include "ChessPiece.hpp"
#include "Types.hpp"

namespace Engine
{
	template<Types::ui32 rows, Types::ui32 columns>
	class ClassicalArrangement
	{
	public:
		auto Arrange(Array2D<std::optional<ChessPiece>, rows, columns>& state)->void
		{
			Types::ui32 count = 0;
			for (auto[col, row] : state)
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