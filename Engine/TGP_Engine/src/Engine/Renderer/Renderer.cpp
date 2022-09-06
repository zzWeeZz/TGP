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
				{"BONEWEIGHTS", 0, Value::Float4}
			};

			ShaderLibrary::AddToLibrary("AnimatedForwardPBR", { PixelShader::Create("Shaders/forward_ps.cso"), VertexShader::Create("Shaders/forwardAnimated_vs.cso", newLayout) });
			ShaderLibrary::AddToLibrary("AnimatedDefferedPBR", { PixelShader::Create("Shaders/GenerateDefferedImages_ps.cso"), VertexShader::Create("Shaders/forwardAnimated_vs.cso", newLayout) });
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
			};
			ShaderLibrary::AddToLibrary("ForwardPBR", { PixelShader::Create("Shaders/forward_ps.cso"), VertexShader::Create("Shaders/forward_vs.cso", newLayout) });
			ShaderLibrary::AddToLibrary("DefferedPBR", { PixelShader::Create("Shaders/GenerateDefferedImages_ps.cso"), VertexShader::Create("Shaders/forward_vs.cso", newLayout) });
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
			ShaderLibrary::AddToLibrary("Particle", { PixelShader::Create("Shaders/ParticleSystem_ps.cso"), GeometryShader::Create("Shaders/QuadGenerator_gs.cso"),VertexShader::Create("Shaders/ParticleSystem_vs.cso", newLayout)});
		}
		{
			InputLayout newLayout
			{
				{"POSITION", 0, Value::Float4},
				{"UV", 0, Value::Float2},
			};
			ShaderLibrary::AddToLibrary("DeferredLightCalc", { PixelShader::Create("Shaders/DefferedLightCalc_ps.cso"), VertexShader::Create("Shaders/DefferedLightCalc_vs.cso", newLayout) });
		}
		{
			InputLayout newLayout
			{
				{"NEARPOINT", 0, Value::Float3},
				{"FARPOINT", 0, Value::Float3},
				{"FRAGVIEW", 0, Value::Float4X4},
				{"FRAGPROJ", 0, Value::Float4X4},
			};
			ShaderLibrary::AddToLibrary("Grid", { PixelShader::Create("Shaders/Grid_ps.cso"), VertexShader::Create("Shaders/Grid_vs.cso", newLayout) });
		}
		{
			FrameBufferSpecs specs{};
			specs.height = 720;
			specs.width = 1280;
			specs.formats = { ImageFormat::RGBA32F, ImageFormat::Depth32 };
			s_Data->RendererFinalframeBuffer = FrameBuffer::Create(specs);
		}
		{
			FrameBufferSpecs specs{};
			specs.height = 720;
			specs.width = 1280;
			specs.formats = { ImageFormat::RGBA32F, ImageFormat::Depth32 };
			s_Data->particleGBuffer = FrameBuffer::Create(specs);
		}
		{
			FrameBufferSpecs specs{};
			specs.height = 720;
			specs.width = 1280;
			specs.formats = { ImageFormat::R8G8B8A8UN, ImageFormat::RGBA16SN, ImageFormat::R8G8B8A8UN, ImageFormat::RGBA16SN, ImageFormat::RGBA32F, ImageFormat::R8UN, ImageFormat::Depth32 };
			s_Data->defferedGBuffer = FrameBuffer::Create(specs);
		}
		ShaderLibrary::Bind("DefferedPBR");
		s_Data->CameraBuffer.Create();
		s_Data->ModelBuffer.Create();
		s_Data->pointLightBuffer.Create();
		s_Data->directionalLightBuffer.Create();
		s_Data->spotLightBuffer.Create();
		DX11::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DX11::GetRenderStateManager().SetSamplerState(SamplerMode::Clamp, ShaderType::Pixel);
	}

	void Renderer::SubmitMesh(Model* mesh)
	{
		s_Data->Meshes.emplace_back(mesh);
	}

	void Renderer::SetActiveCamera(Ref<Camera> camera)
	{
		s_Data->ActiveCamera = camera;
	}

	void Renderer::SubmitDirectionalLight(Ref<DirectionalLight> light)
	{
		s_Data->DirectionalLight = light.get();
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
		return s_Data->particleGBuffer;
	}

	void Renderer::Begin()
	{
		s_Data->RendererFinalframeBuffer->Clear();
		s_Data->defferedGBuffer->Clear({0,0,0,0});
		if (s_Data->RendererFinalframeBuffer->GetSpecs().width != s_Data->defferedGBuffer->GetSpecs().width || s_Data->RendererFinalframeBuffer->GetSpecs().height != s_Data->defferedGBuffer->GetSpecs().height)
		{
			s_Data->defferedGBuffer->Resize({ (float)s_Data->RendererFinalframeBuffer->GetSpecs().width , (float)s_Data->RendererFinalframeBuffer->GetSpecs().height });
		}
		if (s_Data->ActiveCamera)
		{
			s_Data->CameraBufferObject.cameraSpace = s_Data->ActiveCamera->GetLookAt();
			s_Data->CameraBufferObject.toProjectionSpace =(s_Data->ActiveCamera->GetProjection());
			s_Data->CameraBufferObject.cameraPosition = Vector4f(s_Data->ActiveCamera->GetPos().x, s_Data->ActiveCamera->GetPos().y, s_Data->ActiveCamera->GetPos().z, 1.0f);
			s_Data->CameraBuffer.SetData(&s_Data->CameraBufferObject);
			s_Data->CameraBuffer.Bind(0);
		}
		if (s_Data->DirectionalLight)
		{
			s_Data->DirectionalLightBufferObject.direction = s_Data->DirectionalLight->myDirection;
			s_Data->DirectionalLightBufferObject.colorAndIntensity = s_Data->DirectionalLight->myColor;
			s_Data->directionalLightBuffer.SetData(&s_Data->DirectionalLightBufferObject);
			s_Data->directionalLightBuffer.Bind(2);
		}

		s_Data->pointLightBuffer.SetData(&s_Data->PointLightBufferObject);
		s_Data->pointLightBuffer.Bind(3);

		s_Data->spotLightBuffer.SetData(&s_Data->spotLightBufferObject);
		s_Data->spotLightBuffer.Bind(4);

		DX11::Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// ----------------------------------------------------------------------- //
		// ----------------------- DEFERRED RENDERING ---------------------------- //
		// ----------------------------------------------------------------------- //
		s_Data->defferedGBuffer->Bind();
		ShaderLibrary::Bind("DefferedPBR");
		for (auto mesh : s_Data->Meshes)
		{
			s_Data->ModelBufferObject.modelSpace = (mesh->GetTransform().GetMatrix());
			s_Data->ModelBuffer.SetData(&s_Data->ModelBufferObject);
			s_Data->ModelBuffer.Bind(1);
			mesh->Draw();
		}
		ShaderLibrary::Bind("AnimatedDefferedPBR");

		for (auto mesh : s_Data->AnimatedMeshes)
		{
			s_Data->ModelBufferObject.modelSpace = (mesh->GetTransform().GetMatrix());
			memcpy_s(s_Data->ModelBufferObject.bones, sizeof(s_Data->ModelBufferObject.bones), mesh->GetAnimatedMesh().GetCurrentTransforms().data(), sizeof(s_Data->ModelBufferObject.bones));
			s_Data->ModelBuffer.SetData(&s_Data->ModelBufferObject);
			s_Data->ModelBuffer.Bind(1);
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
		//s_Data->defferedGBuffer->BindToShader();
		DX11::GetRenderStateManager().PushDepthStencilState(DepthStencilMode::None);
		DX11::Context()->Draw(3, 0);
		s_Data->RendererFinalframeBuffer->TransferDepth(s_Data->defferedGBuffer);
		// ----------------------------------------------------------------------- //
		// ----------------------- FORWARD RENDERING ----------------------------- //
		// ----------------------------------------------------------------------- //
		DX11::GetRenderStateManager().PopDepthStencilState();
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

		s_Data->RendererFinalframeBuffer->UnBind();
		// ----------------------------------------------------------------------- //
		// ------------------------ DONE RENDERING ------------------------------- //
		// ----------------------------------------------------------------------- //

		

		s_Data->Meshes.clear();
		s_Data->pointLightIterator = 0;
		s_Data->spotLightIterator = 0;
		memset(&s_Data->PointLightBufferObject, 0, sizeof(PointLightBuffer));
		memset(&s_Data->spotLightBufferObject, 0, sizeof(SpotLightBuffer));
		s_Data->AnimatedMeshes.clear();
		s_Data->ParticleSystem.clear();
	}

	void Renderer::Shutdown()
	{
	}
}
