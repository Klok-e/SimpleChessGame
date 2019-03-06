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
		using SimplestHash = Engine::Misc::SimplestHash<u32>;

		static constexpr u32 componentsCount = ComponentsCount;

		class ICanBecomeDangling
		{
		public:
			void SetIsNowDangling()
			{
				_isTotallyNotDangling = false;
			}

		protected:
			bool _isTotallyNotDangling = true;
		};

		template<typename CompType>
		class TotallySafeArrayWrapper :
			public ICanBecomeDangling
		{
		public:
			TotallySafeArrayWrapper(std::byte* ptr, size_t size) :
				_ptr(reinterpret_cast<CompType*>(ptr)),
				_size(size)
			{
			}

			CompType& operator[](u32 i)
			{
				if (!_isTotallyNotDangling)
					std::terminate();

				return _ptr[i];
			}

			CompType const& operator[](u32 i)const
			{
				if (!_isTotallyNotDangling)
					std::terminate();

				return _ptr[i];
			}

			size_t size()
			{
				return _size;
			}

		private:
			CompType* _ptr;
			size_t _size;
		};

		ArchetypeContainer() :
			_map(SimplestHash())
		{
		}

		ArchetypeContainer(std::array<std::tuple<componentIndex, size_t>, ComponentsCount> componentIds) :
			_map(SimplestHash())
		{
			for (std::tuple<componentIndex, size_t> elem : componentIds)
			{
				_map.insert(std::move(std::get<0>(elem)), std::make_tuple(std::vector<std::byte>(), std::get<1>(elem)));
			}
		}

		auto ContainsComponent(componentIndex comp)->bool
		{
			return _map.contains(comp);
		}

		template<typename ComponentType>
		auto GetComponentArray(componentIndex componentId)->TotallySafeArrayWrapper<ComponentType>&
		{
			auto tpl = _map.get(componentId);

			std::vector<std::byte> vect = std::get<0>(tpl);
			size_t size = std::get<1>(tpl);

			size_t elementCount = vect.size() / sizeof(ComponentType);

			if (vect.size() % sizeof(ComponentType) != 0)
				std::terminate();

			_wrappers.push_back(std::make_unique<TotallySafeArrayWrapper>(vect.data(), elementCount));

			return *_wrappers.back();
		}

		auto AddEntity(Entity entity, std::array<std::tuple<componentIndex, size_t>, ComponentsCount> entityComponents)->void
		{
			for (auto& wrapper : _wrappers)
				wrapper->SetIsNowDangling();

			for (auto element : entityComponents)
			{
				componentIndex index = std::get<0>(element);
				size_t elementSize = std::get<1>(element);

				std::vector<std::byte> storageVector = std::get<0>(_map.get(index));
				size_t storageElementSize = std::get<1>(_map.get(index));

				if (elementSize != storageElementSize)
					std::terminate();

				for (size_t i = 0; i < elementSize; i++)
					storageVector.push_back(std::byte(0));
			}
			_entities.push_back(entity);
		}

		auto SetEntityComponents(Entity entity, std::array<std::tuple<componentIndex, std::byte*>, ComponentsCount> components)->void
		{
			for (auto element : components)
			{
				auto index = std::get<0>(element);
				auto ptr = std::get<1>(element);

				size_t entityInd = Engine::Misc::find_index_of(_entities.begin(), _entities.end(), entity);

				auto vect = std::get<0>(_map.get(index));
				auto size = std::get<1>(_map.get(index));

				auto ind = entityInd * size;

				if (ind + size >= vect.size())
					std::terminate();

				std::memcpy(vect.data() + ind, ptr, size);
			}
		}

		auto RemoveEntity(Entity entity)->void
		{
			for (auto& wrapper : _wrappers)
				wrapper->SetIsNowDangling();

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
		static_map<componentIndex, std::tuple<std::vector<std::byte>, size_t>, ComponentsCount, SimplestHash> _map;

		std::vector<Entity> _entities;

		std::vector<std::unique_ptr<ICanBecomeDangling>> _wrappers;
	};
}