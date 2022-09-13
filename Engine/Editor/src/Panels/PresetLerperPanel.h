#pragma once
#include "Engine/Scene/Prefab/Prefab.h"
namespace Engine
{
	class Scene;
	class PresetLerperPanel
	{
	public:
		void OnImGuiRender(Scene* scene);
	private:
		PrefabData m_FirstData;
		PrefabData m_SecondData;
		PrefabData m_ResultData;
	};
}