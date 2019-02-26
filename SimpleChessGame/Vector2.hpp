#pragma once

namespace Game
{
	template<class Numeric>
	class Vector2
	{
	public:
		Numeric x, y;

		Vector2()
		{
		}

		Vector2(Numeric x, Numeric y)
		{
			this->x = x;
			this->y = y;
		}

		inline Vector2 operator+(Vector2 other) const
		{

			return Vector2(x + other.x, y + other.y);
		}

		inline Vector2 operator-(Vector2 other) const
		{

			return Vector2(x - other.x, y - other.y);
		}
	};
}