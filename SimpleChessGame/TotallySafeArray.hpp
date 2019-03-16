#pragma once
#include <doctest/doctest.h>

namespace Game
{
	class ICanBecomeDangling
	{
	public:
		ICanBecomeDangling() = default;
		ICanBecomeDangling(ICanBecomeDangling&& other) = delete;
		ICanBecomeDangling(ICanBecomeDangling const& other) = delete;

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
		using u32 = Engine::Types::u32;
		using componentType = CompType;

		TotallySafeArrayWrapper(TotallySafeArrayWrapper&& other) = default;
		TotallySafeArrayWrapper(TotallySafeArrayWrapper const& other) = delete;

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

		size_t size() const
		{
			return _size;
		}

	private:
		CompType* _ptr;
		size_t _size;
	};

	template<>
	class TotallySafeArrayWrapper<Entity> :
		public ICanBecomeDangling
	{
	public:
		using u32 = Engine::Types::u32;
		using componentType = Entity;

		TotallySafeArrayWrapper(TotallySafeArrayWrapper&& other) = default;
		TotallySafeArrayWrapper(TotallySafeArrayWrapper const& other) = delete;

		TotallySafeArrayWrapper(Entity* ptr, size_t size) :
			_ptr(ptr),
			_size(size)
		{
		}

		Entity& operator[](u32 i)
		{
			if (!_isTotallyNotDangling)
				std::terminate();

			return _ptr[i];
		}

		Entity const& operator[](u32 i)const
		{
			if (!_isTotallyNotDangling)
				std::terminate();

			return _ptr[i];
		}

		size_t size() const
		{
			return _size;
		}

	private:
		Entity* _ptr;
		size_t _size;
	};
}