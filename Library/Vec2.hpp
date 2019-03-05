#pragma once

namespace Engine
{
	template<class TypeToGetIndexOf>
	struct Vec2
	{
		TypeToGetIndexOf x, y;

		Vec2()
		{
		}

		Vec2(TypeToGetIndexOf x, TypeToGetIndexOf y) :
			x(x),
			y(y)
		{
		}
	};

	template<class TypeToGetIndexOf>
	auto operator+(Vec2<TypeToGetIndexOf> left, Vec2<TypeToGetIndexOf> right) -> Vec2<TypeToGetIndexOf>
	{
		return Vec2(left.x + right.x, left.y + right.y);
	}

	template<class TypeToGetIndexOf>
	auto operator-(Vec2<TypeToGetIndexOf> left, Vec2<TypeToGetIndexOf> right) -> Vec2<TypeToGetIndexOf>
	{
		return Vec2(left.x - right.x, left.y - right.y);
	}

	template<class TypeToGetIndexOf>
	auto operator*(Vec2<TypeToGetIndexOf> left, Vec2<TypeToGetIndexOf> right) -> Vec2<TypeToGetIndexOf>
	{
		return Vec2(left.x * right.x, left.y * right.y);
	}

	template<class TypeToGetIndexOf>
	auto operator/(Vec2<TypeToGetIndexOf> left, Vec2<TypeToGetIndexOf> right) -> Vec2<TypeToGetIndexOf>
	{
		return Vec2(left.x / right.x, left.y / right.y);
	}
}