#pragma once

#include <optional>
#include <tuple>
#include <vector>
#include "ChessPiece.hpp"
#include "Array2D.hpp"
#include "ClassicalChessRules.hpp"
#include "AvailableMoves.hpp"
#include "ClassicalArrangement.hpp"

namespace Game
{
	template<Types::ui32 rows, Types::ui32 columns,
		template<Types::ui32 rows, Types::ui32 columns> class Rules,
		template<Types::ui32 rows, Types::ui32 columns> class Arrangement>
	class Board
	{
	public:
		Board()
		{
			Arrangement<rows, columns> arrangement;
			arrangement.Arrange(_state);
		}

		Board(Array2D<std::optional<ChessPiece>, rows, columns> const& state) :
			_state(state)
		{
		}

		Array2D<std::optional<ChessPiece>, rows, columns> const& GetState()
		{
			return _state;
		}

		std::vector<ChessPiece> const& DoTurn(Types::ui32 moveInd)
		{
			_playersTurn = !_playersTurn;

			auto move = _currentPlayerMoves.data[moveInd];

			// capture if there's anything
			if (_state.at(move.toRow, move.toCol))
				_wereCaptured.push_back(_state.at(move.toRow, move.toCol).value());

			// move piece
			_state.at(move.toRow, move.toCol) = _state.at(move.fromRow, move.fromCol);

			// remove piece from previous position
			_state.at(move.fromRow, move.fromCol).reset();

			return _wereCaptured;
		}

		AvailableMoves const& ConstructAvailableMovesForTurn()
		{
			_rules.GetAvailableMovesForTurn(_currentPlayerMoves, _playersTurn, _state);
			return _currentPlayerMoves;
		}

	private:
		Array2D<std::optional<ChessPiece>, rows, columns> _state;

		Rules<rows, columns> _rules;

		AvailableMoves _currentPlayerMoves;
		std::vector<ChessPiece> _wereCaptured;

		MoveData _currentPlayerMove;

		bool _playersTurn = false;
	};
}