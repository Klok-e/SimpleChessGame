#pragma once

#include <optional>
#include <tuple>
#include <vector>
#include "ChessPiece.hpp"
#include "Array2D.hpp"
#include "ClassicalChessRules.hpp"
#include "AvailableMoves.hpp"
#include "ClassicalArrangement.hpp"

namespace Engine
{
	template<Types::ui32 rows, Types::ui32 columns,
		template<Types::ui32 rows, Types::ui32 columns> class Rules,
		template<Types::ui32 rows, Types::ui32 columns> class Arrangement>
	class Board
	{
	public:
		Board() :
			_state()
		{
			Arrangement<rows, columns> arrangement;
			arrangement.Arrange(_state);
		}

		Board(Array2D<std::optional<ChessPiece>, rows, columns> const& state) :
			_state(state)
		{
		}

		auto GetState()->Array2D<std::optional<ChessPiece>, rows, columns> const&
		{
			return _state;
		}

		auto DoTurn(Types::ui32 moveInd)->std::optional<ChessPiece>
		{
			_playersTurn = !_playersTurn;

			auto move = _currentPlayerMoves.data[moveInd];

			// move piece
			_state.at(move.ToRow(), move.ToCol()) = _state.at(move.FromRow(), move.FromCol());

			// remove piece from previous position
			_state.at(move.FromRow(), move.FromCol()).reset();

			// capture if there's anything
			std::optional<ChessPiece> capt;
			if (move.Capt())
			{
				capt = _state.at(move.Capt().value());
				_state.at(move.Capt().value()).reset();
			}

			return capt;
		}

		auto ConstructAvailableMovesForTurn()->AvailableMoves const&
		{
			_rules.GetAvailableMovesForTurn(_currentPlayerMoves, _playersTurn, _state);
			return _currentPlayerMoves;
		}

	private:
		Array2D<std::optional<ChessPiece>, rows, columns> _state;

		Rules<rows, columns> _rules;

		AvailableMoves _currentPlayerMoves;

		MoveData _currentPlayerMove;

		bool _playersTurn = false;
	};
}