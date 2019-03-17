#pragma once

#include <tuple>
#include <vector>
#include "Types.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Meta.hpp"
#include "ISystem.hpp"
#include <algorithm>
#include "ArchetypeContainer.hpp"
#include "Misc.hpp"
#include <string>

namespace Game
{
	template<typename ComponentTypesTuple, typename SystemsTuple>
	class World;

	template<typename ...ComponentTypes, typename ...Systems>
	class World<std::tuple<ComponentTypes...>, std::tuple<Systems...>>
	{
		// all components must inherit from IComponent
		static_assert(is_everything_in_set_inherits_from<IComponent, ComponentTypes...>);

		// all components must be trivially copyable
		static_assert(is_all_true<std::is_trivially_copyable_v<ComponentTypes>...>, "One of components isn't trivially copyable");

		// all components must be default constructible
		static_assert(is_all_true<std::is_default_constructible_v<ComponentTypes>...>, "One of components isn't default constructible");

		// all systems must inherit from ISystem
		static_assert(is_all_true<is_derived_from<ISystem, Systems>::value...>);

	public:
		using u32 = Engine::Types::u32;
		using allComponents = std::tuple<ComponentTypes...>;
		using SimplestHash = Engine::Misc::SimplestHash<u32>;
		static constexpr size_t allComponentsCount = sizeof...(ComponentTypes);

		World()
		{
			// insert every type in the map
			(_mapTypeIdToSize.insert(Engine::Misc::to_u32(GetComponentIndex<ComponentTypes>()), sizeof(ComponentTypes)), ...);
		}

		template<class ...Components>
		auto CreateEntity(Components... values)->Entity
		{
			// all components must inherit from IComponent
			static_assert(is_everything_in_set_inherits_from<IComponent, Components...>);

			// check whether Components is a subset of ComponentTypes
			static_assert(set<Components...>::template is_subset_of<ComponentTypes...>);

			Entity newEnt(_entityCounter++);

			auto& archetype = FindArchetypeFullMatchOrCreate<Components...>();
			archetype.AddEntity(newEnt);

			(archetype.SetEntityComponent(newEnt, std::make_tuple(GetComponentIndex<Components>(), (std::byte*)&values)), ...);

			return newEnt;
		}

		template<typename Component>
		auto AddComponent(Entity entity, Component value)->void
		{
			bool foundAnything = false;
			static_foreach_if(_archetypeByComponentCount, [&](auto& archetypeVect)->bool
			{
				std::optional<size_t> entIndex;
				auto res = std::find_if(archetypeVect.begin(), archetypeVect.end(), [&](auto& archetype)
				{
					entIndex = archetype.FindEntityIndex(entity);
					return entIndex.has_value();
				});
				if (res != archetypeVect.end())
				{
					foundAnything = true;

					auto& archetype = *res;

					constexpr size_t archetypeComponentsCount = archetype.componentsCount;

					// it's impossible to add components over the possible theoretical limit of components that can be
					// associated with an entity (allComponentsCount)
					if constexpr (archetypeComponentsCount < allComponentsCount)
					{
						// find archetype with indexOfThisArhectype+1 components that contains all components of a
						// previous and a component that is asked to be added
						std::array<u32, archetypeComponentsCount + 1> arr;

						auto ids = archetype.GetAllComponentIds();

						for (size_t i = 0; i < archetypeComponentsCount; i++)
							arr[i] = ids[i];

						auto c = GetComponentIndex<Component>();
						arr[archetypeComponentsCount] = c;

						auto& newArchetype = FindArchetypeFullMatchOrCreate(arr);

						newArchetype.AddEntity(entity);

						// copy all components to new archetype
						for (size_t i = 0; i < archetypeComponentsCount; i++)
						{
							auto id = ids[i];

							std::byte const* component = archetype.GetEntityComponent(entity, id);

							newArchetype.SetEntityComponent(entity, std::make_tuple(id, component));
						}
						newArchetype.SetEntityComponent(entity, std::make_tuple(GetComponentIndex<Component>(), (std::byte const*)&value));

						archetype.RemoveEntity(entIndex.value());

						// no need to continue
						return false;
					}
				}
				// continue
				return true;
			});
			if (!foundAnything)
			{
				std::cerr << "Entity hasn't been found";
				std::terminate();
			}
		}

		template<typename Component>
		auto RemoveComponent(Entity entity)->void
		{
			bool foundAnything = false;
			static_foreach_if(_archetypeByComponentCount, [&](auto& archetypeVect)->bool
			{
				std::optional<size_t> entIndex;
				auto res = std::find_if(archetypeVect.begin(), archetypeVect.end(), [&](auto& archetype)
				{
					entIndex = archetype.FindEntityIndex(entity);
					return entIndex.has_value();
				});
				if (res != archetypeVect.end())
				{
					foundAnything = true;

					auto& archetype = *res;

					constexpr size_t archetypeComponentsCount = archetype.componentsCount;

					// it's impossible to remove components below 1 cause entity can't exist without its components
					if constexpr (archetypeComponentsCount > 1)
					{
						// find archetype with indexOfThisArhectype-1 components that contains all components of a
						// previous without the component that is asked to be removed
						std::array<u32, archetypeComponentsCount - 1> newIds;

						std::array<u32, archetypeComponentsCount>const& oldIds = archetype.GetAllComponentIds();

						for (size_t archIds = 0, newArrIds = 0; archIds < archetypeComponentsCount; archIds++, newArrIds++)
						{
							// if current id isn't the same as id that we try to remove then assign it to new array
							if (oldIds[archIds] != GetComponentIndex<Component>())
								newIds[newArrIds] = oldIds[archIds];
							else
								newArrIds -= 1;
						}

						auto& newArchetype = FindArchetypeFullMatchOrCreate(newIds);

						newArchetype.AddEntity(entity);

						// copy all components to new archetype
						for (size_t i = 0; i < newIds.size(); i++)
						{
							auto id = newIds[i];

							std::byte const* component = archetype.GetEntityComponent(entity, id);

							newArchetype.SetEntityComponent(entity, std::make_tuple(id, component));
						}

						archetype.RemoveEntity(entIndex.value());

						// no need to continue
						return false;
					}
				}
				// continue
				return true;
			});
			if (!foundAnything)
			{
				std::cerr << "Entity hasn't been found";
				std::terminate();
			}
		}

		template<typename Component>
		auto SetComponent(Entity entity, Component newValue)->void
		{
			bool foundAnything = false;
			static_foreach_if(_archetypeByComponentCount, [&](auto& archetypeVect)->bool
			{
				std::optional<size_t> entIndex;
				auto res = std::find_if(archetypeVect.begin(), archetypeVect.end(), [&](auto& archetype)
				{
					entIndex = archetype.FindEntityIndex(entity);
					return entIndex.has_value();
				});
				if (res != archetypeVect.end())
				{
					foundAnything = true;

					auto& arch = *res;
					arch.SetEntityComponent(entity, std::make_tuple(GetComponentIndex<Component>(), reinterpret_cast<std::byte const*>(newValue)));
					return false;
				}
				return true;
			});
			if (!foundAnything)
			{
				std::cerr << "Tried to set component on a nonexistent entity";
				std::terminate();
			}
		}

		auto Update()->void
		{
			static_foreach(_systems, [&](auto& system)
			{
				using tuple = typename std::decay_t<decltype(system)>::componentsRequired;

				constexpr bool needEntity = system.needEntity;

				auto[iters, entities] = GetComponentIters(tuple());

				if constexpr (needEntity)
				{
					system.Update(entities, 0, iters);
				}
				else
				{
					system.Update(0, iters);
				}
			});
		}

		template<typename Component>
		static constexpr auto GetComponentIndex()->u32
		{
			return get_type_index<Component, allComponents>::value;
		}

	private:
		std::tuple<Systems...> _systems;

		put_every_right_index_pl1_in_left_t<make_template_int_container_type<std::vector, ArchetypeContainer>::value, ComponentTypes...> _archetypeByComponentCount;

		u32 _entityCounter = 1;

		static_map<u32, size_t, allComponentsCount, SimplestHash> _mapTypeIdToSize{SimplestHash()};

		template<typename ...Components>
		auto GetComponentIters(std::tuple<Components...> tpl)->std::pair<std::tuple<ComponentArray<Components>...>, ComponentArray<Entity>>
		{
			auto tplOfArchetypePtrVectors = FindArchetypeWithAtLeast(tpl);

			std::vector<TotallySafeArrayWrapper<Entity>*> entityArrays;
			// fill entity arrays
			static_foreach(tplOfArchetypePtrVectors, [&](auto& archptrVect)
			{
				for (auto& archptr : archptrVect)
				{
					auto arr = &archptr->GetEntityArray();
					entityArrays.push_back(arr);
				}
			});

			std::tuple<std::vector<TotallySafeArrayWrapper<Components>*>...> tplOfArrayptrs;
			// fill component arrays
			static_foreach(tplOfArrayptrs, [&](auto& vect)
			{
				using compType = typename std::remove_pointer_t<typename std::decay_t<decltype(vect)>::value_type>::componentType;
				static_foreach(tplOfArchetypePtrVectors, [&](auto& archptrVect)
				{
					for (auto& archptr : archptrVect)
					{
						auto arr = &archptr->GetComponentArray<compType>(GetComponentIndex<compType>());
						vect.push_back(arr);
					}
				});
			});

			// construct component iterators
			std::tuple<ComponentArray<Components>...> arrays =
				std::make_tuple(ComponentArray<Components>(std::get<std::vector<TotallySafeArrayWrapper<Components>*>>(tplOfArrayptrs))...);

			auto entArr = ComponentArray<Entity>(std::move(entityArrays));

			return std::make_pair(arrays, entArr);
		}

		// return type is std::tuple<std::vector<ArchetypeContainer<5>*>, std::vector<ArchetypeContainer<6>*>, std::vector<ArchetypeContainer<7>*>...etc>>
		template<typename ...Components>
		auto FindArchetypeWithAtLeast(std::tuple<Components...>)->typename put_every_right_index_in_left<make_template_int_container_rawptr_to_type<std::vector, ArchetypeContainer>::value,
			make_range<sizeof...(Components), sizeof...(ComponentTypes) + 1>>::value
		{
			// check whether Components is a subset of ComponentTypes
			static_assert(set<Components...>::template is_subset_of<ComponentTypes...>);

			using rangeFromCompsToAllComps = make_range<sizeof...(Components) - 1, sizeof...(ComponentTypes)>;

			typename put_every_right_index_in_left<make_template_int_container_rawptr_to_type<std::vector, ArchetypeContainer>::value,
				make_range<sizeof...(Components), sizeof...(ComponentTypes) + 1>>::value archetypePtrVectorsWithRequiredComps{};

			// for every archetype in range
			static_apply_with_indices(_archetypeByComponentCount, rangeFromCompsToAllComps(), [&](auto& archetypeVect)
			{
				for (auto& arch : archetypeVect)
				{
					constexpr size_t compCount = arch.componentsCount;
					constexpr size_t tupleIndex = compCount - sizeof...(Components);

					auto res = std::find_if(archetypeVect.begin(), archetypeVect.end(), [](auto& archetype)
					{
						return (archetype.ContainsComponent(GetComponentIndex<Components>()) && ...);
					});

					// contains all components
					if (res != archetypeVect.end())
					{
						auto& archVect = std::get<tupleIndex>(archetypePtrVectorsWithRequiredComps);
						archVect.push_back(&(*res));
					}
				}
			});

			return archetypePtrVectorsWithRequiredComps;
		}

			template<typename ...Components>
		auto FindArchetypeFullMatchOrCreate()->ArchetypeContainer<sizeof...(Components)>&
		{
			// check whether Components is a subset of ComponentTypes
			static_assert(set<Components...>::template is_subset_of<ComponentTypes...>);

			auto& archetypes = std::get<std::vector<ArchetypeContainer<sizeof...(Components)>>>(_archetypeByComponentCount);

			auto res = std::find_if(archetypes.begin(), archetypes.end(), [](auto& archetype)
			{
				return (archetype.ContainsComponent(GetComponentIndex<Components>()) && ...);
			});
			if (res != archetypes.end())
			{
				return *res;
			}
			else
			{
				ArchetypeContainer<sizeof...(Components)> newArch(std::array{
																  std::make_tuple(GetComponentIndex<Components>(),sizeof(Components))...
																  });
				archetypes.push_back(std::move(newArch));
				return archetypes.back();
			}
		}

		template<size_t CompCount>
		auto FindArchetypeFullMatchOrCreate(std::array<u32, CompCount> const& componentIds)->ArchetypeContainer<CompCount>&
		{
			static_assert(CompCount > 0 && CompCount <= allComponentsCount, "CompCount > 0 && CompCount <= allComponentsCount is false");

			auto& archetypes = std::get<std::vector<ArchetypeContainer<CompCount>>>(_archetypeByComponentCount);

			auto res = std::find_if(archetypes.begin(), archetypes.end(), [&](auto& archetype)
			{
				for (auto& id : componentIds)
					if (!archetype.ContainsComponent(id))
						return false;
				return true;
			});
			if (res != archetypes.end())
			{
				return *res;
			}
			else
			{
				std::array<std::tuple<u32, size_t>, CompCount> arr;
				for (size_t i = 0; i < CompCount; i++)
					arr[i] = std::make_tuple(componentIds[i], _mapTypeIdToSize.get(componentIds[i]));

				ArchetypeContainer<CompCount> newArch(arr);
				archetypes.push_back(std::move(newArch));
				return archetypes.back();
			}
		}
	};

	TEST_CASE("Test World")
	{
		struct Comp1 :
			public IComponent
		{
			double v;
			Comp1()
			{
			}

			Comp1(double v) :
				v(v)
			{
			}
		};
		struct Comp2 :
			public IComponent
		{
			float v;
			Comp2()
			{
			}

			Comp2(float v) :
				v(v)
			{
			}
		};
		struct Comp3 :
			public IComponent
		{
			int v;
			Comp3()
			{
			}

			Comp3(int v) :
				v(v)
			{
			}
		};

		class TestSyst1 :
			public ISystem<std::true_type, Comp3>
		{
		public:
			void Update(ComponentArray<Entity>& entities, delta_t deltaTime, std::tuple<ComponentArray<Comp3>>& components)
			{
				auto&[x] = components;
				CHECK(x[0].v == 12);
			}
		};

		class TestSyst2 :
			public ISystem<std::true_type, Comp1, Comp3>
		{
		public:
			void Update(ComponentArray<Entity>& entities, delta_t deltaTime, std::tuple<ComponentArray<Comp1>, ComponentArray<Comp3>>& components)
			{
				auto&[x1, x2] = components;
				CHECK(x2[0].v == 12);
				CHECK(x1[0].v == 140.);
			}
		};

		World<std::tuple<Comp1, Comp2, Comp3>, std::tuple<TestSyst1, TestSyst2>> world;

		auto ent = world.CreateEntity(Comp1{140.});

		world.AddComponent(ent, Comp3{12});

		world.Update();
	}
}