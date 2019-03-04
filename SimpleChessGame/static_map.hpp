#pragma once

#include "Types.hpp"

namespace Game
{
	template<typename KeyType, typename ValueType, Engine::Types::u32 elements>
	class static_map
	{
	public:
		using u32 = Engine::Types::u32;

		class Iter
		{
		public:
			Iter(u32 ind, std::array<std::tuple<KeyType, ValueType>, elements>& arr) :
				ind(ind),
				arr(arr)
			{
			}

			Iter operator++()
			{
				return ++ind;
			}
			auto operator*() const->ValueType
			{
				return std::get<ValueType>(arr[ind]);
			}
			auto operator!=(const Iter& other) const&->bool
			{
				return ind != other.ind;
			}
		private:
			u32 ind;
			std::array<std::tuple<KeyType, ValueType>, elements>& arr;
		};

		static_map()
		{
		}

		auto begin()->Iter
		{
			return Iter(0, _data);
		}

		auto end()->Iter
		{
			return Iter(elements, _data);
		}

		std::array<std::tuple<KeyType, ValueType>, elements>const& data()
		{
			return _data;
		}

		template<typename HashFunction>
		void insert(KeyType&& key, ValueType&& value, HashFunction hash)noexcept
		{
			if (_fill >= elements)
				std::terminate();
			_data[_fill++] = std::make_tuple(key, value);
			std::sort(_data.begin(), _data.end(), [&](std::tuple<KeyType, ValueType> const& x, std::tuple<KeyType, ValueType> const& y)
			{
				return hash(std::get<KeyType>(x)) < hash(std::get<KeyType>(y));
			});
		}

		template<typename HashFunction>
		auto get(KeyType const& key, HashFunction hash)noexcept->ValueType
		{
			if (auto index = binary_search(_dataKeys.begin(), _dataKeys.end(), key, [&](KeyType const& x, KeyType const& y)
			{
				return hash(x) < hash(y);
			}))
			{
				return _dataValues[index];
			}
			else
			{
				std::terminate();
			}
		}

	private:
		std::array<KeyType, elements> _dataKeys;
		std::array<ValueType, elements> _dataValues;

		u32 _fill = 0;

		ValueType _cached;

		template<class ForwardIt, class TypeToGetIndexOf, class Compare>
		std::optional<u32> binary_search(ForwardIt first, ForwardIt last, const TypeToGetIndexOf& value, Compare comp)
		{
			first = std::lower_bound(first, last, value, comp);
			if (!(first == last) && !(comp(value, *first)))
				return std::make_optional(*first);
			else
				return std::nullopt;
		}
	};
}