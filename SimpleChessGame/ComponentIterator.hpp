#pragma once

#include "TotallySafeArray.hpp"
#include <utility>

namespace Game
{
	template<typename Component>
	class ComponentArray
	{
	public:
		using componentType = Component;

		class Iter
		{
		public:
			Iter(size_t currentArray, size_t currentComponent, std::vector<TotallySafeArrayWrapper<Component>*>& componentArrays) :
				_currentArray(currentArray),
				_currentComponent(currentComponent),
				_componentArrays(&componentArrays)
			{
			}

			Component& operator*()
			{
				return (*((*_componentArrays)[_currentArray]))[_currentComponent];
			}

			Iter operator++()
			{
				if (_currentComponent + 1 >= (*_componentArrays)[_currentArray]->size())
				{
					_currentComponent = 0;
					_currentArray += 1;
				}
				else
					_currentComponent += 1;
				return *this;
			}

			Iter operator--()
			{
				if (_currentComponent == 0)
				{
					_currentArray -= 1;
					_currentComponent = (*_componentArrays)[_currentArray]->size() - 1;
				}
				else
					_currentComponent -= 1;
				return *this;
			}

			bool operator==(Iter other)
			{
				return _currentArray == other._currentArray
					&&
					_currentComponent == other._currentComponent
					&&
					_componentArrays == other._componentArrays;
			}

			bool operator!=(Iter other)
			{
				return !(*this == other);
			}

		private:
			size_t _currentArray;
			size_t _currentComponent;
			std::vector<TotallySafeArrayWrapper<Component>*>* _componentArrays;
		};

		ComponentArray(std::vector<TotallySafeArrayWrapper<Component>*> componentArrays) :
			_componentArrays(componentArrays)
		{
		}

		ComponentArray& operator=(ComponentArray const& other) = default;

		Iter begin()
		{
			return Iter(0, 0, _componentArrays);
		}

		Iter end()
		{
			return Iter(_componentArrays.size(), 0, _componentArrays);
		}

		Component operator[](size_t i)
		{
			if (i >= ElementCount())
				std::terminate();

			// TODO: this is too naive (interval tree must be better (in O(log n) instead of O(n)))
			size_t arrInd = 0;
			size_t sum = 0;
			for (; arrInd < _componentArrays.size(); arrInd++)
			{
				size_t s = _componentArrays[arrInd]->size();
				if (sum + s >= arrInd)
					break;
				sum += s;
			}
			return (*_componentArrays[arrInd])[i - sum];
		}

		size_t ElementCount()
		{
			if (!_cachedSize)
			{
				size_t sum = 0;
				for (size_t i = 0; i < _componentArrays.size(); i++)
					sum += _componentArrays[i]->size();
				_cachedSize = sum;
			}
			return _cachedSize.value();
		}

	private:
		std::vector<TotallySafeArrayWrapper<Component>*> _componentArrays;
		std::optional<size_t> _cachedSize = std::nullopt;
	};
}