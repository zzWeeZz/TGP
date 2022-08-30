#include "Transform.h"

ToolBox::Transform::Transform()
{
	myPosition = { 0.0f, 0.0f, 0.0f };
	myRotation = { 0.0f, 0.0f, 0.0f };
	myScale = { 1.0f, 1.0f, 1.0f };
}

void ToolBox::Transform::SetPosition(const Vector3f& position)
{
	myPosition = position;
	CalculateTransfrom();
}

void ToolBox::Transform::SetPosition(float x, float y, float z)
{
	myPosition = Vector3f(x, y, z);
	CalculateTransfrom();
}

void ToolBox::Transform::SetRotation(const Vector3f& rotation)
{
	myRotation = rotation;
	CalculateTransfrom();
}

void ToolBox::Transform::SetRotation(float x, float y, float z)
{
	myRotation = Vector3f(x, y, z);
	CalculateTransfrom();
}

void ToolBox::Transform::SetScale(const Vector3f& scale)
{
	myScale = scale;
	CalculateTransfrom();
}

void ToolBox::Transform::SetScale(float x, float y, float z)
{
	myScale = Vector3f(x, y, z);
	CalculateTransfrom();
}

void ToolBox::Transform::CalculateTransfrom()
{
	Matrix4x4f transform;
	transform *= Matrix4x4f::CreateScaleMatrix(myScale);
	transform *= Matrix4x4f::CreateRotationAroundX(myRotation.x);
	transform *= Matrix4x4f::CreateRotationAroundZ(myRotation.z);
	transform *= Matrix4x4f::CreateRotationAroundY(myRotation.y);
	transform *= Matrix4x4f::CreateTranslation(myPosition);
	myMatrix = transform;
}
