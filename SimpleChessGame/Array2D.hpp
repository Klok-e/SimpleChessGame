#pragma once

#include <array>

namespace Game
{
	template<class T, size_t rows, size_t columns>
	class Array2D
	{
	public:
		T const& operator[](Types::ui32 i) const noexcept
		{
			return _data[i];
		}
		T& operator[](Types::ui32 i) noexcept
		{
			return _data[i];
		}
		T const& at(Types::ui32 row, Types::ui32 col) const noexcept
		{
			return _data[columns*row + col];
		}
		T& at(Types::ui32 row, Types::ui32 col) noexcept
		{
			return _data[columns*row + col];
		}
	private:
		std::array<T, rows*columns> _data;
	};
}