#pragma once
#include "Matrix/Matrix.h"
#include "Vector/Vector.h"

namespace ToolBox
{
	class Transform
	{
	public:
		Transform();

		void SetPosition(const Vector3f& position);
		void SetPosition(float x, float y, float z);
		Vector3f GetPosition() const { return myPosition; }
		Vector3f& GetPosition() { return myPosition; }

		void SetRotation(const Vector3f& rotation, bool LocalRotation = false);
		void SetRotation(float x, float y, float z);
		Vector3f GetRotation() const { return myRotation; }
		Vector3f& GetRotation() { return myRotation; }

		void SetScale(const Vector3f& scale);
		void SetScale(float x, float y, float z);
		Vector3f GetScale() const { return myScale; }
		Vector3f& GetScale() { return myScale; }

		void SetMatrix(Matrix4x4f& mat);

		Vector3f GetForward() { return myMatrix.GetForward(); }
		Vector3f GetUp() { return myMatrix.GetUp(); }
		Vector3f GetRight() { return myMatrix.GetRight(); }
		

		Matrix4x4f& GetMatrix() { return myMatrix; }

	private:
		void CalculateTransfrom(bool localRot = false);
		
		ToolBox::Math::Matrix4x4<float> myMatrix;
		
		Vector3f myPosition;
		Vector3f myRotation;
		Vector3f myScale;
	};
}