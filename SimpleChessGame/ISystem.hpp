#pragma once

namespace Game
{
	template<typename BoolTypeNeedEntity, typename ...ComponentsRequires>
	class ISystem
	{
	public:
		static constexpr bool needEntity = BoolTypeNeedEntity::value;
		using componentsRequired = std::tuple<ComponentsRequires...>;

		using delta_t = Engine::Types::f64;

		template<bool is_enabled = !needEntity>
		auto Update(typename std::enable_if_t<is_enabled, delta_t> deltaTime, std::tuple<ComponentsRequires...> components)->void
		{
			std::terminate();
		}

		template<bool is_enabled = needEntity>
		auto Update(typename std::enable_if_t<is_enabled, Entity> const& entity, delta_t deltaTime, std::tuple<ComponentsRequires...> components)->void
		{
			std::terminate();
		}
	};
}