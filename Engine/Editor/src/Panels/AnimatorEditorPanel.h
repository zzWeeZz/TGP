#pragma once
#include <vector>
#include <string>
namespace Engine
{
	struct AnimationSpecs;
	class AnimatorEditorPanel
	{
	public:
		void OnImGuiRender();
	private:
		void WriteToFile(const std::string& path);
		void ReadFromFile(const std::string& path);
		std::string m_SkPath;
		std::vector<AnimationSpecs> m_AnimationSpecs;
	};
}

