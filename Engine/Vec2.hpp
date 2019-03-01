#pragma once

namespace Engine
{
	template<class T>
	struct Vec2
	{
		T x, y;

		Vec2()
		{
		}

		Vec2(T x, T y) :
			x(x),
			y(y)
		{
		}
	};

	template<class T>
	auto operator+(Vec2<T> left, Vec2<T> right) -> Vec2<T>
	{
		return Vec2(left.x + right.x, left.y + right.y);
	}

	template<class T>
	auto operator-(Vec2<T> left, Vec2<T> right) -> Vec2<T>
	{
		return Vec2(left.x - right.x, left.y - right.y);
	}

	template<class T>
	auto operator*(Vec2<T> left, Vec2<T> right) -> Vec2<T>
	{
		return Vec2(left.x * right.x, left.y * right.y);
	}

	template<class T>
	auto operator/(Vec2<T> left, Vec2<T> right) -> Vec2<T>
	{
		return Vec2(left.x / right.x, left.y / right.y);
	}
}