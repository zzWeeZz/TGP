#pragma once
#include "Engine/Assets/Asset.h"
#include "Engine/Utils/Utilities.h"
#include "Engine/Renderer/Mesh/AnimatedMesh.h"

#include "ToolBox/Math/Transform.h"
#include "ToolBox/Utility/Chrono.h"

namespace Engine
{

	class AnimatedModel : public Asset
	{
	public:
		AnimatedModel(const std::filesystem::path& path);
		AnimatedModel(AnimatedModel& other);

		std::vector<Ref<Material>>& GetMaterials() { return m_Materials; }

		void AddAnimation(const AnimationSpecs& specs) { myMesh.SetAnimation(specs.FilePath, specs.Name); myAnimationRegistry[specs.Name] = specs; }
		void PlayAnimation(const std::string& name, bool blendFromPrevious = false) { myMesh.SetCurrentAnimation(&myAnimationRegistry[name], blendFromPrevious); }
		void RunAnimation() { myMesh.Update(ToolBox::Chrono::Timestep()); }
		void BlendBetween(const BlendState& blendState) { myMesh.BlendBetween({&myAnimationRegistry[blendState.firstAnimation], &myAnimationRegistry[blendState.secondAnimation], blendState.alpha }); }

		void BlendFactor(float blendFactor) { myMesh.SetBlend(blendFactor); }

		void Draw() { myMesh.Draw(); }
		AnimatedMesh& GetAnimatedMesh() { return myMesh; }
		ToolBox::Transform& GetTransform() { return myTransform; }

		static Ref<AnimatedModel> Create(const std::filesystem::path& path);
	private:
		AnimatedMesh myMesh;
		std::vector<Ref<Material>> m_Materials;
		std::unordered_map<std::string, AnimationSpecs> myAnimationRegistry;

		ToolBox::Transform myTransform;
	};
}