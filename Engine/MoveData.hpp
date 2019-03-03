#pragma once

#include <optional>
#include "Types.hpp"
#include "Vec2.hpp"

namespace Engine
{
	struct MoveData
	{
	public:
		MoveData()
		{
		}

		MoveData(Types::u32 ToRow, Types::u32 ToCol, Types::u32 FromRow, Types::u32 FromCol) :
			_to(ToCol, ToRow),
			_from(FromCol, FromRow)
		{
		}

		MoveData(Types::u32 ToRow, Types::u32 ToCol, Types::u32 FromRow, Types::u32 FromCol, Types::u32 CaptRow, Types::u32 CaptCol) :
			_to(ToCol, ToRow),
			_from(FromCol, FromRow),
			_captured(std::in_place, CaptRow, CaptCol)
		{
		}

		MoveData(Vec2<Types::u32> _to, Vec2<Types::u32> _from) :
			_to(_to),
			_from(_from)
		{
		}

		MoveData(Vec2<Types::u32> _to, Vec2<Types::u32> _from, Vec2<Types::u32> Capt) :
			_to(_to),
			_from(_from),
			_captured(std::in_place, Capt)
		{
		}

		auto To()->Vec2<Types::u32>
		{
			return _to;
		}

		auto From()->Vec2<Types::u32>
		{
			return _from;
		}

		auto Capt()->std::optional<Vec2<Types::u32>>
		{
			return _captured;
		}

		auto ToCol()->Types::u32
		{
			return _to.x;
		}

		auto ToRow()->Types::u32
		{
			return _to.y;
		}

		auto FromCol()->Types::u32
		{
			return _from.x;
		}

		auto FromRow()->Types::u32
		{
			return _from.y;
		}

		auto CaptCol()->std::optional<Types::u32>
		{
			return _captured ? _captured->x : std::make_optional<Types::u32>();
		}

		auto CaptRow()->std::optional<Types::u32>
		{
			return _captured ? _captured->y : std::make_optional<Types::u32>();
		}

	private:
		Vec2<Types::u32> _to;
		Vec2<Types::u32> _from;

		std::optional<Vec2<Types::u32>> _captured;
	};
}