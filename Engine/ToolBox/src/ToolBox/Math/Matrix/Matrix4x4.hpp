#pragma once
#include "ToolBox/Math/Vector/Vector.h"
#include "ToolBox/Utility/UtilityFunctions.hpp"
#include <array>

namespace ToolBox
{
	namespace Math
	{
		template<class T>
		class Matrix4x4
		{
		public:
			Matrix4x4<T>();
			Matrix4x4<T>(const Matrix4x4<T>& aMatrix);

			inline Vector3<float> GetForward();
			inline Vector3<float> GetUp();
			inline Vector3<float> GetRight();

			T& operator()(const int aRow, const int aColumn);
			const T& operator()(const int aRow, const int aColumn) const;
			Matrix4x4<T> operator=(const Matrix4x4& aMatrix4x4);

			void Deconstruct(Vector3<T>& position, Vector3<T>& rotation, Vector3<T>& scale);
			Matrix4x4<T> Lerp(Matrix4x4<T>& aMatrix, const float aLerpFactor);

			static Matrix4x4<T> CreateRotationAroundX(T aAngleInRadians);
			static Matrix4x4<T> CreateRotationAroundY(T aAngleInRadians);
			static Matrix4x4<T> CreateRotationAroundZ(T aAngleInRadians);
			static Matrix4x4<T> CreateRotationInLocalSpace(Matrix4x4<T> aTransform, const float aAngle, Vector3<T> aAxis);
			static Matrix4x4<T> CreateRotationMatrixFromQuaternionVectorWXYZ(Vector4<T> aVector);
			static Matrix4x4<T> CreateRollPitchYaw(Vector3<T> rotation);
			static Matrix4x4<T> CreateLookAt(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up);
			static Matrix4x4<T> CreateTranslation(const Vector4<T>& aPosition);
			static Matrix4x4<T> CreateTranslation(const Vector3<T>& aPosition);

			static Matrix4x4<T> CreateScaleMatrix(const Vector3<T>& aScale);

			static Matrix4x4<T> CreateLeftHandLookAtMatrix(const Vector3<T>& aPosition, const Vector3<T>& aDirection, const Vector3<T>& aUp);
			static Matrix4x4<T> CreateLeftHandPerspectiveMatrix(float aFOV, Vector2i aSize, float aNearPlane, float aFarPlane);
			static Matrix4x4<T> Transpose(const Matrix4x4<T>& aMatrixToTranspose);
			static Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& aTransform);
		private:
			std::array<std::array<T, 4>, 4> myMatrix;
		};


		template<class T>
		inline Matrix4x4<T> Matrix4x4<T>::CreateLookAt(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up)
		{
			Vector3<T>  forward = (center - eye).GetNormalized();
			Vector3<T>  right = up.Cross(forward).GetNormalized();
			Vector3<T>  newup = forward.Cross(right);

			Matrix4x4<T> result;

			result(1, 1) = right.x;
			result(1, 2) = newup.x;
			result(1, 3) = forward.x;
			result(1, 4) = 0;
			result(2, 1) = right.y;
			result(2, 2) = newup.y;
			result(2, 3) = forward.y;
			result(2, 4) = 0;
			result(3, 1) = right.z;
			result(3, 2) = newup.z;
			result(3, 3) = forward.z;
			result(3, 4) = 0;
			result(4, 1) = right.Dot({ -eye.x,-eye.y,-eye.z });
			result(4, 2) = newup.Dot({ -eye.x,-eye.y,-eye.z });
			result(4, 3) = forward.Dot({ -eye.x,-eye.y,-eye.z });
			result(4, 4) = 1;

			return result;
		}

		template <class T>
		Matrix4x4<T>::Matrix4x4()
		{
			myMatrix[0][0] = 1;
			myMatrix[0][1] = 0;
			myMatrix[0][2] = 0;
			myMatrix[0][3] = 0;

			myMatrix[1][0] = 0;
			myMatrix[1][1] = 1;
			myMatrix[1][2] = 0;
			myMatrix[1][3] = 0;

			myMatrix[2][0] = 0;
			myMatrix[2][1] = 0;
			myMatrix[2][2] = 1;
			myMatrix[2][3] = 0;

			myMatrix[3][0] = 0;
			myMatrix[3][1] = 0;
			myMatrix[3][2] = 0;
			myMatrix[3][3] = 1;
		}

		template <class T>
		Matrix4x4<T>::Matrix4x4(const Matrix4x4<T>& aMatrix)
		{
			myMatrix[0][0] = aMatrix(1, 1);
			myMatrix[0][1] = aMatrix(1, 2);
			myMatrix[0][2] = aMatrix(1, 3);
			myMatrix[0][3] = aMatrix(1, 4);

			myMatrix[1][0] = aMatrix(2, 1);
			myMatrix[1][1] = aMatrix(2, 2);
			myMatrix[1][2] = aMatrix(2, 3);
			myMatrix[1][3] = aMatrix(2, 4);

			myMatrix[2][0] = aMatrix(3, 1);
			myMatrix[2][1] = aMatrix(3, 2);
			myMatrix[2][2] = aMatrix(3, 3);
			myMatrix[2][3] = aMatrix(3, 4);

			myMatrix[3][0] = aMatrix(4, 1);
			myMatrix[3][1] = aMatrix(4, 2);
			myMatrix[3][2] = aMatrix(4, 3);
			myMatrix[3][3] = aMatrix(4, 4);
		}

		template<class T>
		inline Vector3<float> Matrix4x4<T>::GetForward()
		{
			Matrix4x4<float> rotation = *this;
			return { rotation(3, 1), rotation(3, 2), rotation(3, 3) };
		}

		template<class T>
		inline Vector3<float> Matrix4x4<T>::GetUp()
		{
			Matrix4x4<float> rotation = *this;

			return { rotation(2, 1), rotation(2, 2), rotation(2, 3) };
		}

		template<class T>
		inline Vector3<float> Matrix4x4<T>::GetRight()
		{
			Matrix4x4<float> rotation = *this;

			return { rotation(1, 1), rotation(1, 2), rotation(1, 3) };
		}

		template <class T>
		T& Matrix4x4<T>::operator()(const int aRow, const int aColumn)
		{
			assert(aRow <= 4 && aColumn <= 4 && aRow > 0 && aColumn > 0);
			return myMatrix[aRow - 1][aColumn - 1];
		}

		template <class T>
		const T& Matrix4x4<T>::operator()(const int aRow, const int aColumn) const
		{
			assert(aRow <= 4 && aColumn <= 4 && aRow > 0 && aColumn > 0);
			return myMatrix[aRow - 1][aColumn - 1];
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::operator=(const Matrix4x4& aMatrix4x4)
		{
			myMatrix[0][0] = aMatrix4x4(1, 1);
			myMatrix[0][1] = aMatrix4x4(1, 2);
			myMatrix[0][2] = aMatrix4x4(1, 3);
			myMatrix[0][3] = aMatrix4x4(1, 4);

			myMatrix[1][0] = aMatrix4x4(2, 1);
			myMatrix[1][1] = aMatrix4x4(2, 2);
			myMatrix[1][2] = aMatrix4x4(2, 3);
			myMatrix[1][3] = aMatrix4x4(2, 4);

			myMatrix[2][0] = aMatrix4x4(3, 1);
			myMatrix[2][1] = aMatrix4x4(3, 2);
			myMatrix[2][2] = aMatrix4x4(3, 3);
			myMatrix[2][3] = aMatrix4x4(3, 4);

			myMatrix[3][0] = aMatrix4x4(4, 1);
			myMatrix[3][1] = aMatrix4x4(4, 2);
			myMatrix[3][2] = aMatrix4x4(4, 3);
			myMatrix[3][3] = aMatrix4x4(4, 4);

			return *this;
		}

		template <class T>
		void Matrix4x4<T>::Deconstruct(Vector3<T>& position, Vector3<T>& rotation, Vector3<T>& scale)
		{
			static const float ZPI = 3.1415f;
			static const float RAD2DEG = (180.f / ZPI);
			const Matrix4x4<T>& mat = *this;
			Vector4f scaleX = { mat(1,1),mat(1,2) ,mat(1,3) ,mat(1,4) };
			Vector4f scaleY = { mat(2,1),mat(2,2) ,mat(2,3) ,mat(2,4) };
			Vector4f scaleZ = { mat(3,1),mat(3,2) ,mat(3,3) ,mat(3,4) };

			scale.x = scaleX.Length();
			scale.y = scaleY.Length();
			scale.z = scaleZ.Length();

			scaleX.Normalize();
			scaleY.Normalize();
			scaleZ.Normalize();

			rotation.x = RAD2DEG * atan2f(scaleY.z, scaleZ.z);
			rotation.y = RAD2DEG * atan2f(-scaleX.z, sqrtf(scaleY.z * scaleY.z + scaleZ.z * scaleZ.z));
			rotation.z = RAD2DEG * atan2f(scaleX.y, scaleX.x);

			position.x = mat(4, 1);
			position.y = mat(4, 2);
			position.z = mat(4, 3);
		}
		template<typename T>
		Matrix4x4<T> Matrix4x4<T>::Lerp(Matrix4x4<T>& aMatrix, float aTheta)
		{
			Matrix4x4<T> tempCopy = *this;
			Vector3<T> tempPos = { tempCopy(1,4),tempCopy(2,4) ,tempCopy(3,4) };
			Vector3<T> inPos = { aMatrix(1,4),aMatrix(2,4) ,aMatrix(3,4) };
			Vector3<T> lerpPos = Vector3<T>::Lerp(tempPos, inPos, aTheta);
			tempCopy(1, 4) = lerpPos.x;
			tempCopy(2, 4) = lerpPos.y;
			tempCopy(3, 4) = lerpPos.z;

			Vector3f myRightVector = Vector3f::Nlerp(tempCopy.GetRight(), aMatrix.GetRight(), aTheta);
			Vector3f myUpVector = Vector3f::Nlerp(tempCopy.GetUp(), aMatrix.GetUp(), aTheta);
			Vector3f myFrontVector = Vector3f::Nlerp(tempCopy.GetForward(), aMatrix.GetForward(), aTheta);
			tempCopy(1, 1) = myRightVector.x;
			tempCopy(1, 2) = myRightVector.y;
			tempCopy(1, 3) = myRightVector.z;

			tempCopy(2, 1) = myUpVector.x;
			tempCopy(2, 2) = myUpVector.y;
			tempCopy(2, 3) = myUpVector.z;

			tempCopy(3, 1) = myFrontVector.x;
			tempCopy(3, 2) = myFrontVector.y;
			tempCopy(3, 3) = myFrontVector.z;



			return tempCopy;
		}

		template <class T>
		bool operator==(const Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			for (int i = 1; i < 5; i++)
			{
				for (int j = 1; j < 5; j++)
				{
					if (aMatrix4x4Zero(i, j) != aMatrix4x4One(i, j))
					{
						return false;
					}
				}
			}

			return true;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(T aAngleInRadians)
		{
			Matrix4x4<T> tempMatrix4x4;

			tempMatrix4x4(1, 1) = 1;
			tempMatrix4x4(1, 2) = 0;
			tempMatrix4x4(1, 3) = 0;
			tempMatrix4x4(1, 4) = 0;

			tempMatrix4x4(2, 1) = 0;
			tempMatrix4x4(2, 2) = cos(aAngleInRadians);
			tempMatrix4x4(2, 3) = sin(aAngleInRadians);
			tempMatrix4x4(2, 4) = 0;

			tempMatrix4x4(3, 1) = 0;
			tempMatrix4x4(3, 2) = -sin(aAngleInRadians);
			tempMatrix4x4(3, 3) = cos(aAngleInRadians);
			tempMatrix4x4(3, 4) = 0;

			tempMatrix4x4(4, 1) = 0;
			tempMatrix4x4(4, 2) = 0;
			tempMatrix4x4(4, 3) = 0;
			tempMatrix4x4(4, 4) = 1;

			return tempMatrix4x4;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(T aAngleInRadians)
		{
			Matrix4x4<T> tempMatrix4x4;

			tempMatrix4x4(1, 1) = cos(aAngleInRadians);;
			tempMatrix4x4(1, 2) = 0;
			tempMatrix4x4(1, 3) = -sin(aAngleInRadians);
			tempMatrix4x4(1, 4) = 0;
			tempMatrix4x4(2, 1) = 0;
			tempMatrix4x4(2, 2) = 1;
			tempMatrix4x4(2, 3) = 0;
			tempMatrix4x4(2, 4) = 0;
			tempMatrix4x4(3, 1) = sin(aAngleInRadians);
			tempMatrix4x4(3, 2) = 0;
			tempMatrix4x4(3, 3) = cos(aAngleInRadians);
			tempMatrix4x4(3, 4) = 0;
			tempMatrix4x4(4, 1) = 0;
			tempMatrix4x4(4, 2) = 0;
			tempMatrix4x4(4, 3) = 0;
			tempMatrix4x4(4, 4) = 1;

			return tempMatrix4x4;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(T aAngleInRadians)
		{
			Matrix4x4<T> tempMatrix4x4;

			tempMatrix4x4(1, 1) = cos(aAngleInRadians);
			tempMatrix4x4(1, 2) = sin(aAngleInRadians);
			tempMatrix4x4(1, 3) = 0;
			tempMatrix4x4(1, 4) = 0;

			tempMatrix4x4(2, 1) = -sin(aAngleInRadians);
			tempMatrix4x4(2, 2) = cos(aAngleInRadians);
			tempMatrix4x4(2, 3) = 0;
			tempMatrix4x4(2, 4) = 0;

			tempMatrix4x4(3, 1) = 0;
			tempMatrix4x4(3, 2) = 0;
			tempMatrix4x4(3, 3) = 1;
			tempMatrix4x4(3, 4) = 0;

			tempMatrix4x4(4, 1) = 0;
			tempMatrix4x4(4, 2) = 0;
			tempMatrix4x4(4, 3) = 0;
			tempMatrix4x4(4, 4) = 1;

			return tempMatrix4x4;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateRotationInLocalSpace(Matrix4x4<T> aTransform, const float aAngle, Vector3<T> aAxis)
		{
			Vector4<float> axis = { aAxis.x, aAxis.y ,aAxis.z , 1 };

			axis = axis * aTransform;
			Matrix4x4<T> tempMatrix;

			tempMatrix(1, 1) = cos(aAngle) + (axis.x * axis.x) * (1 - cos(aAngle));
			tempMatrix(1, 2) = axis.x * axis.y * (1 - cos(aAngle)) - axis.z * sin(aAngle);
			tempMatrix(1, 3) = axis.x * axis.z * (1 - cos(aAngle)) + axis.y * sin(aAngle);

			tempMatrix(2, 1) = axis.y * axis.x * (1 - cos(aAngle)) + axis.z * sin(aAngle);
			tempMatrix(2, 2) = cos(aAngle) + (axis.y * axis.y) * (1 - cos(aAngle));
			tempMatrix(2, 3) = axis.y * axis.z * (1 - cos(aAngle)) - axis.x * sin(aAngle);

			tempMatrix(3, 1) = axis.z * axis.x * (1 - cos(aAngle)) - axis.y * sin(aAngle);
			tempMatrix(3, 2) = axis.z * axis.y * (1 - cos(aAngle)) + axis.x * sin(aAngle);
			tempMatrix(3, 3) = cos(aAngle) + (axis.z * axis.z) * (1 - cos(aAngle));

			return tempMatrix;
		}

		template<class T>
		inline Matrix4x4<T> Matrix4x4<T>::CreateRotationMatrixFromQuaternionVectorWXYZ(Vector4<T> aVector)
		{
			Matrix4x4<T> result;

			T qxx(aVector.y * aVector.y);
			T qyy(aVector.z * aVector.z);
			T qzz(aVector.w * aVector.w);

			T qxz(aVector.y * aVector.w);
			T qxy(aVector.y * aVector.z);
			T qyz(aVector.z * aVector.w);

			T qwx(aVector.x * aVector.y);
			T qwy(aVector.x * aVector.z);
			T qwz(aVector.x * aVector.w);

			result(1, 1) = T(1) - T(2) * (qyy + qzz);
			result(1, 2) = T(2) * (qxy + qwz);
			result(1, 3) = T(2) * (qxz - qwy);

			result(2, 1) = T(2) * (qxy - qwz);
			result(2, 2) = T(1) - T(2) * (qxx + qzz);
			result(2, 3) = T(2) * (qyz + qwx);

			result(3, 1) = T(2) * (qxz + qwy);
			result(3, 2) = T(2) * (qyz - qwx);
			result(3, 3) = T(1) - T(2) * (qxx + qyy);

			result(4, 1) = result(4, 2) = result(4, 3) = 0;
			result(4, 4) = 1;
			return result;
		}


		template<class T>
		inline Matrix4x4<T> Matrix4x4<T>::CreateRollPitchYaw(Vector3<T> rotation)
		{
			auto MatRoll = CreateRotationAroundZ(rotation.z);
			auto MatPitch = CreateRotationAroundX(rotation.x);
			auto MatYaw = CreateRotationAroundY(rotation.y);
			auto finalMat = MatRoll * MatYaw;
			finalMat = finalMat * MatPitch;
			return finalMat;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateTranslation(const Vector4<T>& aPosition)
		{
			Matrix4x4<T> returnMatrix;
			returnMatrix(4, 1) = aPosition.x;
			returnMatrix(4, 2) = aPosition.y;
			returnMatrix(4, 3) = aPosition.z;
			returnMatrix(4, 4) = aPosition.w;
			return returnMatrix;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateTranslation(const Vector3<T>& aPosition)
		{
			Matrix4x4<T> returnMatrix;
			returnMatrix(4, 1) = aPosition.x;
			returnMatrix(4, 2) = aPosition.y;
			returnMatrix(4, 3) = aPosition.z;
			return returnMatrix;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateScaleMatrix(const Vector3<T>& aScale)
		{
			Matrix4x4<T> matrix;
			matrix(1, 1) = aScale.x;
			matrix(2, 2) = aScale.y;
			matrix(3, 3) = aScale.z;
			return matrix;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateLeftHandLookAtMatrix(const Vector3<T>& aPosition, const Vector3<T>& aDirection, const Vector3<T>& aUp)
		{
			Vector3<T> xaxis = aUp.Cross(aDirection).GetNormalized();
			Vector3<T> yaxis = aDirection.Cross(xaxis);
			return Matrix4x4<T>();
			/*	return {
				xaxis.y, yaxis.x, aDirection.x, T(0),
				xaxis.y, yaxis.y, aDirection.y, T(0),
				xaxis.z, yaxis.z, aDirection.z, T(0),
				-xaxis.Dot(aPosition), -yaxis.Dot(aPosition), -aDirection.Dot(aPosition), T(1)
			};*/
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::CreateLeftHandPerspectiveMatrix(float aFOV, Vector2i aSize, float aNearPlane, float aFarPlane)
		{
			Matrix4x4<T> ProjectionMatrix;


			const float hFoVRad = aFOV * (PI / 180.f);
			const float vFoVRad = 2 * std::atan(std::tan(hFoVRad / 2.f) * ((float)aSize.y / (float)aSize.x));


			const float xScale = 1.0f / std::tanf(hFoVRad / 2.0f);
			const float yScale = 1.0f / std::tanf(vFoVRad * 0.5f);
			const float Q = aFarPlane / (aFarPlane - aNearPlane);
			ProjectionMatrix(1, 1) = xScale;
			ProjectionMatrix(2, 2) = yScale;

			ProjectionMatrix(3, 3) = Q;
			ProjectionMatrix(4, 3) = -Q * aNearPlane;
			ProjectionMatrix(3, 4) = 1.f / Q;
			ProjectionMatrix(4, 4) = 0;

			return ProjectionMatrix;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::Transpose(const Matrix4x4<T>& aMatrixToTranspose)
		{
			Matrix4x4<T> tempMatrix4x4;

			tempMatrix4x4(1, 1) = aMatrixToTranspose(1, 1);
			tempMatrix4x4(1, 2) = aMatrixToTranspose(2, 1);
			tempMatrix4x4(1, 3) = aMatrixToTranspose(3, 1);
			tempMatrix4x4(1, 4) = aMatrixToTranspose(4, 1);

			tempMatrix4x4(2, 1) = aMatrixToTranspose(1, 2);
			tempMatrix4x4(2, 2) = aMatrixToTranspose(2, 2);
			tempMatrix4x4(2, 3) = aMatrixToTranspose(3, 2);
			tempMatrix4x4(2, 4) = aMatrixToTranspose(4, 2);

			tempMatrix4x4(3, 1) = aMatrixToTranspose(1, 3);
			tempMatrix4x4(3, 2) = aMatrixToTranspose(2, 3);
			tempMatrix4x4(3, 3) = aMatrixToTranspose(3, 3);
			tempMatrix4x4(3, 4) = aMatrixToTranspose(4, 3);

			tempMatrix4x4(4, 1) = aMatrixToTranspose(1, 4);
			tempMatrix4x4(4, 2) = aMatrixToTranspose(2, 4);
			tempMatrix4x4(4, 3) = aMatrixToTranspose(3, 4);
			tempMatrix4x4(4, 4) = aMatrixToTranspose(4, 4);

			return tempMatrix4x4;
		}

		template <class T>
		Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& aTransform)
		{
			Matrix4x4<T> tempRot;
			tempRot(1, 1) = aTransform(1, 1);
			tempRot(1, 2) = aTransform(2, 1);
			tempRot(1, 3) = aTransform(3, 1);

			tempRot(2, 1) = aTransform(1, 2);
			tempRot(2, 2) = aTransform(2, 2);
			tempRot(2, 3) = aTransform(3, 2);

			tempRot(3, 1) = aTransform(1, 3);
			tempRot(3, 2) = aTransform(2, 3);
			tempRot(3, 3) = aTransform(3, 3);

			Matrix4x4<T> tempPos;
			tempPos(4, 1) = aTransform(4, 1) * -1;
			tempPos(4, 2) = aTransform(4, 2) * -1;
			tempPos(4, 3) = aTransform(4, 3) * -1;


			Matrix4x4<T> result;

			auto mat = tempRot * tempPos;

			result(1, 1) = tempRot(1, 1);
			result(1, 2) = tempRot(1, 2);
			result(1, 3) = tempRot(1, 3);
			result(1, 4) = 0;

			result(2, 1) = tempRot(2, 1);
			result(2, 2) = tempRot(2, 2);
			result(2, 3) = tempRot(2, 3);
			result(2, 4) = 0;

			result(3, 1) = tempRot(3, 1);
			result(3, 2) = tempRot(3, 2);
			result(3, 3) = tempRot(3, 3);
			result(3, 4) = 0;

			result(4, 1) = mat(4, 1);
			result(4, 2) = mat(4, 2);
			result(4, 3) = mat(4, 3);
			result(4, 4) = 1;

			return result;
		}

		template <class T>
		Matrix4x4<T> operator+(Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			Matrix4x4<T> tempMatrix;
			tempMatrix(1, 1) = aMatrix4x4Zero(1, 1) + aMatrix4x4One(1, 1);
			tempMatrix(1, 2) = aMatrix4x4Zero(1, 2) + aMatrix4x4One(1, 2);
			tempMatrix(1, 3) = aMatrix4x4Zero(1, 3) + aMatrix4x4One(1, 3);
			tempMatrix(1, 4) = aMatrix4x4Zero(1, 4) + aMatrix4x4One(1, 4);

			tempMatrix(2, 1) = aMatrix4x4Zero(2, 1) + aMatrix4x4One(2, 1);
			tempMatrix(2, 2) = aMatrix4x4Zero(2, 2) + aMatrix4x4One(2, 2);
			tempMatrix(2, 3) = aMatrix4x4Zero(2, 3) + aMatrix4x4One(2, 3);
			tempMatrix(2, 4) = aMatrix4x4Zero(2, 4) + aMatrix4x4One(2, 4);

			tempMatrix(3, 1) = aMatrix4x4Zero(3, 1) + aMatrix4x4One(3, 1);
			tempMatrix(3, 2) = aMatrix4x4Zero(3, 2) + aMatrix4x4One(3, 2);
			tempMatrix(3, 3) = aMatrix4x4Zero(3, 3) + aMatrix4x4One(3, 3);
			tempMatrix(3, 4) = aMatrix4x4Zero(3, 4) + aMatrix4x4One(3, 4);

			tempMatrix(4, 1) = aMatrix4x4Zero(4, 1) + aMatrix4x4One(4, 1);
			tempMatrix(4, 2) = aMatrix4x4Zero(4, 2) + aMatrix4x4One(4, 2);
			tempMatrix(4, 3) = aMatrix4x4Zero(4, 3) + aMatrix4x4One(4, 3);
			tempMatrix(4, 4) = aMatrix4x4Zero(4, 4) + aMatrix4x4One(4, 4);

			return tempMatrix;
		}

		template <class T>
		void operator+=(Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			aMatrix4x4Zero(1, 1) += aMatrix4x4One(1, 1);
			aMatrix4x4Zero(1, 2) += aMatrix4x4One(1, 2);
			aMatrix4x4Zero(1, 3) += aMatrix4x4One(1, 3);
			aMatrix4x4Zero(1, 4) += aMatrix4x4One(1, 4);

			aMatrix4x4Zero(2, 1) += aMatrix4x4One(2, 1);
			aMatrix4x4Zero(2, 2) += aMatrix4x4One(2, 2);
			aMatrix4x4Zero(2, 3) += aMatrix4x4One(2, 3);
			aMatrix4x4Zero(2, 4) += aMatrix4x4One(2, 4);

			aMatrix4x4Zero(3, 1) += aMatrix4x4One(3, 1);
			aMatrix4x4Zero(3, 2) += aMatrix4x4One(3, 2);
			aMatrix4x4Zero(3, 3) += aMatrix4x4One(3, 3);
			aMatrix4x4Zero(3, 4) += aMatrix4x4One(3, 4);

			aMatrix4x4Zero(4, 1) += aMatrix4x4One(4, 1);
			aMatrix4x4Zero(4, 2) += aMatrix4x4One(4, 2);
			aMatrix4x4Zero(4, 3) += aMatrix4x4One(4, 3);
			aMatrix4x4Zero(4, 4) += aMatrix4x4One(4, 4);
		}

		template <class T>
		Matrix4x4<T> operator-(Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			Matrix4x4<T> tempMatrix;
			tempMatrix(1, 1) = aMatrix4x4Zero(1, 1) - aMatrix4x4One(1, 1);
			tempMatrix(1, 2) = aMatrix4x4Zero(1, 2) - aMatrix4x4One(1, 2);
			tempMatrix(1, 3) = aMatrix4x4Zero(1, 3) - aMatrix4x4One(1, 3);
			tempMatrix(1, 4) = aMatrix4x4Zero(1, 4) - aMatrix4x4One(1, 4);

			tempMatrix(2, 1) = aMatrix4x4Zero(2, 1) - aMatrix4x4One(2, 1);
			tempMatrix(2, 2) = aMatrix4x4Zero(2, 2) - aMatrix4x4One(2, 2);
			tempMatrix(2, 3) = aMatrix4x4Zero(2, 3) - aMatrix4x4One(2, 3);
			tempMatrix(2, 4) = aMatrix4x4Zero(2, 4) - aMatrix4x4One(2, 4);

			tempMatrix(3, 1) = aMatrix4x4Zero(3, 1) - aMatrix4x4One(3, 1);
			tempMatrix(3, 2) = aMatrix4x4Zero(3, 2) - aMatrix4x4One(3, 2);
			tempMatrix(3, 3) = aMatrix4x4Zero(3, 3) - aMatrix4x4One(3, 3);
			tempMatrix(3, 4) = aMatrix4x4Zero(3, 4) - aMatrix4x4One(3, 4);

			tempMatrix(4, 1) = aMatrix4x4Zero(4, 1) - aMatrix4x4One(4, 1);
			tempMatrix(4, 2) = aMatrix4x4Zero(4, 2) - aMatrix4x4One(4, 2);
			tempMatrix(4, 3) = aMatrix4x4Zero(4, 3) - aMatrix4x4One(4, 3);
			tempMatrix(4, 4) = aMatrix4x4Zero(4, 4) - aMatrix4x4One(4, 4);

			return tempMatrix;
		}

		template <class T>
		void operator-=(Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			aMatrix4x4Zero(1, 1) -= aMatrix4x4One(1, 1);
			aMatrix4x4Zero(1, 2) -= aMatrix4x4One(1, 2);
			aMatrix4x4Zero(1, 3) -= aMatrix4x4One(1, 3);
			aMatrix4x4Zero(1, 4) -= aMatrix4x4One(1, 4);

			aMatrix4x4Zero(2, 1) -= aMatrix4x4One(2, 1);
			aMatrix4x4Zero(2, 2) -= aMatrix4x4One(2, 2);
			aMatrix4x4Zero(2, 3) -= aMatrix4x4One(2, 3);
			aMatrix4x4Zero(2, 4) -= aMatrix4x4One(2, 4);

			aMatrix4x4Zero(3, 1) -= aMatrix4x4One(3, 1);
			aMatrix4x4Zero(3, 2) -= aMatrix4x4One(3, 2);
			aMatrix4x4Zero(3, 3) -= aMatrix4x4One(3, 3);
			aMatrix4x4Zero(3, 4) -= aMatrix4x4One(3, 4);

			aMatrix4x4Zero(4, 1) -= aMatrix4x4One(4, 1);
			aMatrix4x4Zero(4, 2) -= aMatrix4x4One(4, 2);
			aMatrix4x4Zero(4, 3) -= aMatrix4x4One(4, 3);
			aMatrix4x4Zero(4, 4) -= aMatrix4x4One(4, 4);
		}



		template<class T>
		Matrix4x4<T> operator*(const Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			Matrix4x4<T> tempMatrix;

			tempMatrix(1, 1) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 1)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 1)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 1)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 1));
			tempMatrix(1, 2) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 2)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 2)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 2)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 2));
			tempMatrix(1, 3) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 3)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 3)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 3)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 3));
			tempMatrix(1, 4) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 4)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 4)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 4)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 4));

			tempMatrix(2, 1) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 1) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 1) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 1) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 1);
			tempMatrix(2, 2) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 2) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 2) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 2) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 2);
			tempMatrix(2, 3) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 3) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 3) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 3) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 3);
			tempMatrix(2, 4) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 4) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 4) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 4) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 4);

			tempMatrix(3, 1) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 1) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 1) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 1) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 1);
			tempMatrix(3, 2) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 2) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 2) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 2) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 2);
			tempMatrix(3, 3) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 3) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 3) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 3) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 3);
			tempMatrix(3, 4) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 4) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 4) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 4) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 4);

			tempMatrix(4, 1) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 1) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 1) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 1) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 1);
			tempMatrix(4, 2) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 2) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 2) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 2) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 2);
			tempMatrix(4, 3) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 3) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 3) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 3) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 3);
			tempMatrix(4, 4) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 4) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 4) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 4) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 4);

			return tempMatrix;
		}

		template<class T>
		void operator*=(Matrix4x4<T>& aMatrix4x4Zero, const Matrix4x4<T>& aMatrix4x4One)
		{
			Matrix4x4<T> tempMatrix;

			tempMatrix(1, 1) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 1)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 1)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 1)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 1));
			tempMatrix(1, 2) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 2)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 2)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 2)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 2));
			tempMatrix(1, 3) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 3)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 3)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 3)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 3));
			tempMatrix(1, 4) = (aMatrix4x4Zero(1, 1) * aMatrix4x4One(1, 4)) + (aMatrix4x4Zero(1, 2) * aMatrix4x4One(2, 4)) + (aMatrix4x4Zero(1, 3) * aMatrix4x4One(3, 4)) + (aMatrix4x4Zero(1, 4) * aMatrix4x4One(4, 4));

			tempMatrix(2, 1) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 1) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 1) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 1) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 1);
			tempMatrix(2, 2) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 2) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 2) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 2) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 2);
			tempMatrix(2, 3) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 3) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 3) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 3) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 3);
			tempMatrix(2, 4) = aMatrix4x4Zero(2, 1) * aMatrix4x4One(1, 4) + aMatrix4x4Zero(2, 2) * aMatrix4x4One(2, 4) + aMatrix4x4Zero(2, 3) * aMatrix4x4One(3, 4) + aMatrix4x4Zero(2, 4) * aMatrix4x4One(4, 4);

			tempMatrix(3, 1) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 1) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 1) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 1) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 1);
			tempMatrix(3, 2) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 2) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 2) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 2) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 2);
			tempMatrix(3, 3) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 3) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 3) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 3) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 3);
			tempMatrix(3, 4) = aMatrix4x4Zero(3, 1) * aMatrix4x4One(1, 4) + aMatrix4x4Zero(3, 2) * aMatrix4x4One(2, 4) + aMatrix4x4Zero(3, 3) * aMatrix4x4One(3, 4) + aMatrix4x4Zero(3, 4) * aMatrix4x4One(4, 4);

			tempMatrix(4, 1) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 1) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 1) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 1) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 1);
			tempMatrix(4, 2) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 2) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 2) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 2) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 2);
			tempMatrix(4, 3) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 3) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 3) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 3) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 3);
			tempMatrix(4, 4) = aMatrix4x4Zero(4, 1) * aMatrix4x4One(1, 4) + aMatrix4x4Zero(4, 2) * aMatrix4x4One(2, 4) + aMatrix4x4Zero(4, 3) * aMatrix4x4One(3, 4) + aMatrix4x4Zero(4, 4) * aMatrix4x4One(4, 4);
			aMatrix4x4Zero = tempMatrix;
		}

		template <class T>
		Vector4<T> operator*(const Vector4<T>& aVector4, const Matrix4x4<T>& aMatrix4x4)
		{
			Vector4<T> tempVector;

			tempVector.x = aVector4.x * aMatrix4x4(1, 1) + aVector4.y * aMatrix4x4(2, 1) + aVector4.z * aMatrix4x4(3, 1) + aVector4.w * aMatrix4x4(4, 1);
			tempVector.y = aVector4.x * aMatrix4x4(1, 2) + aVector4.y * aMatrix4x4(2, 2) + aVector4.z * aMatrix4x4(3, 2) + aVector4.w * aMatrix4x4(4, 2);
			tempVector.z = aVector4.x * aMatrix4x4(1, 3) + aVector4.y * aMatrix4x4(2, 3) + aVector4.z * aMatrix4x4(3, 3) + aVector4.w * aMatrix4x4(4, 3);
			tempVector.w = aVector4.x * aMatrix4x4(1, 4) + aVector4.y * aMatrix4x4(2, 4) + aVector4.z * aMatrix4x4(3, 4) + aVector4.w * aMatrix4x4(4, 4);

			return tempVector;
		}
	}
}
