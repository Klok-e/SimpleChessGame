#pragma once

#include <tuple>
#include <type_traits>

namespace Game
{
	template<typename T, typename... Ts>
	constexpr bool contains = (std::is_same_v<T, Ts> || ...);

	template<typename... Elements>
	struct set
	{
		template<typename... OtherElements>
		static constexpr bool is_subset_of = (contains<Elements, OtherElements...>&&...);
	};

	template<typename Base, typename ...Set>
	constexpr bool is_everything_in_set_inherits_from = (std::is_base_of_v<Base, Set>&&...);

	template<bool...Bools>
	constexpr bool is_all_true = (Bools && ...);

	template <class Tup, class Func, std::size_t ...Is>
	constexpr void static_for_impl(Tup&& t, Func &&f, std::index_sequence<Is...>)
	{
		(f(std::integral_constant<std::size_t, Is>{}, std::get<Is>(t)), ...);
	}

	template <class ... T, class Func >
	constexpr void static_for(std::tuple<T...>&t, Func &&f)
	{
		static_for_impl(t, std::forward<Func>(f), std::make_index_sequence<sizeof...(T)>{});
	}

	template <class Tup, class Func, std::size_t ...Is>
	constexpr void static_foreach_impl(Tup&& t, Func &&f, std::index_sequence<Is...>)
	{
		(f(std::get<Is>(t)), ...);
	}

	template <class ... T, class Func >
	constexpr void static_foreach(std::tuple<T...>&t, Func &&f)
	{
		static_foreach_impl(t, std::forward<Func>(f), std::make_index_sequence<sizeof...(T)>{});
	}

	template<template<class...> class Base, class Derived>
	struct is_derived_from
	{
	private:
		template<class... V>
		static decltype(static_cast<const Base<V...>&>(std::declval<Derived>()), std::true_type()) test(const Base<V...>&);

		static std::false_type test(...);
	public:
		static constexpr bool value = decltype(is_derived_from::test(std::declval<Derived>()))::value;
	};

	template<template<typename> typename Left, typename RightTuple>
	struct put_every_right_in_left;

	template<template<typename> typename Left, typename...Right>
	struct put_every_right_in_left<Left, std::tuple<Right...>>
	{
		static constexpr std::tuple<Left<Right>...> value;
	};
}
