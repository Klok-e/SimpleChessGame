#pragma once

#include <array>

namespace Game
{
	template<class T, size_t rows, size_t columns>
	class Array2D
	{
	public:
		T const& operator[](size_t i) const noexcept
		{
			return _data[i];
		}
		T& operator[](size_t i) noexcept
		{
			return _data[i];
		}
		T const& at(size_t row, size_t col) const noexcept
		{
			return _data[columns*row + col];
		}
		T& at(size_t row, size_t col) noexcept
		{
			return _data[columns*row + col];
		}
	private:
		std::array<T, rows*columns> _data;
	};
}