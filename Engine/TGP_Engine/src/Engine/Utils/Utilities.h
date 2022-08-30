#pragma once
#include <memory>
#include <filesystem>
#include <wrl/client.h>
#include <iostream>
#include <cassert>
using namespace Microsoft::WRL;

#define AssertIfFailed(x) { assert(x == S_OK); }

namespace Engine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
