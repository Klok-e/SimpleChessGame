#pragma once

#include <tuple>
#include <type_traits>
#include <array>
#include <algorithm>

namespace Game
{
	template<typename TypeToGetIndexOf, typename... Ts>
	constexpr bool contains = (std::is_same_v<TypeToGetIndexOf, Ts> || ...);

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

	template<bool PassInIndex = false, class Tup, class Func, std::size_t ...Is>
	constexpr void static_apply_with_indices(Tup& t, std::index_sequence<Is...> indices, Func &&f)
	{
		if constexpr (PassInIndex)
			(f(std::integral_constant<std::size_t, Is>{}, std::get<Is>(t)), ...);
		else
			(f(std::get<Is>(t)), ...);
	}

	template <class ... TypeToGetIndexOf, class Func >
	constexpr void static_for(std::tuple<TypeToGetIndexOf...>&t, Func &&f)
	{
		static_apply_with_indices<true>(t, std::make_index_sequence<sizeof...(TypeToGetIndexOf)>(), std::forward<Func>(f));
	}

	template <class ... TypeToGetIndexOf, class Func >
	constexpr void static_foreach(std::tuple<TypeToGetIndexOf...>&t, Func &&f)
	{
		static_apply_with_indices(t, std::make_index_sequence<sizeof...(TypeToGetIndexOf)>(), std::forward<Func>(f));
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

	template<template<typename> typename Left, typename...Right>
	struct put_every_right_in_left
	{
		static constexpr std::tuple<Left<Right>...> value;
	};

	template<template<size_t> typename Left, typename RightTuple>
	struct put_every_right_index_in_left;

	template<template<size_t> typename Left, size_t... RightTuple>
	struct put_every_right_index_in_left<Left, std::index_sequence<RightTuple...>>
	{
		using value = std::tuple<Left<RightTuple>...>;
	};

	template<template<size_t> typename Left, typename numbersContainer>
	struct put_every_right_index_plus1_in_left_impl;

	template<template<size_t> typename Left, Engine::Types::u32... numbers>
	struct put_every_right_index_plus1_in_left_impl<Left, std::index_sequence<numbers...>>
	{
		typedef std::tuple<Left<numbers + 1>...> value;
	};

	template<template<size_t> typename Left, typename...Right>
	struct put_every_right_index_plus1_in_left
	{
		typedef typename put_every_right_index_plus1_in_left_impl<Left, std::make_index_sequence<sizeof...(Right)>>::value value;
	};

	template<template<size_t> typename Left, typename...Right>
	using  put_every_right_index_pl1_in_left_t = typename put_every_right_index_plus1_in_left<Left, Right...>::value;

	template<template<typename> typename Container, template<size_t> typename Type>
	struct make_template_int_container_type
	{
		template<size_t number>
		using value = Container<Type<number>>;
	};

	template<template<typename> typename Container, template<size_t> typename Type>
	struct make_template_int_container_rawptr_to_type
	{
		template<size_t number>
		using value = Container<Type<number>*>;
	};

	template <class TypeToGetIndexOf, class Tuple>
	struct get_type_index;

	template <class TypeToGetIndexOf, class... Types>
	struct get_type_index<TypeToGetIndexOf, std::tuple<TypeToGetIndexOf, Types...>>
	{
		static constexpr std::size_t value = 0;
	};

	template <class TypeToGetIndexOf, class CompType, class... Types>
	struct get_type_index<TypeToGetIndexOf, std::tuple<CompType, Types...>>
	{
		static constexpr std::size_t value = 1 + get_type_index<TypeToGetIndexOf, std::tuple<Types...>>::value;
	};

	template<typename Min, typename IntegerList>
	struct make_range_impl;

	template<size_t Min, size_t... IntegerList>
	struct make_range_impl<std::index_sequence<Min>, std::index_sequence<IntegerList...>>
	{
		using value = std::index_sequence<(IntegerList + Min)...>;
	};

	template<size_t From, size_t To>
	using make_range = typename make_range_impl<std::index_sequence<From>, std::make_index_sequence<To - From>>::value;
}
