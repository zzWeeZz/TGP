#include "AnimatedMesh.h"
#include "FBXImporter/FBXImporter.h"
#include "ToolBox/Utility/Chrono.h"

Engine::AnimatedMesh::AnimatedMesh(AnimatedMesh& mesh)
{
	mySkeleton = mesh.mySkeleton;
	memcpy(myBoneTransforms.data(), mesh.myBoneTransforms.data(), sizeof(float) * 16 * 128);
	mySubMeshes = mesh.mySubMeshes;
	myBlendFactor = mesh.myBlendFactor;
	myBlend = mesh.myBlend;
	myTime = mesh.myTime;
	myFrame = mesh.myFrame;
}

void Engine::AnimatedMesh::SetMesh(const std::filesystem::path& aPath, std::vector<Ref<Material>>& outMaterials)
{
	if (std::filesystem::exists(aPath))
	{
		LoadAnimation(aPath, outMaterials);
	}
	else
	{
		throw std::runtime_error("Animation SK does not exist!!");
	}
}

void Engine::AnimatedMesh::SetAnimation(const std::filesystem::path& aPath, const std::string& name)
{
	TGA::FBXAnimation FBXanimation;
	TGA::FBXImporter::LoadAnimation(aPath.string(), mySkeleton.boneNames, FBXanimation);
	Animation animation{};
	animation.path = aPath;
	animation.duration = static_cast<float>(FBXanimation.Duration);
	animation.fps = FBXanimation.FramesPerSecond;
	for (auto& frame : FBXanimation.Frames)
	{
		Animation::Frame pushFrame;
		for (auto& locMat : frame.LocalTransforms)
		{
			Matrix4x4f mat;
			memcpy(&mat, locMat.Data, sizeof(Matrix4x4f));
			pushFrame.localTransforms.emplace_back(Matrix4x4f::Transpose(mat));
		}
		animation.frames.emplace_back(pushFrame);
	}
	mySkeleton.animations[name] = animation;
}

void Engine::AnimatedMesh::SetCurrentAnimation(AnimationSpecs* aAnimation, bool blend)
{
	myCurrentAnimation = aAnimation;

}

void Engine::AnimatedMesh::BlendBetween(const BlendStateData& blendState)
{

	RunBlending(blendState);
}

void Engine::AnimatedMesh::Update(float aDeltaTime)
{
	if (!myCurrentAnimation)
	{
		return;
	}
	
	auto timestep = 1.0f / mySkeleton.animations[myCurrentAnimation->Name].fps;
	myTime += ToolBox::Chrono::Timestep() * myCurrentAnimation->Speed;
	if (myCurrentAnimation->Interpolate)
	{
		Matrix4x4f identity{};
		InterpolateHierarchy(myFrame, myTime / timestep, 0, &mySkeleton.animations[myCurrentAnimation->Name], identity, &myBoneTransforms[0]);
		if (myTime > timestep)
		{
			myFrame += 1;
			myTime = 0;
		}
	}
	else
	{
		if (myTime > timestep)
		{
			Matrix4x4f identity{};
			UpdateHierarchy(myFrame, 0, &mySkeleton.animations[myCurrentAnimation->Name], identity, &myBoneTransforms[0]);
			myFrame += 1;
			myTime = 0;
		}
	}
	if (myFrame >= mySkeleton.animations[myCurrentAnimation->Name].frames.size())
	{
		myFrame = 1;
	}
}

void Engine::AnimatedMesh::Draw()
{
	for (auto& mesh : mySubMeshes)
	{
		mesh.Draw();
	}
}

void Engine::AnimatedMesh::RunBlending(const BlendStateData& blendState)
{
	auto& animA = mySkeleton.animations[blendState.firstAnimation->Name];
	auto& animB = mySkeleton.animations[blendState.secondAnimation->Name];
	static int firstAnimCurrentFrame = 1;
	static int secondAnimCurrentFrame = 1;
	auto timestep = 1.0f / animA.fps;

	//---------- Animation speed matching math -------------------
	float a = 1.0f;
	float b = animA.duration / animB.duration;
	const float animSpeedMultiplierUp = (1.0f - blendState.alpha) * a + b * blendState.alpha;

	a = animB.duration / animA.duration;
	b = 1.0f;
	const float animSpeedMultiplierDown = (1.0f - blendState.alpha) * a + b * blendState.alpha;

	static float currentTimeBase = 0.0f;
	currentTimeBase += ToolBox::Chrono::Timestep() * animSpeedMultiplierUp;
	currentTimeBase = fmod(currentTimeBase, animA.duration);

	static float currentTimeLayered = 0.0f;
	currentTimeLayered += ToolBox::Chrono::Timestep() * animSpeedMultiplierDown;
	currentTimeLayered = fmod(currentTimeLayered, animB.duration);

	// -----------------------------------------------------------

	if (currentTimeBase > timestep)
	{
		firstAnimCurrentFrame += 1;
		currentTimeBase = 0;
	}
	if (currentTimeLayered > timestep)
	{
		secondAnimCurrentFrame += 1;
		currentTimeLayered = 0;
	}
	Matrix4x4f identity{};
	BlendHierarchy(
		firstAnimCurrentFrame - 1, 
		secondAnimCurrentFrame - 1, 
		currentTimeBase / timestep, 
		currentTimeLayered / timestep,
		blendState.alpha,
		0, 
		&animA, 
		&animB, 
		identity, 
		&myBoneTransforms[0]);


	if (secondAnimCurrentFrame >= animB.frames.size())
	{
		secondAnimCurrentFrame = 1;
	}
	if (firstAnimCurrentFrame >= animA.frames.size())
	{
		firstAnimCurrentFrame = 1;
	}
}

void Engine::AnimatedMesh::UpdateHierarchy(size_t aCurrentFrame, uint32_t aBoneIndex, const Animation* aAnim, Matrix4x4f& aParentTransform, Matrix4x4f* outBoneTransforms)
{
	outBoneTransforms[aBoneIndex] = aParentTransform * aAnim->frames[aCurrentFrame].localTransforms[aBoneIndex];
	for (auto& child : mySkeleton.bones[aBoneIndex].childrenIndices)
	{
		UpdateHierarchy(aCurrentFrame, child, aAnim, outBoneTransforms[aBoneIndex], outBoneTransforms);
	}
	outBoneTransforms[aBoneIndex] *= mySkeleton.bones[aBoneIndex].bindPoseInverse;
}

void Engine::AnimatedMesh::InterpolateHierarchy(size_t aCurrentFrame, float blendFactor, uint32_t aBoneIndex, const Animation* aAnim,
	Matrix4x4f& aParentTransform, Matrix4x4f* outBoneTransforms)
{
	Matrix4x4f interpolatedToTransform = aParentTransform * aAnim->frames[aCurrentFrame].localTransforms[aBoneIndex];
	Matrix4x4f interpolatedFromTransform = aParentTransform * aAnim->frames[(aCurrentFrame + 1) % (aAnim->frames.size() - 1)].localTransforms[aBoneIndex];



	outBoneTransforms[aBoneIndex] = interpolatedToTransform.Lerp(interpolatedFromTransform, blendFactor);
	for (auto& child : mySkeleton.bones[aBoneIndex].childrenIndices)
	{
		InterpolateHierarchy(aCurrentFrame, blendFactor, child, aAnim, outBoneTransforms[aBoneIndex], outBoneTransforms);
	}
	outBoneTransforms[aBoneIndex] *= mySkeleton.bones[aBoneIndex].bindPoseInverse;
}

void Engine::AnimatedMesh::BlendHierarchy(size_t aCurrentFrame, size_t aCurrentFrame2, float blendFactor1, float blendFactor2, float blendAlpha, uint32_t aBoneIndex,
	const Animation* aAnim, const Animation* aAnim2, Matrix4x4f& aParentTransform, Matrix4x4f* outBoneTransforms)
{

	Matrix4x4f interpolatedToTransform = aParentTransform * aAnim->frames[aCurrentFrame].localTransforms[aBoneIndex];
	Matrix4x4f interpolatedFromTransform = aParentTransform * aAnim->frames[(aCurrentFrame + 1) % (aAnim->frames.size() - 1)].localTransforms[aBoneIndex];

	Matrix4x4f interpolatedTranform = interpolatedToTransform.Lerp(interpolatedFromTransform, blendFactor1);


	Matrix4x4f interpolatedToTransform2 = aParentTransform * aAnim2->frames[aCurrentFrame2].localTransforms[aBoneIndex];
	Matrix4x4f interpolatedFromTransform2 = aParentTransform * aAnim2->frames[(aCurrentFrame2 + 1) % (aAnim2->frames.size() - 1)].localTransforms[aBoneIndex];

	Matrix4x4f interpolatedTranform2 = interpolatedToTransform2.Lerp(interpolatedFromTransform2, blendFactor2);

	outBoneTransforms[aBoneIndex] = Blender(interpolatedTranform, interpolatedTranform2, blendAlpha);
	for (auto& child : mySkeleton.bones[aBoneIndex].childrenIndices)
	{
		BlendHierarchy(aCurrentFrame, aCurrentFrame2, blendFactor1, blendFactor2, blendAlpha, child, aAnim, aAnim2,outBoneTransforms[aBoneIndex], outBoneTransforms);
	}
	outBoneTransforms[aBoneIndex] *= mySkeleton.bones[aBoneIndex].bindPoseInverse;
}

void Engine::AnimatedMesh::ProcessMaterials(const std::filesystem::path& path, const std::string& name, std::vector<Ref<Material>>& outMaterials)
{
	auto dir = path.parent_path();
	Ref<Material> tempMaterial = Material::Create(name);

	std::string matName = name;
	std::string DiffusePath = path.parent_path().string() + "/" + matName + "_C.dds";
	std::string NormalPath = path.parent_path().string() + "/" + matName + "_N.dds";
	std::string SpecularPath = path.parent_path().string() + "/" + matName + "_M.dds";
	tempMaterial->SetShader("AnimatedForward");
	if (std::filesystem::exists(DiffusePath))
	{
		tempMaterial->AddDiffuseTexture(Texture2D::Create(DiffusePath));
	}
	else
	{
		tempMaterial->AddDiffuseTexture(Texture2D::Create("Assets/Textures/T_Default_C.dds"));
	}
	if (std::filesystem::exists(NormalPath))
	{
		tempMaterial->AddNormalTexture(Texture2D::Create(NormalPath));
	}
	else
	{
		tempMaterial->AddNormalTexture(Texture2D::Create("Assets/Textures/T_Default_N.dds"));
	}
	if (std::filesystem::exists(SpecularPath))
	{
		tempMaterial->AddSpecularTexture(Texture2D::Create(SpecularPath));
	}
	else
	{
		uint32_t white = 0xffffffff;
		tempMaterial->AddSpecularTexture(Texture2D::Create("Assets/Textures/T_Default_M.dds"));
	}

	outMaterials.push_back(tempMaterial);
}

void Engine::AnimatedMesh::LoadAnimation(const std::filesystem::path& aPath, std::vector<Ref<Material>>& outMaterials)
{
	TGA::FBXModel FBXmodel;
	TGA::FBXImporter::LoadModel(aPath.string(), FBXmodel);
	for (size_t i = 0; i < FBXmodel.Meshes.size(); i++)
	{
		ProcessMaterials(aPath, FBXmodel.Meshes[i].MaterialName, outMaterials);
		std::vector<Vertex> vertex;
		std::vector<DWORD> index;

		for (size_t j = 0; j < FBXmodel.Meshes[i].Vertices.size(); j++)
		{
			Vertex v;
			memcpy(&v, &FBXmodel.Meshes[i].Vertices[j], sizeof(Vertex));
			vertex.push_back(v);
		}

		for (size_t j = 0; j < FBXmodel.Meshes[i].Indices.size(); j++)
		{
			index.push_back(FBXmodel.Meshes[i].Indices[j]);
		}

		mySubMeshes.emplace_back(vertex, index, outMaterials.back());
	}
	if (FBXmodel.Skeleton.GetRoot())
	{
		mySkeleton.name = FBXmodel.Name;
		for (size_t j = 0; j < FBXmodel.Skeleton.Bones.size(); ++j)
		{
			Matrix4x4f bindPoseInverse;
			memcpy(&bindPoseInverse, FBXmodel.Skeleton.Bones[j].BindPoseInverse.Data, sizeof(float) * 16);
			mySkeleton.bones.emplace_back(bindPoseInverse, FBXmodel.Skeleton.Bones[j].Parent, FBXmodel.Skeleton.Bones[j].Name, FBXmodel.Skeleton.Bones[j].Children);
			mySkeleton.boneNameToIndex[FBXmodel.Skeleton.Bones[j].Name] = FBXmodel.Skeleton.BoneNameToIndex[FBXmodel.Skeleton.Bones[j].Name];
			mySkeleton.boneNames.emplace_back(FBXmodel.Skeleton.Bones[j].Name);
		}
	}
}

Matrix4x4f Engine::AnimatedMesh::Blender(Matrix4x4f& aFirstBone, Matrix4x4f& aSecondBone, float aBlendTime)
{
	return aFirstBone.Lerp(aSecondBone, aBlendTime);
}