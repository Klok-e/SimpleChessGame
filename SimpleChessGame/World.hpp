#pragma once

#include <tuple>
#include <vector>
#include "Types.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "Meta.hpp"
#include <boost/fusion/container.hpp>
#include "ISystem.hpp"

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

		}

		template<class ...Components>
		auto CreateEntity(Components&&... values)->void
		{
			// all components must inherit from IComponent
			static_assert(is_everything_in_set_inherits_from<IComponent, Components...>);

			// check whether Components is a subset of ComponentTypes
			static_assert(set<Components...>::template is_subset_of<ComponentTypes...>);

			((values.entity.ind = _entityCounter++,
			  std::get<std::vector<Components>>(_components).push_back(values),
			  _entities.push_back(values.entity)), ...);

		}

		auto Update()->void
		{
			static_foreach(_systems, [&](auto system)
			{
				// compiler bugs out otherwise
				using tp = decltype(system);
				typedef tp::componentsRequired tuple;

				tuple comps;

				using comps_in_vectors = put_every_right_in_left<std::vector, tuple>::value;

				static_foreach(comps, [&](auto compType)
				{
					std::get<std::vector<decltype(compType)>>(_components);
				});

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

		Engine::Types::ui32 _entityCounter = 1;
	};
}