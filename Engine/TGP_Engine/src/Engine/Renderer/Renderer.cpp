#include "Renderer.h"

#include "Engine/ShaderClasses/GeometryShader.h"
#include "Engine/ShaderClasses/PixelShader.h"
#include "Engine/ShaderClasses/ShaderLibrary.h"
#include "Engine/ShaderClasses/VertexShader.h"
#include "Engine/Utils/Utilities.h"

namespace Engine
{
	static Scope<Data> s_Data = CreateScope<Data>();
	void Renderer::Initialize()
	{
		DefineShaders();
		DefineFrameBuffers();
		DefineBuffers();
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Wrap, ShaderType::Pixel);
	}

	void Renderer::SubmitMesh(Model* mesh)
	{
		s_Data->Meshes.emplace_back(mesh);
	}

	void Renderer::SubmitLine(const Line& line)
	{
		{
			auto& start = s_Data->lineVertices[s_Data->lineIterator];
			start.position = line.startPos;
			start.color = line.color;
			s_Data->lineIterator++;
		}
		{
			auto& start = s_Data->lineVertices[s_Data->lineIterator];
			start.position = line.endPos;
			start.color = line.color;
			s_Data->lineIterator++;
		}

	}

	void Renderer::SetActiveCamera(Ref<Camera>& camera)
	{
		s_Data->ActiveCamera = camera.get();
	}

	void Renderer::SubmitDirectionalLight(const DirLightData& light)
	{
		s_Data->DirectionalLightBufferObject.dirLightData[s_Data->dirLightIterator] = light;
		s_Data->dirLightIterator++;
	}

	void Renderer::SubmitPointLight(const PointLightData& light)
	{
		s_Data->PointLightBufferObject.pointLightData[s_Data->pointLightIterator] = light;
		s_Data->pointLightIterator++;
	}

	void Renderer::SubmitSpotLight(const SpotLightData& light)
	{
		s_Data->spotLightBufferObject.spotLightData[s_Data->spotLightIterator] = light;
		s_Data->spotLightIterator++;
	}

	void Renderer::SetIBL(Ref<Texture2D> map)
	{
		s_Data->map = map.get();
		s_Data->map->Bind(30, ShaderType::Pixel);
	}

	void Renderer::SubmitAnimatedMesh(AnimatedModel* mesh)
	{
		s_Data->AnimatedMeshes.emplace_back(mesh);
	}

	int Renderer::GetClickedEntityId(uint32_t x, uint32_t y)
	{
		return s_Data->defferedGBuffer->ReadPixel<int>(6, x, y);
	}

	void Renderer::SubmitParticleSystem(Ref<ParticleSystem> sys)
	{
		s_Data->ParticleSystem.emplace_back(sys.get());
	}

	Ref<FrameBuffer> Renderer::GetMainFramebuffer()
	{
		return s_Data->RendererFinalframeBuffer;
	}

	Ref<FrameBuffer> Renderer::GetPaticleFramebuffer()
	{
		return nullptr;
	}

	void Renderer::Begin()
	{
		s_Data->RendererFinalframeBuffer->Clear();
		s_Data->defferedGBuffer->Clear({ 0,0,0,0 });
		if (s_Data->RendererFinalframeBuffer->GetSpecs().width != s_Data->defferedGBuffer->GetSpecs().width || s_Data->RendererFinalframeBuffer->GetSpecs().height != s_Data->defferedGBuffer->GetSpecs().height)
		{
			s_Data->defferedGBuffer->Resize({ (float)s_Data->RendererFinalframeBuffer->GetSpecs().width , (float)s_Data->RendererFinalframeBuffer->GetSpecs().height });
		}


		s_Data->directionalLightBuffer.SetData(&s_Data->DirectionalLightBufferObject);
		s_Data->directionalLightBuffer.Bind(2);

		s_Data->pointLightBuffer.SetData(&s_Data->PointLightBufferObject);
		s_Data->pointLightBuffer.Bind(3);

		s_Data->spotLightBuffer.SetData(&s_Data->spotLightBufferObject);
		s_Data->spotLightBuffer.Bind(4);

		DX11::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		BeginShadowPass();

		if (s_Data->ActiveCamera)
		{
			s_Data->CameraBufferObject.cameraSpace = s_Data->ActiveCamera->GetLookAt();
			s_Data->CameraBufferObject.toProjectionSpace = s_Data->ActiveCamera->GetProjection();
			s_Data->CameraBufferObject.cameraPosition = Vector4f(s_Data->ActiveCamera->GetPos().x, s_Data->ActiveCamera->GetPos().y, s_Data->ActiveCamera->GetPos().z, 1.0f);
			s_Data->cameraBuffer.SetData(&s_Data->CameraBufferObject);
			s_Data->cameraBuffer.Bind(0);
		}
		s_Data->shadowCube->BindToShader(11);
		BeginDeferredPass();

		// ----------------------------------------------------------------------- //
		// ----------------------- FORWARD RENDERING ----------------------------- //
		// ----------------------------------------------------------------------- //

		DX11::GetRenderStateManager().PushDepthStencilState(DepthStencilMode::ReadOnly);
		DX11::GetRenderStateManager().PushBlendState(BlendMode::AlphaBlend);
		ShaderLibrary::Bind("Grid");
		DX11::Context()->Draw(6, 0);
		ShaderLibrary::UnBind("Grid");
		ShaderLibrary::Bind("Particle");
		for (auto sys : s_Data->ParticleSystem)
		{
			sys->Bind();
			sys->Draw();
		}
		ShaderLibrary::UnBind("Particle");

		DX11::GetRenderStateManager().PopDepthStencilState();

		DX11::GetRenderStateManager().PopBlendState();

		BeginLineDraw();

		s_Data->RendererFinalframeBuffer->UnBind();
		// ----------------------------------------------------------------------- //
		// ------------------------ DONE RENDERING ------------------------------- //
		// ----------------------------------------------------------------------- //


		FlushLineBatch();
		Flush();
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::Flush()
	{
		s_Data->Meshes.clear();
		s_Data->pointLightIterator = 0;
		s_Data->spotLightIterator = 0;
		s_Data->dirLightIterator = 0;
		memset(&s_Data->PointLightBufferObject, 0, sizeof(PointLightBuffer));
		memset(&s_Data->spotLightBufferObject, 0, sizeof(SpotLightBuffer));
		memset(&s_Data->DirectionalLightBufferObject, 0, sizeof(DirLightBuffer));
		s_Data->AnimatedMeshes.clear();
		s_Data->ParticleSystem.clear();
	}

	void Renderer::BeginShadowPass()
	{
		DX11::GetRenderStateManager().PushRasterizerState(CullMode::Back);
		for (size_t i = 0; i < s_Data->dirLightIterator; ++i)
		{
			s_Data->dirLightFBs[i]->Clear();
			s_Data->dirLightFBs[i]->Bind();
			s_Data->CameraBufferObject.cameraSpace = s_Data->DirectionalLightBufferObject.dirLightData[i].view;
			s_Data->CameraBufferObject.toProjectionSpace = s_Data->DirectionalLightBufferObject.dirLightData[i].proj;
			s_Data->CameraBufferObject.cameraPosition = Vector4f(s_Data->ActiveCamera->GetPos().x, s_Data->ActiveCamera->GetPos().y, s_Data->ActiveCamera->GetPos().z, 1.0f);
			s_Data->cameraBuffer.SetData(&s_Data->CameraBufferObject);
			s_Data->cameraBuffer.Bind(0);
			ShaderLibrary::Bind("DirShadowPass");
			for (auto mesh : s_Data->Meshes)
			{
				s_Data->ModelBufferObject.entityId = { (int)mesh->GetEntity(), 0, 0, 0 };
				s_Data->ModelBufferObject.modelSpace = (mesh->GetTransform().GetMatrix());
				s_Data->modelBuffer.SetData(&s_Data->ModelBufferObject);
				s_Data->modelBuffer.Bind(1);
				mesh->Draw();
			}
			ShaderLibrary::Bind("AnimDirShadowPass");

			for (auto mesh : s_Data->AnimatedMeshes)
			{
				s_Data->ModelBufferObject.modelSpace = (mesh->GetTransform().GetMatrix());
				memcpy_s(s_Data->ModelBufferObject.bones, sizeof(s_Data->ModelBufferObject.bones), mesh->GetAnimatedMesh().GetCurrentTransforms().data(), sizeof(s_Data->ModelBufferObject.bones));
				s_Data->modelBuffer.SetData(&s_Data->ModelBufferObject);
				s_Data->modelBuffer.Bind(1);
				mesh->Draw();
			}
			s_Data->dirLightFBs[i]->UnBind();
		}
		s_Data->shadowCube->Clear();
		for (size_t i = 0; i < s_Data->pointLightIterator; ++i)
		{
			PointShadowData data{};
			data.farPlane = s_Data->PointLightBufferObject.pointLightData[i].radius;
			data.lightPos = s_Data->PointLightBufferObject.pointLightData[i].position;
			memcpy(&data.mat[0], &s_Data->PointLightBufferObject.pointLightData[i].transforms[0], sizeof(Matrix4x4f) * 6);
			s_Data->pointShadowBuffer.SetData(&data);
			s_Data->pointShadowBuffer.Bind(8);
			s_Data->shadowCube->Bind();
			ShaderLibrary::Bind("PointShadowPass");
			for (auto mesh : s_Data->Meshes)
			{
				s_Data->ModelBufferObject.entityId = { (int)mesh->GetEntity(), 0, 0, 0 };
				s_Data->ModelBufferObject.modelSpace = (mesh->GetTransform().GetMatrix());
				s_Data->modelBuffer.SetData(&s_Data->ModelBufferObject);
				s_Data->modelBuffer.Bind(1);
				mesh->Draw();
			}
			ShaderLibrary::UnBind("PointShadowPass");
			ShaderLibrary::Bind("AnimPointShadowPass");

			for (auto mesh : s_Data->AnimatedMeshes)
			{
				s_Data->ModelBufferObject.modelSpace = (mesh->GetTransform().GetMatrix());
				memcpy_s(s_Data->ModelBufferObject.bones, sizeof(s_Data->ModelBufferObject.bones), mesh->GetAnimatedMesh().GetCurrentTransforms().data(), sizeof(s_Data->ModelBufferObject.bones));
				s_Data->modelBuffer.SetData(&s_Data->ModelBufferObject);
				s_Data->modelBuffer.Bind(1);
				mesh->Draw();
			}
			ShaderLibrary::UnBind("AnimPointShadowPass");
			s_Data->shadowCube->UnBind();
		}
		DX11::GetRenderStateManager().PopRasterizerState();
	}

	void Renderer::BeginDeferredPass()
	{
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Point, ShaderType::Pixel, 1);

		s_Data->dirLightFBs[0]->BindDepthToShader(10);
		s_Data->defferedGBuffer->Bind();
		ShaderLibrary::Bind("DefferedPBR");
		for (auto mesh : s_Data->Meshes)
		{
			s_Data->ModelBufferObject.entityId = { (int)mesh->GetEntity(), 0, 0, 0 };
			s_Data->ModelBufferObject.modelSpace = (mesh->GetTransform().GetMatrix());
			s_Data->modelBuffer.SetData(&s_Data->ModelBufferObject);
			s_Data->modelBuffer.Bind(1);
			mesh->Draw();
		}
		ShaderLibrary::Bind("AnimatedDefferedPBR");

		for (auto mesh : s_Data->AnimatedMeshes)
		{
			s_Data->ModelBufferObject.modelSpace = (mesh->GetTransform().GetMatrix());
			memcpy_s(s_Data->ModelBufferObject.bones, sizeof(s_Data->ModelBufferObject.bones), mesh->GetAnimatedMesh().GetCurrentTransforms().data(), sizeof(s_Data->ModelBufferObject.bones));
			s_Data->modelBuffer.SetData(&s_Data->ModelBufferObject);
			s_Data->modelBuffer.Bind(1);
			mesh->Draw();
		}
		s_Data->defferedGBuffer->UnBind();
		s_Data->RendererFinalframeBuffer->Bind();
		s_Data->defferedGBuffer->BindToShader(0, 0);
		s_Data->defferedGBuffer->BindToShader(1, 1);
		s_Data->defferedGBuffer->BindToShader(2, 2);
		s_Data->defferedGBuffer->BindToShader(3, 3);
		s_Data->defferedGBuffer->BindToShader(4, 4);
		s_Data->defferedGBuffer->BindToShader(5, 5);
		ShaderLibrary::Bind("DeferredLightCalc");

		DX11::GetRenderStateManager().PushDepthStencilState(DepthStencilMode::None);
		DX11::Context()->Draw(3, 0);
		s_Data->RendererFinalframeBuffer->TransferDepth(s_Data->defferedGBuffer);
		DX11::GetRenderStateManager().PopDepthStencilState();
	}

	void Renderer::FlushLineBatch()
	{
		memset(s_Data->lineVertices.data(), 0, s_Data->lineVertices.size() * sizeof(LineVertex));
		s_Data->lineIterator = 0;
	}

	void Renderer::BeginLineDraw()
	{
		ShaderLibrary::Bind("Line");
		DX11::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		s_Data->lineVertexBuffer.SetData(s_Data->lineVertices.data(), s_Data->lineVertices.size());
		s_Data->lineVertexBuffer.Bind();
		DX11::Context()->DrawInstanced(2, s_Data->lineIterator / 2, 0, 0);
		ShaderLibrary::UnBind("Line");
	}

	void Renderer::DefineShaders()
	{
		{
			InputLayout newLayout
			{
				{"POSITION", 0, Value::Float4},
				{"COLOR", 0, Value::Float4},
				{"COLOR", 1, Value::Float4},
				{"COLOR", 2, Value::Float4},
				{"COLOR", 3, Value::Float4},
				{"UV", 0, Value::Float2},
				{"UV", 1, Value::Float2},
				{"UV", 2, Value::Float2},
				{"UV", 3, Value::Float2},
				{"NORMAL", 0, Value::Float3},
				{"TANGENT", 0, Value::Float3},
				{"BITANGENT", 0, Value::Float3},
				{"BONEIDS", 0, Value::UInt4},
				{"BONEWEIGHTS", 0, Value::Float4},
				{"ENTITYID", 0, Value::UInt},
			};

			ShaderLibrary::AddToLibrary("AnimatedForwardPBR",
				{
					PixelShader::Create("Shaders/forward_ps.cso"),
					VertexShader::Create("Shaders/forwardAnimated_vs.cso", newLayout)
				});
			ShaderLibrary::AddToLibrary("AnimatedDefferedPBR",
				{
					PixelShader::Create("Shaders/GenerateDefferedImages_ps.cso"),
					VertexShader::Create("Shaders/forwardAnimated_vs.cso", newLayout)
				});
			ShaderLibrary::AddToLibrary("AnimDirShadowPass",
				{
					VertexShader::Create("Shaders/AnimDirShadow_vs.cso", newLayout)
				});
			
		}
		{
			InputLayout newLayout
			{
				{"POSITION", 0, Value::Float4},
				{"COLOR", 0, Value::Float4},
				{"COLOR", 1, Value::Float4},
				{"COLOR", 2, Value::Float4},
				{"COLOR", 3, Value::Float4},
				{"UV", 0, Value::Float2},
				{"UV", 1, Value::Float2},
				{"UV", 2, Value::Float2},
				{"UV", 3, Value::Float2},
				{"NORMAL", 0, Value::Float3},
				{"TANGENT", 0, Value::Float3},
				{"BITANGENT", 0, Value::Float3},
				{"ENTITYID", 0, Value::UInt},
			};
			ShaderLibrary::AddToLibrary("ForwardPBR",
				{
					PixelShader::Create("Shaders/forward_ps.cso"),
					VertexShader::Create("Shaders/forward_vs.cso", newLayout)
				});
			ShaderLibrary::AddToLibrary("DefferedPBR",
				{
					PixelShader::Create("Shaders/GenerateDefferedImages_ps.cso"),
					VertexShader::Create("Shaders/forward_vs.cso", newLayout)
				});
		}
		{
			InputLayout newLayout
			{
				{"POSITION", 0, Value::Float4},
			};
			ShaderLibrary::AddToLibrary("DirShadowPass",
				{
					VertexShader::Create("Shaders/DirShadow_vs.cso", newLayout)
				});
			
		}
		{
			InputLayout newLayout
			{
				{"POSITION", 0, Value::Float4},
				{"INPUT", 0, Value::UInt}
			};
			ShaderLibrary::AddToLibrary("PointShadowPass",
				{
					VertexShader::Create("Shaders/PointShadow_vs.cso", newLayout),
					GeometryShader::Create("Shaders/PointlightGen_gs.cso"),
				PixelShader::Create("Shaders/PointShadow_ps.cso"),
				});
			ShaderLibrary::AddToLibrary("AnimPointShadowPass",
				{
					VertexShader::Create("Shaders/PointShadow_vs.cso", newLayout),
					GeometryShader::Create("Shaders/PointlightGen_gs.cso"),
					PixelShader::Create("Shaders/PointShadow_ps.cso"),

				});
		}

		{
			InputLayout newLayout
			{
				{"POSITION", 0, Value::Float4},
				{"COLOR", 0, Value::Float4},

				{"VELOCITY", 0, Value::Float3},
				{"SCALE", 0, Value::Float3},
				{"LIFETIME", 0, Value::Float},
			};
			ShaderLibrary::AddToLibrary("Particle",
				{
					PixelShader::Create("Shaders/ParticleSystem_ps.cso"),
					GeometryShader::Create("Shaders/QuadGenerator_gs.cso"),
					VertexShader::Create("Shaders/ParticleSystem_vs.cso", newLayout)
				});
		}
		{
			InputLayout newLayout
			{
				{"POSITION", 0, Value::Float4},
				{"UV", 0, Value::Float2},
			};
			ShaderLibrary::AddToLibrary("DeferredLightCalc",
				{
					PixelShader::Create("Shaders/DefferedLightCalc_ps.cso"),
					VertexShader::Create("Shaders/DefferedLightCalc_vs.cso", newLayout)
				});
		}
		{
			InputLayout newLayout
			{
				{"NEARPOINT", 0, Value::Float3},
				{"FARPOINT", 0, Value::Float3},
				{"FRAGVIEW", 0, Value::Float4X4},
				{"FRAGPROJ", 0, Value::Float4X4},
			};
			ShaderLibrary::AddToLibrary("Grid",
				{
					PixelShader::Create("Shaders/Grid_ps.cso"),
					VertexShader::Create("Shaders/Grid_vs.cso", newLayout)
				});
		}

		{
			InputLayout newLayout
			{
				{"POSITION", 0, Value::Float4},
				{"COLOR", 0, Value::Float4},
				{"INSTANCE", 0, Value::Float3, PerData::Instance}
			};
			ShaderLibrary::AddToLibrary("Line",
				{
					PixelShader::Create("Shaders/Line_ps.cso"),
					VertexShader::Create("Shaders/Line_vs.cso", newLayout)
				});
		}
	}

	void Renderer::DefineFrameBuffers()
	{
		{
			FrameBufferSpecs specs{};
			specs.height = 720;
			specs.width = 1280;
			specs.formats = { ImageFormat::RGBA32F, ImageFormat::Depth32 };
			s_Data->RendererFinalframeBuffer = FrameBuffer::Create(specs);
		}
		{
			CubeBufferSpecs specs{};
			specs.height = 1024;
			specs.width = 1024;
			s_Data->shadowCube = CubeBuffer::Create(specs);
		}

		{
			FrameBufferSpecs specs{};
			specs.height = 720;
			specs.width = 1280;
			specs.formats = { ImageFormat::R8G8B8A8UN, ImageFormat::RGBA16SN, ImageFormat::R8G8B8A8UN, ImageFormat::RGBA16SN, ImageFormat::RGBA32F, ImageFormat::R8UN, ImageFormat::R32UI, ImageFormat::Depth32 };
			s_Data->defferedGBuffer = FrameBuffer::Create(specs);
		}
		FrameBufferSpecs specs{};
		specs.width = 8192;
		specs.height = 8192;
		specs.formats = { ImageFormat::R8UN, ImageFormat::Depth32 };
		s_Data->dirLightFBs[0] = FrameBuffer::Create(specs);
	}

	void Engine::Renderer::DefineBuffers()
	{
		s_Data->lineVertexBuffer.Initialize(s_Data->lineVertices.data(), s_Data->lineVertices.size());

		s_Data->pointShadowBuffer.Create();
		s_Data->cameraBuffer.Create();
		s_Data->modelBuffer.Create();
		s_Data->pointLightBuffer.Create();
		s_Data->directionalLightBuffer.Create();
		s_Data->spotLightBuffer.Create();
	}
}
