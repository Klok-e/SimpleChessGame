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

namespace Game
{
	template<typename ComponentTypesTuple, typename SystemsTuple>
	class World;

	template<typename ...ComponentTypes, typename ...Systems>
	class World<std::tuple<ComponentTypes...>, std::tuple<Systems...>>
	{
		// all components must inherit from IComponent
		static_assert(is_everything_in_set_inherits_from<IComponent, ComponentTypes...>);

		// all systems must inherit from ISystem
		static_assert(is_all_true<is_derived_from<ISystem, Systems>::value...>);
	public:
		World()
		{
			auto& x = _archetypeByComponentCount;
			int y = 0;
		}

		template<class ...Components>
		auto CreateEntity(Components&&... values)->void
		{
			// all components must inherit from IComponent
			static_assert(is_everything_in_set_inherits_from<IComponent, Components...>);

			// check whether Components is a subset of ComponentTypes
			static_assert(set<Components...>::template is_subset_of<ComponentTypes...>);

			((std::get<std::vector<Components>>(_components).push_back(values),
			  _entities.push_back(Entity(_entityCounter++))), ...);
		}

		template<typename Component>
		auto AddComponent(Entity entity, Component value)->void
		{
			std::get<std::vector<Component>>(_components).push_back(value);
		}

		template<typename Component>
		auto RemoveComponent(Entity entity)->void
		{
			auto comps = std::get<std::vector<Component>>(_components);
			auto res = std::find_if(comps.begin(), comps.end(), [entity](auto value)
			{
				return value.entity.ind == entity.ind;
			});
			if (res != comps.end())
			{
				comps.erase(res);
			}
			else
			{
				throw std::exception("component does not exist on provided entity");
			}
		}

		template<typename Component>
		auto SetComponent(Entity entity, Component newValue)->void
		{
			std::get<std::vector<Component>>(_components);
			throw std::exception("not implemented");
		}

		auto Update()->void
		{
			static_foreach(_systems, [&](auto system)
			{
				// compiler bugs out otherwise
				using tp = decltype(system);
				typedef tp::componentsRequired tuple;

				//tuple comps;

				//using comps_in_vectors = put_every_right_in_left<std::vector, tuple>::typename value;

				//static_foreach(comps, [&](auto compType)
				//{
				//	std::get<std::vector<decltype(compType)>>(_components);
				//});

				for (size_t i = 0; i < 2; i++)
				{
					if constexpr (system.needEntity)
					{
						//system.Update();
					}
					else
					{
						//system.Update();
					}
				}
			});
		}

	private:
		std::tuple<std::vector<ComponentTypes>...> _components;
		std::vector<Entity> _entities;

		std::tuple<Systems...> _systems;

		Engine::Types::u32 _entityCounter = 1;

		put_every_right_index_pl1_in_left_t<make_template_int_container_type<std::vector, ArchetypeContainer>::value, ComponentTypes...> _archetypeByComponentCount;
	};
}