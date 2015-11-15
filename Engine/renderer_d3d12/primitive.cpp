#include "primitive.h"
#include <string.h>

PrimitiveFactory::PrimitiveFactory()
{
	float cube_size = 1.f * 0.5f;

	_cube_vertices[0] = { -cube_size, cube_size, -cube_size };
	_cube_vertices[1] = { cube_size, cube_size, -cube_size };
	_cube_vertices[2] = { -cube_size, cube_size,  cube_size };
	_cube_vertices[3] = { cube_size, cube_size,  cube_size };
	_cube_vertices[4] = { -cube_size, -cube_size, -cube_size };
	_cube_vertices[5] = { cube_size, -cube_size, -cube_size };
	_cube_vertices[6] = { -cube_size, -cube_size,  cube_size };
	_cube_vertices[7] = { cube_size, -cube_size,  cube_size };

	_cube_normals[0] = { 0, 1, 0 };
	_cube_normals[1] = { 0, 0, -1 };
	_cube_normals[2] = { 1, 0, 0 };
	_cube_normals[3] = { 0, 0, 1 };
	_cube_normals[4] = { -1, 0, 0 };
	_cube_normals[5] = { 0, -1, 0 };

	_cube_colors[0] = { 1, 0, 0, 1 };
	_cube_colors[1] = { 0, 1, 0, 1 };
	_cube_colors[2] = { 0, 0, 1, 1 };
	_cube_colors[3] = { 1, 1, 0, 1 };
	_cube_colors[4] = { 1, 0, 1, 1 };
	_cube_colors[5] = { 0, 1, 1, 1 };

	_face_uvs[0] = { 0, 1 };
	_face_uvs[1] = { 1, 1 };
	_face_uvs[2] = { 0, 0 };
	_face_uvs[3] = { 1, 0 };

	_cube_faces[0] = { 0, 1, 2, 3 };
	_cube_faces[1] = { 2, 3, 6, 7 };
	_cube_faces[2] = { 3, 1, 7, 5 };
	_cube_faces[3] = { 1, 0, 5, 4 };
	_cube_faces[4] = { 0, 2, 4, 6 };
	_cube_faces[5] = { 5, 4, 7, 6 };

	_cube_triangles[0] = { 0, 1, 2 };
	_cube_triangles[1] = { 2, 1, 3 };
	_cube_triangles[2] = { 2, 3, 6 };
	_cube_triangles[3] = { 6, 3, 7 };
	_cube_triangles[4] = { 3, 1, 7 };
	_cube_triangles[5] = { 7, 1, 5 };
	_cube_triangles[6] = { 1, 0, 5 };
	_cube_triangles[7] = { 5, 0, 4 };
	_cube_triangles[8] = { 0, 2, 4 };
	_cube_triangles[9] = { 4, 2, 6 };
	_cube_triangles[10] = { 5, 4, 7 };
	_cube_triangles[11] = { 7, 4, 6 };

	//    0         1

	//2          3
	// 4         5

	//   6          7


}

void PrimitiveFactory::cube(float *vertices, unsigned &n_verts, unsigned short *indices, unsigned &n_indices, unsigned components)
{
	n_verts = 0;
	n_indices = 0;
	float *verts = vertices;
	unsigned short *ind = indices;

	for (unsigned i = 0; i < 6; ++i)
	{
		const Face& face = _cube_faces[i];
		unsigned short index_buffer[4];
		for (unsigned v = 0; v < 4; ++v) {
			store(verts, _cube_vertices[face._v[v]]);
			if (components & Normals)
				store(verts, _cube_normals[i]);
			if (components & Colors)
				store(verts, _cube_colors[i]);
			if (components & UVs)
				store(verts, _face_uvs[v]);
			index_buffer[v] = n_verts++;
		}

		store(ind, index_buffer[0]);
		store(ind, index_buffer[1]);
		store(ind, index_buffer[2]);
		store(ind, index_buffer[2]);
		store(ind, index_buffer[1]);
		store(ind, index_buffer[3]);
		n_indices += 6;
	}
}