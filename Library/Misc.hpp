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

	template<typename T>
	struct SimplestHash
	{
		T operator()(T x)const
		{
			return x;
		}
	};

	constexpr auto to_u32(size_t x)->Engine::Types::u32
	{
		return static_cast<Engine::Types::u32>(x);
	}
}