#pragma once

#include "Vector2.hpp"

namespace Game
{
	template<class Numeric>
	class Rect
	{
	public:
		Vector2<Numeric> xy, wh;

		Rect(Numeric x, Numeric y, Numeric width, Numeric height) :
			xy(x, y),
			wh(width, height)
		{
		}

		Rect(Vector2<Numeric> xy, Vector2<Numeric> wh) :
			xy(xy),
			wh(wh)
		{
		}

	};
}