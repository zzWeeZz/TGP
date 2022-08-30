#pragma once
#include <future>
#include <string>
#include "SubMesh.h"
#include "Engine/DX11/DX11.h"
#include "Engine/Renderer/Mesh/Skeleton.h"


namespace Engine
{
	struct AnimationSpecs
	{
		std::filesystem::path FilePath;
		std::string Name;
		bool Loop = true;
		bool Interpolate = true;
		float Speed = 1.f;
	};

	struct BlendState
	{
		std::string firstAnimation;
		std::string secondAnimation;

		float alpha;
	};

	struct BlendStateData
	{
		AnimationSpecs* firstAnimation = nullptr;
		AnimationSpecs* secondAnimation = nullptr;

		float alpha = 0.5f;
	};

	class AnimatedMesh
	{
	public:
		AnimatedMesh() = default;
		AnimatedMesh(AnimatedMesh& mesh);
		void SetMesh(const std::filesystem::path& aPath, std::vector<Ref<Material>>& outMaterials);
		void SetAnimation(const std::filesystem::path& aPath, const std::string& name);
		void SetCurrentAnimation(AnimationSpecs* aAnimation, bool blend);
		void BlendBetween(const BlendStateData& blendState);
		void Update(float aDeltaTime);
		void SetBlend(float blend) {myBlendFactor = blend;}
		Skeleton& GetSkeleton() { return mySkeleton; }
		std::array<Matrix4x4f, 128>& GetCurrentTransforms() { return myBoneTransforms; }
		void Draw();

	private:
		void RunBlending(const BlendStateData& blendState);

		void UpdateHierarchy(size_t aCurrentFrame, uint32_t aBoneIndex, const Animation* aAnim, Matrix4x4f& aParentTransform, Matrix4x4f* outBoneTransforms);
		void InterpolateHierarchy(size_t aCurrentFrame, float blendFactor, uint32_t aBoneIndex, const Animation* aAnim, Matrix4x4f& aParentTransform, Matrix4x4f* outBoneTransforms);
		void BlendHierarchy(size_t aCurrentFrame, size_t aCurrentFrame2, float blendFactor1, float blendFactor2, float blendAlpha, uint32_t aBoneIndex, const Animation* aAnim, const Animation* aAnim2, Matrix4x4f& aParentTransform, Matrix4x4f* outBoneTransforms);
		void ProcessMaterials(const std::filesystem::path& path, const std::string& name,
		                      std::vector<Ref<Material>>& outMaterials);
		void LoadAnimation(const std::filesystem::path& aPath, std::vector<Ref<Material>>& outMaterials);
		Matrix4x4f Blender(Matrix4x4f& aFirstBone, Matrix4x4f& aSecondBone, float aBlendTime);
		std::array<Matrix4x4f, 128> myBoneTransforms;
		Skeleton mySkeleton;
		std::vector<SubMesh> mySubMeshes;
		float myBlendFactor = 0.5f;
		bool myBlend = false;

		float myTime = 0;
		int myFrame = 0;
		AnimationSpecs* myCurrentAnimation = nullptr;
	};
}