#include "Camera.h"
#include "ToolBox/Input/Input.h"

Engine::Camera::Camera(float fov, Vector2i size, float zNear, float zFar)
{
	myFov = fov;
	myAspect = (float)size.x/(float)size.y;
	myZNear = zNear;
	myZFar = zFar;
	myProjection = Matrix4x4f::CreateLeftHandPerspectiveMatrix(myFov, size, myZNear, myZFar);
}

void Engine::Camera::Update(float aDeltaTime)
{
	auto position = myTransform.GetPosition();
	if (ToolBox::Input::IsMouseDown(MouseButton::Right))
	{

		if (ToolBox::Input::IsKeyDown(Keys::A))
		{
			position -= myTransform.GetRight() * 200.f * aDeltaTime;
		}
		if (ToolBox::Input::IsKeyDown(Keys::D))
		{
			position += myTransform.GetRight() * 200.f * aDeltaTime;
		}
		if (ToolBox::Input::IsKeyDown(Keys::W))
		{
			position += myTransform.GetForward() * 200.f * aDeltaTime;
		}
		if (ToolBox::Input::IsKeyDown(Keys::S))
		{
			position -= myTransform.GetForward() * 200.f * aDeltaTime;
		}
		if (ToolBox::Input::IsKeyDown(Keys::Control))
		{
			position -= myTransform.GetUp() * 200.f * aDeltaTime;
		}
		if (ToolBox::Input::IsKeyDown(Keys::Space))
		{
			position += myTransform.GetUp() * 200.f * aDeltaTime;
		}
		CalcMouseMovement();
		myTransform.SetPosition(position);
	}
	else
	{
		myGiveControl = false;
	}
}

void Engine::Camera::SetAspectRatio(float x, float y)
{
	myProjection = Matrix4x4f::CreateLeftHandPerspectiveMatrix(myFov, {(int)x, (int)y}, myZNear, myZFar);
}

Engine::Ref<Engine::Camera> Engine::Camera::Create(float fov, Vector2i size, float zNear, float zFar)
{
	return CreateRef<Camera>(fov, size, zNear, zFar);
}

void Engine::Camera::CalcMouseMovement()
{
	auto mouseDelta = ToolBox::Input::GetMouseDelta();
	auto mouseSpeed = 0.005f;
	static auto rotation = myTransform.GetRotation();
	rotation += Vector3f(mouseDelta.y * mouseSpeed, mouseDelta.x * mouseSpeed, 0.f);
	myTransform.SetRotation(rotation);
}
