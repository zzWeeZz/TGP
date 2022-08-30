#pragma once
#include "ToolBox/Math/Vector/Vector.h"

#include "Engine/Assets/Asset.h"
#include "Engine/Utils/Utilities.h"

namespace Engine
{
	class DirectionalLight : public Asset
	{
		friend class Renderer;
	public:
		DirectionalLight() = default;

		void SetColor(const Vector3f& color) { myColor.x = color.x; myColor.y = color.y; myColor.z = color.z; }
		void SetIntensity(const float& intensity) { myColor.w = intensity; }
		void SetDirection(const Vector3f& direction) { myDirection = direction; }

		static Ref<DirectionalLight> Create();
	private:
		Vector4f myColor;
		Vector3f myDirection;
	};
}