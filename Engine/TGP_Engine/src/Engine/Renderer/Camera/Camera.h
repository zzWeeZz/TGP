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

		ToolBox::Transform& GetTransform() { return myTransform; }
		Matrix4x4f GetProjection() { return myProjection; }
		static Ref<Camera> Create(float fov, Vector2i size, float zNear, float zFar);
	private:
		void CalcMouseMovement();
		ToolBox::Transform myTransform;
		Matrix4x4f myProjection;
		bool myGiveControl;
		float myFov;
		float myAspect;
		float myZNear;
		float myZFar;
	};
}