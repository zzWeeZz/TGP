#pragma once
#include <unordered_map>

#include "Engine/Renderer/Mesh/Animation.h"


struct Skeleton
{
	std::string name;

	struct Bone
	{
		Matrix4x4f bindPoseInverse;
		uint32_t parentIndex;
		std::string name;
		std::vector<uint32_t> childrenIndices;
	};

	std::vector<Matrix4x4f>& GetBoneMatrices()
	{
		std::vector<Matrix4x4f> mats;
		for(auto& bone : bones)
		{
			mats.emplace_back(bone.bindPoseInverse);
		}
		return mats;
	}

	std::vector<Bone> bones;

	std::unordered_map<std::string, size_t> boneNameToIndex;
	std::vector<std::string> boneNames;

	std::unordered_map<std::string, Animation> animations;
	Skeleton() = default;
	Skeleton(const Skeleton& other)
	{
		name = other.name;
		bones = other.bones;
		boneNameToIndex = other.boneNameToIndex;
		boneNames = other.boneNames;
		animations = other.animations;
	}
};
