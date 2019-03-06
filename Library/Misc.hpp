#pragma once

#include <optional>

namespace Engine::Misc
{
	template<class TypeToGetIndexOf, class Func>
	constexpr auto do_if_has_value(std::optional<TypeToGetIndexOf> opt, Func function)->void
	{
		if (opt)
			function(opt.value);
	}

	template<typename InputIter, typename ToFindType>
	auto find_index_of(InputIter begin, InputIter end, ToFindType const& value)->size_t
	{
		size_t count = 0;
		for (InputIter i = begin; i != end; i++, count++)
			if (value == *i)
			{
				return count;
			}
	}

	template<typename T>
	struct SimplestHash
	{
		T operator()(T x)
		{
			return x;
		}
	};
}