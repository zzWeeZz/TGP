#pragma once
#include <d3d11.h>
#include "ToolBox/Math/Vector/Vector.h"

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float r, float g, float b, float a)
		: pos(x, y, z, 1), color(r, g, b, a) {}
	Vector4f pos;
	Vector4f color = { 1,1,1,1 };
	Vector4f color1 = { 1,1,1,1 };
	Vector4f color2 = { 1,1,1,1 };
	Vector4f color3 = { 1,1,1,1 };

	Vector2f UV[4];

	Vector3f normal = {};
	Vector3f tangent = {};
	Vector3f biTangent = {};

	ToolBox::Math::Vector4<uint32_t> BoneIds = { 0,0,0,0 };
	Vector4f BoneWeights = { 0,0,0,0 };
};

struct ParticleVertex
{
	Vector4f pos;
	Vector4f color = { 1,1,1,1 };
	Vector3f velocity = {};
	Vector3f scale = {};
	float lifeTime = 0;
};