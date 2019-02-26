#pragma once

namespace Game::ECS
{
	class ISystem
	{
	public:
		virtual void Update() = 0;
		virtual void OnCreate() = 0;
	};
}