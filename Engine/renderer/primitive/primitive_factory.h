#pragma once

#include <math/math_types.h>

namespace ue
{

enum class CubeFace : int
{
	Top = 0,
	Front = Top + 4,
	Right = Front + 4,
	Back = Right + 4,
	Left = Back + 4,
	Bottom = Left + 4,
};

enum Components
{
	Normals = 1 << 0,
	Colors = 1 << 1,
	UVs = 1 << 2,
};


class PrimitiveFactory
{
public:

	struct Vector4
	{
		union {
			float _v[4];
			float x, y, z, w;
			float r, g, b, a;
		};
	};

	struct Vector3
	{
		union {
			float _v[3];
			float x, y, z;
		};
	};

	struct Vector2
	{
		union {
			float _v[2];
			float x, y;
			float u, v;
		};
	};

	struct Face
	{
		union
		{
			unsigned short _v[4];
			unsigned short v0, v1, v2, v3;
		};
	};

	struct Triangle
	{
		union
		{
			unsigned short _v[3];
			unsigned short v0, v1, v2;
		};
	};



	PrimitiveFactory();

	void cube(float *vertices, unsigned &n_verts, unsigned short *indices, unsigned &n_indices, unsigned components);

private:

	template <typename U, typename T>
	inline U store(U &dest, const T& src) {
		memcpy(dest, &src, sizeof(T));
		dest = reinterpret_cast<U>(reinterpret_cast<char*>(dest) + sizeof(T));
		return dest;
	}


	Vector3 _cube_vertices[8];
	Vector3 _cube_normals[6];
	Vector2 _face_uvs[4];
	Face _cube_faces[6];
	Triangle _cube_triangles[12];
	Vector4 _cube_colors[6];

};

}