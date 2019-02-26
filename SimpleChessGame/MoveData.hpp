#pragma once

#include "Types.hpp"

namespace Game
{
	struct MoveData
	{
		Types::ui32 toRow = 0;
		Types::ui32 toCol = 0;
		Types::ui32 fromRow = 0;
		Types::ui32 fromCol = 0;

		MoveData()
		{
		}

		MoveData(Types::ui32 toRow, Types::ui32 toCol, Types::ui32 fromRow, Types::ui32 fromCol) :
			toRow(toRow),
			toCol(toCol),
			fromRow(fromRow),
			fromCol(fromCol)
		{
		}
	};
}