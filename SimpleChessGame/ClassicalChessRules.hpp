#pragma once

#include "AvailableMoves.hpp"
#include <unordered_set>

namespace Game
{
	template<Types::ui32 rows, Types::ui32 columns>
	class ClassicalChessRules
	{
	public:
		void GetAvailableMovesForTurn(AvailableMoves& toFill, bool player,
									  Array2D<std::optional<ChessPiece>, rows, columns> const& state)
		{
			using namespace Types;

			toFill.data.clear();

			for (ui32 row = 0; row < rows; row++)
				for (ui32 col = 0; col < columns; col++)
				{
					if (auto piece = state.at(row, col))
						if (player == piece->_player)
						{
							typedef ChessPiece::PieceType type;

							i32 forward = player ? 1 : -1;
							switch (piece->_type)
							{
								case type::Bishop:
									break;
								case type::King:
									break;
								case type::Knight:
									break;
								case type::Pawn:
									// move forward
									if (CoordsOnBoard(row, col + forward)
										&&
										!state.at(row, col + forward))
										toFill.data.push_back(MoveData
															  {
																  row, col + forward,
																  row, col
															  });
									// attack left forward and right forward
									for (i32 i = -1; i <= 1; i += 2)
										if (CoordsOnBoard(row + i, col + forward))
											if (auto nextPiece = state.at(row + i, col + forward))
												if (nextPiece->_player != player)
													toFill.data.push_back(MoveData
																		  {
																			  row + i, col + forward,
																			  row, col
																		  });
									// move forward 2 tiles
									if (CoordsOnBoard(row, col + 2 * forward)
										&&
										!state.at(row, col + forward)// nothing 1 tile forward
										&&
										!state.at(row, col + 2 * forward)// nothing 2 tiles forward
										&&
										_pawnsThatDidFirstMove.count(piece->_id) == 0)// this pawn didn't do it's first move
									{
										toFill.data.push_back(MoveData
															  {
																  row, col + 2 * forward,
																  row, col
															  });
										_pawnsThatDidFirstMove.insert(piece->_id);
									}
									break;
								case type::Queen:
									break;
								case type::Rook:
									break;
								default:
									std::terminate();
									break;
							}
						}
				}
			_turnsFinished += 1;
		}

		bool IsGameFinished(Array2D<std::optional<ChessPiece>, rows, columns> const& state)
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

		bool CoordsOnBoard(Types::ui32 row, Types::ui32 col) const noexcept
		{
			return row < rows && col < columns;
		}
	};
}