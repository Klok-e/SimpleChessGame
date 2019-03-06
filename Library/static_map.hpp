#pragma once

#include <doctest/doctest.h>
#include "Types.hpp"
#include <optional>
#include <array>

namespace Game
{
	template<typename KeyType, typename ValueType, Engine::Types::u32 Elements, typename Hash>
	class static_map
	{
	public:
		using u32 = Engine::Types::u32;

		static_map(Hash hash) :
			_hash(hash)
		{
		}

		auto begin()
		{
			return _dataValues.begin();
		}

		auto end()
		{
			return _dataValues.end();
		}

		std::tuple<std::array<KeyType, Elements>, std::array<ValueType, Elements>> const& data()
		{
			return std::make_tuple(_dataKeys, _dataValues);
		}

		bool contains(KeyType const& key)
		{
			return binary_search(_dataKeys.cbegin(), _dataKeys.cend(), key, [&](KeyType const& x, KeyType const& y)
			{
				return _hash(x) < _hash(y);
			}).has_value();
		}

		void insert(KeyType&& key, ValueType&& value)noexcept
		{
			if (_fill >= Elements)
				std::terminate();

			// sort the shit out of it
			auto res = std::lower_bound(_dataKeys.begin(), _dataKeys.begin() + _fill, key, [&](KeyType const& x, KeyType const& y)
			{
				return _hash(x) < _hash(y);
			});
			u32 index = res - _dataKeys.begin();

			std::move_backward(_dataKeys.begin() + index, _dataKeys.begin() + _fill, _dataKeys.begin() + _fill + 1);
			std::move_backward(_dataValues.begin() + index, _dataValues.begin() + _fill, _dataValues.begin() + _fill + 1);

			_dataKeys[index] = key;
			_dataValues[index] = value;
			_fill += 1;
		}

		ValueType get(KeyType const& key)noexcept
		{
			if (std::optional<u32> index = binary_search(_dataKeys.begin(), _dataKeys.end(), key, [&](KeyType const& x, KeyType const& y)
			{
				return _hash(x) < _hash(y);
			}))
			{
				return _dataValues[index.value()];
			}
			else
			{
				std::terminate();
			}
		}

		u32 size()
		{
			return _fill;
		}

	private:
		std::array<KeyType, Elements> _dataKeys;
		std::array<ValueType, Elements> _dataValues;
		Hash _hash;

		u32 _fill = 0;

		template<class ForwardIt, class TypeToGetIndexOf, class Compare>
		std::optional<u32> binary_search(ForwardIt first, ForwardIt last, const TypeToGetIndexOf& value, Compare comp)
		{
			auto find = std::lower_bound(first, last, value, comp);
			if (!(find == last) && !(comp(value, *find)))
				return std::make_optional(find - first);
			else
				return std::nullopt;
		}
	};

	TEST_CASE("Testing static_map")
	{
		constexpr size_t sz = 7;

		struct X
		{
			int val;
		};
		struct Y
		{
			int val;
		};

		auto hash = [](X x)
		{
			return x.val;
		};
		using HashT = decltype(hash);

		std::array<int, 7> keys = {1,4,53,74,50,56,894};
		std::array<int, 7> values = {1,15,65,-40,600,10000000,-20};

		SUBCASE("Inserting into map")
		{
			static_map<X, Y, sz, HashT> map(hash);

			for (int i = 0; i < sz; i++)
			{
				map.insert({i}, {i * 42});
			}
			CHECK(map.size() == sz);
		}

		SUBCASE("Getting from map")
		{
			static_map<X, Y, 7, HashT> map(hash);

			for (int i = 0; i < sz; i++)
				map.insert({keys[i]}, {values[i]});

			for (int i = 0; i < sz; i++)
				CHECK(map.get({keys[i]}).val == values[i]);
		}

		SUBCASE("Test contains")
		{
			static_map<X, Y, 7, HashT> map(hash);

			for (int i = 0; i < sz; i++)
				map.insert({keys[i]}, {values[i]});

			for (int i = 0; i < sz; i++)
				CHECK(map.contains({keys[i]}) == true);

			CHECK(map.contains({1241523}) == false);
			CHECK(map.contains({12523}) == false);
			CHECK(map.contains({-123}) == false);
			CHECK(map.contains({156}) == false);
			CHECK(map.contains({13}) == false);
			CHECK(map.contains({1523}) == false);
			CHECK(map.contains({13}) == false);
			CHECK(map.contains({-11523}) == false);
			CHECK(map.contains({124123}) == false);
			CHECK(map.contains({-241523}) == false);
		}

		SUBCASE("Test iteration")
		{
			static_map<X, Y, 7, HashT> map(hash);

			for (int i = 0; i < sz; i++)
				map.insert({keys[i]}, {values[i]});

			for (auto val : map)
			{
				bool cont = false;
				for (auto elem : values)
					if (val.val == elem)
					{
						cont = true;
						break;
					}
				CHECK(cont);
			}
		}
	}
}