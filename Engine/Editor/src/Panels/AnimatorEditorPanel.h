#pragma once
#include <vector>
namespace Engine
{
	struct AnimationSpecs;
	class AnimatorEditorPanel
	{
	public:
		void OnImGuiRender();
	private:
		void WriteToFile();
		std::vector<AnimationSpecs> m_AnimationSpecs;
	};
}

