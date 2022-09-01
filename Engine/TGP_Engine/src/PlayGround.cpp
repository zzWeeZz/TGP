#include "PlayGround.h"

#include "Engine/Renderer/Renderer.h"
#include "ToolBox/Utility/Chrono.h"
#include "ToolBox/Input/Input.h"
#include "Engine/ShaderClasses/ShaderLibrary.h"
#include "imgui.h"

void Engine::PlayGround::Init()
{

	myInterpModel = AnimatedModel::Create("Model/Gremlin/gremlin_sk.fbx");
	myInterpModel->AddAnimation({ "Model/Gremlin/gremlin@run.fbx", "runint", true, true });

	myInterpModel->GetTransform().SetScale(1.f, 1.f, 1.f);
	myInterpModel->GetTransform().SetPosition(-50, 0, 0);
	myInterpModel->PlayAnimation("runint");


	{
		myBlendedModel = AnimatedModel::Create("Model/Gremlin/gremlin_sk.fbx");
		myBlendedModel->AddAnimation({ "Model/Gremlin/gremlin@run.fbx", "runint", true, true });
		myBlendedModel->AddAnimation({ "Model/Gremlin/gremlin@walk.fbx", "runnorm", true, true });

		myBlendedModel->GetTransform().SetScale(1.f, 1.f, 1.f);
		myBlendedModel->GetTransform().SetPosition(0, 0, 0);
	}

	{
		myKeyModel = AnimatedModel::Create("Model/Gremlin/gremlin_sk.fbx");
		myKeyModel->AddAnimation({ "Model/Gremlin/gremlin@run.fbx", "runint", true, false });

		myKeyModel->GetTransform().SetScale(1.f, 1.f, 1.f);
		myKeyModel->GetTransform().SetPosition(50, 0, 0);
		myKeyModel->PlayAnimation("runint");

		auto matkey = myKeyModel->GetMaterials()[0];
	}

	myPrimative = Model::Create("Cube");

	myPrimative->GetTransform().SetScale(10.f, 10.0f, 10.0f);
	myPrimative->GetTransform().SetPosition(0, 2, -20);

	{
		myChest = Model::Create("Model/SM_Particle_Chest.fbx");

		myChest->GetTransform().SetScale(0.5f, 0.5f, 0.5f);
		myChest->GetTransform().SetPosition(-50, 50, 50);
	}

	myCamera = Camera::Create(90, { 1280, 720 }, 0.1f, 100000.f);

	myDirLight = DirectionalLight::Create();
	myDirLight->SetColor({ 1,1,1 });
	myDirLight->SetDirection({ 0, -1,-1 });
	myDirLight->SetIntensity(1);

	myIBL = Texture2D::Create("Textures/skansen_cubemap.dds");
	myParticleSystem = ParticleSystem::Create("agja");
	Renderer::SetIBL(myIBL);

}

void Engine::PlayGround::Update()
{
	static float angle = 0;
	angle += ToolBox::Chrono::Timestep();
	static float blend = 0.5f;
	if (ToolBox::Input::IsKeyDown(Keys::Left))
	{
		blend -= ToolBox::Chrono::Timestep();
		if (blend < 0) blend = 0;
	}
	if (ToolBox::Input::IsKeyDown(Keys::Right))
	{
		blend += ToolBox::Chrono::Timestep();
		if (blend > 1) blend = 1;
	}
	myBlendedModel->BlendBetween({ "runnorm", "runint", blend });
	myInterpModel->RunAnimation();
	myKeyModel->RunAnimation();
	myCamera->Update(ToolBox::Chrono::Timestep());
	myPrimative->GetTransform().SetRotation(0, angle, 0);
	myBlendedModel->GetTransform().SetRotation(0, angle, 0);
	myParticleSystem->Run();
}

void Engine::PlayGround::Render()
{
	//Renderer::SetActiveCamera(myCamera);
	Renderer::SubmitMesh(myPrimative.get());
	Renderer::SubmitMesh(myChest.get());
	Renderer::SubmitAnimatedMesh(myBlendedModel.get());
	Renderer::SubmitAnimatedMesh(myInterpModel.get());
	Renderer::SubmitAnimatedMesh(myKeyModel.get());
	Renderer::SubmitDirectionalLight(myDirLight);
	Renderer::SubmitParticleSystem(myParticleSystem);

}
