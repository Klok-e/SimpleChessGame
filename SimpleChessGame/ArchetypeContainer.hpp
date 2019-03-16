#pragma once

#include <doctest/doctest.h>
#include <array>
#include "Types.hpp"
#include "Meta.hpp"
#include <vector>
#include "Entity.hpp"
#include "Misc.hpp"
#include "static_map.hpp"
#include "TotallySafeArray.hpp"
#include <memory>
#include <optional>

namespace Game
{
	template<size_t ComponentsCount>
	class ArchetypeContainer
	{
	public:
		using u32 = Engine::Types::u32;
		using componentIndex = u32;
		using SimplestHash = Engine::Misc::SimplestHash<u32>;

		static constexpr size_t componentsCount = ComponentsCount;

		ArchetypeContainer(std::array<std::tuple<componentIndex, size_t>, ComponentsCount>const& componentIds)
		{
			for (std::tuple<componentIndex, size_t> elem : componentIds)
			{
				_map.insert(std::move(std::get<0>(elem)), std::make_tuple(std::vector<std::byte>(), std::get<1>(elem)));
			}
		}

		ArchetypeContainer(ArchetypeContainer const& other) = delete;
		//{
		//	std::terminate();
		//}

		ArchetypeContainer(ArchetypeContainer&& other) = default;

		auto ContainsComponent(componentIndex comp)const->bool
		{
			return _map.contains(comp);
		}

		auto FindEntityIndex(Entity ent)const->std::optional<size_t>
		{
			if (auto res = std::find(_entities.begin(), _entities.end(), ent); res != _entities.end())
				return std::make_optional(res - _entities.begin());
			else
				return std::nullopt;
		}

		auto EntitiesCount()const->size_t
		{
			return _entities.size();
		}

		auto GetAllComponentIds()const->std::array<componentIndex, componentsCount>const&
		{
			return _map.get_keys();
		}

		auto GetAllComponentSizes()const->std::array<std::tuple<std::vector<std::byte>, size_t>, componentsCount>const&
		{
			return _map.get_values();
		}

		template<typename ComponentType>
		auto GetComponentArray(componentIndex componentId)->TotallySafeArrayWrapper<ComponentType>&
		{
			auto& tpl = _map.get(componentId);

			std::vector<std::byte>& vect = std::get<0>(tpl);
			size_t size = std::get<1>(tpl);

			size_t elementCount = vect.size() / sizeof(ComponentType);

			if (vect.size() % sizeof(ComponentType) != 0)
				std::terminate();

			return AddCompArray<ComponentType>(vect, elementCount);
		}

		auto GetEntityArray()->TotallySafeArrayWrapper<Entity>&
		{
			return AddCompArray<Entity>(_entities, _entities.size());
		}

		auto AddEntity(Entity entity)->void
		{
			InvalidateAllArrays();

			for (auto& element : _map)
			{
				std::vector<std::byte>& storageVector = std::get<0>(element);
				size_t storageElementSize = std::get<1>(element);

				for (size_t i = 0; i < storageElementSize; i++)
					storageVector.push_back(std::byte(0));
			}
			_entities.push_back(entity);
		}

		auto SetEntityComponents(Entity entity, std::array<std::tuple<componentIndex, std::byte const*>, ComponentsCount>const& components)->void
		{
			for (auto element : components)
			{
				SetEntityComponent(entity, element);
			}
		}

		auto SetEntityComponent(Entity entity, std::tuple<componentIndex, std::byte const*>const& component)->void
		{
			auto index = std::get<0>(component);
			std::byte const* ptr = std::get<1>(component);

			if (!ContainsComponent(index))
			{
				std::cerr << "Tried to set inexistent component";
				std::terminate();
			}

			if (auto entityInd = FindEntityIndex(entity))
			{
				auto& vect = std::get<0>(_map.get(index));
				size_t size = std::get<1>(_map.get(index));

				size_t ind = entityInd.value() * size;

				if (ind + size > vect.size())
					std::terminate();

				if (ptr[0] == std::byte(42))
				{

				}

				auto dst = vect.data() + ind;
				std::memcpy(dst, ptr, size);
			}
			else
			{
				std::cerr << "Tried to set component on a nonexistent entity";
				std::terminate();
			}
		}

		auto GetEntityComponent(Entity entity, componentIndex compId)const-> std::byte const*
		{
			if (auto entityInd = FindEntityIndex(entity))
			{
				auto& vect = std::get<0>(_map.get(compId));
				size_t size = std::get<1>(_map.get(compId));

				size_t ind = entityInd.value() * size;

				if (ind + size > vect.size())
					std::terminate();

				std::byte const* ret = vect.data() + ind;
				return ret;
			}
			else
			{
				std::cerr << "Tried to get component on a nonexistent entity";
				std::terminate();
			}
		}

		auto RemoveEntity(Entity entity)->void
		{
			InvalidateAllArrays();

			if (auto entityInd = FindEntityIndex(entity))
			{
				_entities.erase(_entities.begin() + entityInd.value());

				for (auto& value : _map)
				{
					auto& vect = std::get<0>(value);
					auto size = std::get<1>(value);

					auto ind = entityInd.value() * size;

					vect.erase(vect.begin() + ind, vect.begin() + ind + size);
				}
			}
			else
			{
				std::cerr << "Tried to remove a nonexistent entity";
				std::terminate();
			}
		}

		auto RemoveEntity(size_t entityIndex)->void
		{
			InvalidateAllArrays();

			if (entityIndex < EntitiesCount())
			{
				_entities.erase(_entities.begin() + entityIndex);

				for (auto& value : _map)
				{
					auto& vect = std::get<0>(value);
					auto size = std::get<1>(value);

					auto ind = entityIndex * size;

					vect.erase(vect.begin() + ind, vect.begin() + ind + size);
				}
			}
			else
			{
				std::cerr << "Out of bounds";
				std::terminate();
			}
		}

	private:
		static_map<componentIndex, std::tuple<std::vector<std::byte>, size_t>, ComponentsCount, SimplestHash> _map{SimplestHash()};

		std::vector<Entity> _entities;

		std::vector<std::unique_ptr<ICanBecomeDangling>> _wrappers;

		template<typename ComponentType, typename VectType>
		auto AddCompArray(std::vector<VectType>& vect, size_t elements)->TotallySafeArrayWrapper<ComponentType>&
		{
			_wrappers.push_back(std::make_unique<TotallySafeArrayWrapper<ComponentType>>(vect.data(), elements));

			return static_cast<TotallySafeArrayWrapper<ComponentType>&>(*_wrappers.back());
		}

		auto InvalidateAllArrays()->void
		{
			for (auto& wrapper : _wrappers)
				wrapper->SetIsNowDangling();
		}
	};

	TEST_CASE("Testing ArchetypeContainer")
	{
		using ind = ArchetypeContainer<1>::componentIndex;

		ArchetypeContainer<5> archetype(std::array{
				std::make_tuple((ind)1, (size_t)4),
				std::make_tuple((ind)6, (size_t)8),
				std::make_tuple((ind)3, (size_t)16),
				std::make_tuple((ind)4, (size_t)24),
				std::make_tuple((ind)91, (size_t)32)});

		SUBCASE("Add Entity")
		{
			archetype.AddEntity(Entity(123));

			archetype.AddEntity(Entity(1234));

			CHECK(archetype.EntitiesCount() == 2);
		}
		SUBCASE("Remove Entity")
		{
			archetype.AddEntity(Entity(123));

			archetype.AddEntity(Entity(1234));

			archetype.RemoveEntity(Entity(123));

			CHECK(archetype.EntitiesCount() == 1);
		}
		SUBCASE("Get Component Array and Set Entity Components")
		{
			ArchetypeContainer<3> archetype(std::array{
				   std::make_tuple((ind)1, sizeof(int)),
				   std::make_tuple((ind)6, sizeof(short)),
				   std::make_tuple((ind)3, sizeof(float))});

			archetype.AddEntity(Entity(123));
			archetype.AddEntity(Entity(12345));
			archetype.AddEntity(Entity(1234));
			archetype.RemoveEntity(Entity(12345));

			auto c1 = int(64);
			auto c2 = short(55555.23423);
			auto c3 = float(-143245345);

			archetype.SetEntityComponents(Entity(1234), std::array{
										  std::make_tuple((ind)1,(std::byte const*)(&c1)),
										  std::make_tuple((ind)6,(std::byte const*)(&c2)),
										  std::make_tuple((ind)3,(std::byte const*)(&c3))});

			auto& ints = archetype.GetComponentArray<int>(1);
			auto& floats = archetype.GetComponentArray<float>(3);
			auto& shorts = archetype.GetComponentArray<short>(6);

			CHECK(ints[1] == c1);
			CHECK(floats[1] == c3);
			CHECK(floats[1] == c3);
		}
	}
}
