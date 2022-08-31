#include "Camera.h"
#include "ToolBox/Input/Input.h"
#include "ToolBox/Math/Matrix/Matrix.h"

Engine::Camera::Camera(float fov, Vector2i size, float zNear, float zFar)
{
	myFov = fov;
	myAspect = (float)size.x/(float)size.y;
	myZNear = zNear;
	myZFar = zFar;
	m_Forward = Vector3f(0.f, 0.f, -1.f);
	m_Right = Vector3f(-1.f, 0.f, 0.f);
	m_Up = Vector3f(0.f, 1.f, 0.f);
	m_Rotation = Vector3f(0.f, 0.f, 0.f);
	myProjection = Matrix4x4f::CreateLeftHandPerspectiveMatrix(myFov, size, myZNear, myZFar);
}

void Engine::Camera::Update(float aDeltaTime)
{
	
	if (ToolBox::Input::IsMouseDown(MouseButton::Right))
	{

		if (ToolBox::Input::IsKeyDown(Keys::A))
		{
			m_Position -= m_Right * 200.f * aDeltaTime;
			myLookat = Matrix4x4f::CreateLookAt(m_Position, m_Position + m_Forward, m_Up);
		}
		if (ToolBox::Input::IsKeyDown(Keys::D))
		{
			m_Position += m_Right * 200.f * aDeltaTime;
			myLookat = Matrix4x4f::CreateLookAt(m_Position, m_Position + m_Forward, m_Up);
		}
		if (ToolBox::Input::IsKeyDown(Keys::W))
		{
			m_Position += m_Forward * 200.f * aDeltaTime;
			myLookat = Matrix4x4f::CreateLookAt(m_Position, m_Position + m_Forward, m_Up);
		}
		if (ToolBox::Input::IsKeyDown(Keys::S))
		{
			m_Position -= m_Forward * 200.f * aDeltaTime;
			myLookat = Matrix4x4f::CreateLookAt(m_Position, m_Position + m_Forward, m_Up);
		}
		if (ToolBox::Input::IsKeyDown(Keys::Control))
		{
			m_Position -= m_Up * 200.f * aDeltaTime;
			myLookat = Matrix4x4f::CreateLookAt(m_Position, m_Position + m_Forward, m_Up);
		}
		if (ToolBox::Input::IsKeyDown(Keys::Space))
		{
			m_Position += m_Up * 200.f * aDeltaTime;
			myLookat = Matrix4x4f::CreateLookAt(m_Position, m_Position + m_Forward, m_Up);
		}
		CalcMouseMovement();
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
	
	m_Rotation += Vector3f(mouseDelta.y * mouseSpeed, -mouseDelta.x * mouseSpeed, 0.f);
	m_Rotation.z = 0;
	auto rotMatrix = Matrix4x4f::CreateRollPitchYaw({ m_Rotation.x, m_Rotation.y, m_Rotation.z });
	m_Forward = rotMatrix.GetForward();
	m_Forward.Normalize();

	m_Right = rotMatrix.GetRight();
	m_Right.Normalize();

	m_Up = rotMatrix.GetUp();
	m_Up.Normalize();
	myLookat = Matrix4x4f::CreateLookAt(m_Position, m_Position + m_Forward, m_Up);

}
