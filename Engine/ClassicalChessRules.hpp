#pragma once

#include "AvailableMoves.hpp"
#include <unordered_set>
#include "Vec2.hpp"

namespace Engine
{
	template<Types::ui32 rows, Types::ui32 columns>
	class ClassicalChessRules
	{
	public:
		auto GetAvailableMovesForTurn(AvailableMoves& toFill, bool player,
									  Array2D<std::optional<ChessPiece>, rows, columns> const& state)->void
		{
			using namespace Types;
			typedef ChessPiece::PieceType tp;

			for (auto[col, row] : state)
			{
				if (auto piece = state.at(row, col))
					if (player == piece->_player)
					{
						i32 forward = player ? -1 : 1;
						switch (piece->_type)
						{
							case tp::Bishop:
								break;
							case tp::King:
								break;
							case tp::Knight:
								break;
							case tp::Pawn:
								// move forward
								if (CoordsOnBoard(row + forward, col)
									&&
									!state.at(row + forward, col))
									toFill.data.push_back(MoveData(Vec2(row + forward, col),
																   Vec2(row, col)));

								// attack left forward and right forward
								for (i32 i = -1; i <= 1; i += 2)
									if (CoordsOnBoard(row + forward, col + i))
										if (auto nextPiece = state.at(row + forward, col + i))
											if (nextPiece->_player != player)
												toFill.data.push_back(MoveData(Vec2(row + forward, col + i),
																			   Vec2(row, col),
																			   Vec2(row + forward, col + i)));

								// move forward 2 tiles
								if (CoordsOnBoard(row + 2 * forward, col)
									&&
									!state.at(row + forward, col)// nothing 1 tile forward
									&&
									!state.at(row + 2 * forward, col)// nothing 2 tiles forward
									&&
									_pawnsThatDidFirstMove.count(piece->_id) == 0)// this pawn didn't do it's first move
								{
									toFill.data.push_back(MoveData(Vec2(row + 2 * forward, col),
																   Vec2(row, col)));
									_pawnsThatDidFirstMove.insert(piece->_id);
								}
								break;
							case tp::Queen:
								break;
							case tp::Rook:
								break;
							default:
								std::terminate();
								break;
						}
					}
			}
			_turnsFinished += 1;
		}

		auto IsGameFinished(Array2D<std::optional<ChessPiece>, rows, columns> const& state)->bool
		{
			Types::ui32 p1 = 0, p2 = 0;
			for (Types::ui32 i = 0; i < rows*columns; i++)
				if (auto piece = state[i])
					(piece->_player ? p1 : p2) += 1;
			if (p1 == 0
				||
				p2 == 0)
				return true;
			else
				return false;
		}

	private:
		Types::ui32 _turnsFinished = 0;

		std::unordered_set<Types::ui32> _pawnsThatDidFirstMove;

		constexpr auto CoordsOnBoard(Types::ui32 row, Types::ui32 col) const noexcept -> bool
		{
			return row < rows && col < columns;
		}
	};
}