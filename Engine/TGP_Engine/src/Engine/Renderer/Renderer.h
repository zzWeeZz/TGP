#pragma once
#include "Buffers/BufferData.h"
#include "Buffers/ConstantBuffer.hpp"
#include "Engine/DX11/DX11.h"
#include "Engine/Renderer/Mesh/ModelData.hpp"
#include "Engine/Assets/Model/Model.h"
#include "Engine/Assets/Model/AnimatedModel.h"
#include "Engine/Assets/Lights/DirectionalLight.h"
#include "Engine/Assets/ParticleSystem/ParticleSystem.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Camera/Camera.h"

namespace Engine
{
	struct Data
	{
		std::vector<Model*> Meshes;
		std::vector<AnimatedModel*> AnimatedMeshes;
		std::vector<ParticleSystem*> ParticleSystem;
		DirectionalLight* DirectionalLight;
		Texture2D* map;

		Ref<Camera> ActiveCamera;
		Ref<FrameBuffer> frameBuffer;
		Ref<FrameBuffer> defferedGBuffer;
		CameraBuffer CameraBufferObject;
		ConstantBuffer<CameraBuffer> CameraBuffer;

		ModelBuffer ModelBufferObject;
		ConstantBuffer<ModelBuffer> ModelBuffer;

		DirectionalLightBuffer DirectionalLightBufferObject;
		ConstantBuffer<DirectionalLightBuffer> DirectionalLightBuffer;
	};
	class Renderer
	{
	public:
		static void Initialize();
		static void SetActiveCamera(Ref<Camera> camera);
		static void SubmitDirectionalLight(Ref<DirectionalLight> light);
		static void SetIBL(Ref<Texture2D> map);
		static void SubmitMesh(Model* mesh);
		static void SubmitAnimatedMesh(AnimatedModel* mesh);
		static void SubmitParticleSystem(Ref<ParticleSystem> sys);
		static Ref<FrameBuffer> GetMainFramebuffer();
		static void Begin();
		static void Shutdown();
	};
}
