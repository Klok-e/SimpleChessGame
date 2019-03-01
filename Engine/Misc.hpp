#pragma once

#include <optional>

namespace Engine::Misc
{
	template<class T, class Func>
	constexpr auto do_if_has_value(std::optional<T> opt, Func function)->void
	{
		if (opt)
			function(opt.value);
	}
}