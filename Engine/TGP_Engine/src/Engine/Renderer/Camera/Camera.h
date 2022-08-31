#pragma once
#include "Engine/Utils/Utilities.h"
#include "ToolBox/Math/Transform.h"

namespace Engine
{
	class Camera
	{
	public:
		Camera(float fov, Vector2i size, float zNear, float zFar);
		
		void Update(float deltaTime);

		void SetAspectRatio(float x, float y);

		Matrix4x4f& GetLookAt() { return myLookat; }
		Vector3f GetPos() { return m_Position; }
		Matrix4x4f GetProjection() { return myProjection; }
		static Ref<Camera> Create(float fov, Vector2i size, float zNear, float zFar);
	private:
		void CalcMouseMovement();
		Matrix4x4f myLookat;
		Vector3f m_Position;
		Vector3f m_Forward;
		Vector3f m_Up;
		Vector3f m_Right;
		Vector3f m_Rotation;
		Matrix4x4f myProjection;
		bool myGiveControl;
		float myFov;
		float myAspect;
		float myZNear;
		float myZFar;
	};
}