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

		Camera* ActiveCamera;
		Ref<FrameBuffer> RendererFinalframeBuffer;
		Ref<FrameBuffer> defferedGBuffer;
		Ref<FrameBuffer> particleGBuffer;
		CameraBuffer CameraBufferObject;
		ConstantBuffer<CameraBuffer> CameraBuffer;

		ModelBuffer ModelBufferObject;
		ConstantBuffer<ModelBuffer> ModelBuffer;

		DirectionalLightBuffer DirectionalLightBufferObject;
		ConstantBuffer<DirectionalLightBuffer> directionalLightBuffer;

		PointLightBuffer PointLightBufferObject;
		ConstantBuffer<PointLightBuffer> pointLightBuffer;
		uint32_t pointLightIterator = 0;

		SpotLightBuffer spotLightBufferObject;
		ConstantBuffer<SpotLightBuffer> spotLightBuffer;
		uint32_t spotLightIterator = 0;
	};

	class Renderer
	{
	public:
		static void Initialize();
		static void SetActiveCamera(Ref<Camera>& camera);
		static void SubmitDirectionalLight(Ref<DirectionalLight> light);
		static void SubmitPointLight(const PointLightData& light);
		static void SubmitSpotLight(const SpotLightData& light);
		static void SetIBL(Ref<Texture2D> map);
		static void SubmitMesh(Model* mesh);
		static void SubmitAnimatedMesh(AnimatedModel* mesh);
		static void SubmitParticleSystem(Ref<ParticleSystem> sys);
		static Ref<FrameBuffer> GetMainFramebuffer();
		static Ref<FrameBuffer> GetPaticleFramebuffer();
		static void Begin();
		static void Shutdown();
	private:
		static void DefineShaders();
		static void DefineFrameBuffers();
		static void DefineBuffers();
	};
}
