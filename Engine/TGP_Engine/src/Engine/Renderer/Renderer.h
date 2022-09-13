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
	struct Line
	{
		Vector3f startPos;
		Vector3f endPos;
		Vector4f color;
		Line()
		{
			startPos = {};
			endPos = { 0, 1, 0 };
			color = { 1,1,1,1 };
		}
		Line(const Vector3f& start, const Vector3f& end, const Vector4f& col)
		{
			startPos = start;
			endPos = end;
			color = col;
		}
	};
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
		Ref<FrameBuffer> colorPickingBuffer;
		
		
		CameraBuffer CameraBufferObject;
		ConstantBuffer<CameraBuffer> cameraBuffer;

		ModelBuffer ModelBufferObject;
		ConstantBuffer<ModelBuffer> modelBuffer;

		VertexBuffer<LineVertex> lineVertexBuffer;
		std::array<LineVertex, 512> lineVertices;
		uint32_t lineIterator;

		DirLightBuffer DirectionalLightBufferObject;
		ConstantBuffer<DirLightBuffer> directionalLightBuffer;
		uint32_t dirLightIterator = 0;

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
		static void SubmitDirectionalLight(const DirLightData& light);
		static void SubmitPointLight(const PointLightData& light);
		static void SubmitSpotLight(const SpotLightData& light);
		static void SetIBL(Ref<Texture2D> map);
		static void SubmitMesh(Model* mesh);
		static void SubmitLine(const Line& line);
		static void SubmitAnimatedMesh(AnimatedModel* mesh);
		static int GetClickedEntityId(uint32_t x, uint32_t y);
		static void SubmitParticleSystem(Ref<ParticleSystem> sys);
		static Ref<FrameBuffer> GetMainFramebuffer();
		static Ref<FrameBuffer> GetPaticleFramebuffer();
		static void Begin();
		static void Shutdown();
	private:
		static void Flush();
		static void FlushLineBatch();
		static void BeginLineDraw();
		static void DefineShaders();
		static void DefineFrameBuffers();
		static void DefineBuffers();
	};
}
