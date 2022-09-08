#pragma once
#include <vector>
#include <string>
#include "Engine/Assets/AssetPacker.h"
namespace Engine
{
	struct AnimationSpecs;
	class AnimatorEditorPanel
	{
	public:
		void OnImGuiRender();
	private:
		std::string m_SkPath;
		std::vector<AnimationSpecs> m_AnimationSpecs;
	};
}

