#include "Camera.h"
#include "ToolBox/Input/Input.h"
#include "ToolBox/Math/Matrix/Matrix.h"

Engine::Camera::Camera(float fov, Vector2i size, float zNear, float zFar)
{
	myFov = fov;
	myAspect = (float)size.x / (float)size.y;
	myZNear = zNear;
	myZFar = zFar;
	m_Forward = Vector3f(0.f, 0.f, -1.f);
	m_Right = Vector3f(-1.f, 0.f, 0.f);
	m_Up = Vector3f(0.f, 1.f, 0.f);
	m_Rotation = Vector3f(0.f, 0.f, 0.f);
	myProjection = Matrix4x4f::CreateLeftHandPerspectiveMatrix(myFov, size, myZNear, myZFar);
	myLookat = Matrix4x4f::CreateLookAt(m_Position, m_Position + m_Forward, m_Up);
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
	myProjection = Matrix4x4f::CreateLeftHandPerspectiveMatrix(myFov, { (int)x, (int)y }, myZNear, myZFar);
}

Engine::Ref<Engine::Camera> Engine::Camera::Create(float fov, Vector2i size, float zNear, float zFar)
{
	return CreateRef<Camera>(fov, size, zNear, zFar);
}

void Engine::Camera::CalcMouseMovement()
{
	auto mouseDelta = ToolBox::Input::GetMouseDelta();
	auto mouseSpeed = 0.005f;
	float yaw, pitch, roll;
	m_Rotation += Vector3f(mouseDelta.y * mouseSpeed, mouseDelta.x * mouseSpeed, 0.f);
	yaw = m_Rotation.y;
	pitch = m_Rotation.x;
	roll = m_Rotation.z;

	auto rotMatrixX = Matrix4x4f::CreateRotationAroundX(pitch);
	auto rotMatrixY = Matrix4x4f::CreateRotationAroundY(yaw);
	auto rotMatrixZ = Matrix4x4f::CreateRotationAroundZ(roll);
	auto rotMatrix = rotMatrixX * rotMatrixY * rotMatrixZ;
	const Vector4f forward4 = (Vector4f(0, 0, 1, 0) * rotMatrix).GetNormalized();
	const Vector4f up4 = (Vector4f(0, 1, 0, 0) * rotMatrix).GetNormalized();
	const Vector4f right4 = (Vector4f(1, 0, 0, 0) * rotMatrix).GetNormalized();
	m_Forward = {forward4.x,forward4.y ,forward4.z };
	m_Right = {right4.x, right4.y, right4.z};
	m_Up = { up4.x, up4.y, up4.z };
	myLookat = Matrix4x4f::CreateLookAt(m_Position,m_Position + m_Forward, m_Up);

}
