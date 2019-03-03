#pragma once

#include <array>
#include <iterator>
#include "Vec2.hpp"
#include "Types.hpp"

namespace Engine
{
	template<class T, Types::u32 rows, Types::u32 columns>
	class Array2D
	{
	public:
#pragma region Iterator
		class Iter2D
		{
		public:
			using value_type = Vec2<Types::u32>; //almost always T
			using reference = Vec2<Types::u32>const&; //almost always T& or const T&
			using pointer = Vec2<Types::u32>const*; //almost always T* or const T*
			using iterator_category = std::forward_iterator_tag;  //usually std::forward_iterator_tag or similar

			Iter2D(Types::u32 x, Types::u32 y) :
				x(x),
				y(y)
			{
			}

			auto operator++()->Iter2D
			{
				if (++x == columns)
				{
					x = 0;
					y += 1;
				}
				return *this;
			}
			auto operator*() const->value_type
			{
				return Vec2<Types::u32>(x, y);
			}
			auto operator==(const Iter2D& other) const&->bool
			{
				return x == other.x && y == other.y;
			}
			auto operator!=(const Iter2D& other) const&->bool
			{
				return x != other.x || y != other.y;
			}

		private:
			Types::u32 x, y;
		};

		auto begin() const->Iter2D
		{
			return Iter2D(0, 0);
		}

		auto end() const->Iter2D
		{
			return Iter2D(rows - 1, columns - 1);
		}
#pragma endregion

#pragma region Operators
		auto operator[](Types::u32 i) const noexcept->T const&
		{
			return _data[i];
		}

		auto operator[](Types::u32 i) noexcept->T&
		{
			return _data[i];
		}
#pragma endregion

#pragma region at overloads
		auto at(Types::u32 row, Types::u32 col) const noexcept->T const&
		{
			return _data[columns*row + col];
		}

		auto at(Types::u32 row, Types::u32 col) noexcept->T&
		{
			return _data[columns*row + col];
		}

		auto at(Vec2<Types::u32> col_row) const noexcept->T const&
		{
			auto col = col_row.x;
			auto row = col_row.y;
			return _data[columns*row + col];
		}

		auto at(Vec2<Types::u32> col_row) noexcept->T&
		{
			auto col = col_row.x;
			auto row = col_row.y;
			return _data[columns*row + col];
		}
#pragma endregion

	private:
		std::array<T, rows*columns> _data;
	};
}