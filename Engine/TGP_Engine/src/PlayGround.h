#pragma once
#include "Engine/Utils/Utilities.h"

#include "Engine/ShaderClasses/PixelShader.h"
#include "Engine/ShaderClasses/VertexShader.h"
#include "Engine/Assets/Model/Model.h"
#include "Engine/Assets/Model/AnimatedModel.h"
#include "Engine/Assets/Texture/Texture2D.h"
#include "Engine/Assets/Lights/DirectionalLight.h"
#include "Engine/Assets/ParticleSystem/ParticleSystem.h"

#include "Engine/Renderer/Camera/Camera.h"
#include "Engine/Renderer/Buffers/BufferData.h"
#include "Engine/Renderer/Buffers/ConstantBuffer.hpp"

namespace Engine
{
	class PlayGround
	{

	public:
		void Init();
		void Update();
		void Render();
	private:
		Ref<Model> myChest;
		Ref<Model> myPrimative;

		Ref<Texture2D> myTextureTest;
		Ref<AnimatedModel> myBlendedModel;
		Ref<AnimatedModel> myInterpModel;
		Ref<AnimatedModel> myKeyModel;
		Ref<Camera> myCamera;
		Ref<ParticleSystem> myParticleSystem;
		Ref<Texture2D> myIBL;

		Ref<DirectionalLight> myDirLight;
	};
}