#pragma once

#include <array>
#include "Types.hpp"
#include "Meta.hpp"
#include <vector>
#include "Entity.hpp"
#include "Misc.hpp"
#include "static_map.hpp"

namespace Game
{
	template<Engine::Types::u32 ComponentsCount>
	class ArchetypeContainer
	{
	public:
		using u32 = Engine::Types::u32;
		using componentIndex = u32;

		class TotallySafeArrayWrapper
		{
		public:
			TotallySafeArrayWrapper(char* ptr, size_t _vectorSize) :
				_ptr(ptr),
				_vectSz(_vectorSize)
			{
			}

			void SetIsNowDangling()
			{
				_isTotallyNotDangling = false;
			}

			template<typename Type>
			Type& operator[](u32 i)
			{
				if (!_isTotallyNotDangling
					||
					_vectSz % sizeof(Type) != 0)
					std::terminate();

				return reinterpret_cast<Type*>(_ptr)[i];
			}

			template<typename Type>
			Type const& operator[](u32 i)const
			{
				if (!_isTotallyNotDangling
					||
					_vectSz % sizeof(Type) != 0)
					std::terminate();

				return reinterpret_cast<Type*>(_ptr)[i];
			}

		private:
			char* _ptr;
			bool _isTotallyNotDangling = true;
			size_t _vectSz;
		};


		ArchetypeContainer()
		{
		}

		ArchetypeContainer(std::array<componentIndex, ComponentsCount> componentIds)
		{
			for (componentIndex elem : componentIds)
			{
				_map.insert(elem, std::make_tuple(std::vector<char>(), 0));
			}
		}

		template<typename ComponentType>
		auto GetComponentArray(componentIndex componentId)->std::tuple<TotallySafeArrayWrapper&, u32>
		{
			auto tpl = _map.get(componentId, Hash);

			auto vect = std::get<0>(tpl);
			auto size = std::get<1>(tpl);

			if (vect.size() % sizeof(ComponentType) != 0)
				std::terminate();

			_wrappers.push_back(TotallySafeArrayWrapper(vect.data(), size));

			return std::make_tuple(_wrappers.back(),
								   vect.size() / sizeof(ComponentType));
		}

		auto AddEntity(Entity entity, std::array<std::tuple<componentIndex, size_t>, ComponentsCount> entityComponents)->void
		{
			for (auto wrapper : _wrappers)
				wrapper.SetIsNowDangling();

			for (auto element : entityComponents)
			{
				auto index = std::get<0>(element);
				auto elementSize = std::get<1>(element);

				auto storageVector = std::get<0>(_map.get(index, Hash));
				auto storageElementSize = std::get<1>(_map.get(index, Hash));

				if (elementSize != storageElementSize)
					std::terminate();

				for (size_t i = 0; i < elementSize; i++)
					storageVector.push_back(0);
			}
			_entities.push_back(entity);
		}

		auto SetEntityComponents(Entity entity, std::array<std::tuple<componentIndex, void*>, ComponentsCount> components)->void
		{
			for (auto element : components)
			{
				auto index = std::get<0>(element);
				auto ptr = std::get<1>(element);

				size_t entityInd = Engine::Misc::find_index_of(_entities.begin(), _entities.end(), entity);

				auto vect = std::get<0>(_map.get(index, Hash));
				auto size = std::get<1>(_map.get(index, Hash));

				auto ind = entityInd * size;

				if (ind + size >= vect.size())
					std::terminate();

				std::memcpy(vect.data() + ind, ptr, size);
			}
		}

		auto RemoveEntity(Entity entity)->void
		{
			for (auto wrapper : _wrappers)
				wrapper.SetIsNowDangling();

			size_t entityInd = Engine::Misc::find_index_of(_entities.begin(), _entities.end(), entity);
			_entities.erase(_entities.begin() + entityInd);

			for (auto value : _map)
			{
				auto vect = std::get<0>(value);
				auto size = std::get<1>(value);

				auto ind = entityInd * size;

				vect.erase(vect.begin() + ind, vect.begin() + ind + size);
			}
		}

	private:
		static_map<componentIndex, std::tuple<std::vector<char>, size_t>, ComponentsCount> _map;

		std::vector<Entity> _entities;

		std::vector<TotallySafeArrayWrapper> _wrappers;

		componentIndex Hash(componentIndex x)
		{
			return x;
		}
	};
}