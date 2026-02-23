/*!***********************************************************************
\file	MeshGenerator.cpp
\author	Jerome Lim
\date	12/01/2026
\par	Last Updated by: Jerome Lim
\par	Project: Cardinals

\brief	MeshGenerator is responsible for creating meshes; square-centered,
		square-corner and circle.

\par	Changes: None
**************************************************************************/
#include "../header/MeshGenerator.hpp"

AEGfxVertexList* RectMeshCenter() {
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF000000, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFF000000, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

AEGfxVertexList* RectMeshCorner() {
	AEGfxMeshStart();

	AEGfxTriAdd(
		0.f, 0.f, 0xFF000000, 0.0f, 1.0f,
		1.f, 0.f, 0xFF000000, 1.0f, 1.0f,
		0.f, 1.f, 0xFF000000, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.f, 0.f, 0xFF000000, 1.0f, 1.0f,
		1.f, 1.f, 0xFF000000, 1.0f, 0.0f,
		0.f, 1.f, 0xFF000000, 0.0f, 0.0f);

	return AEGfxMeshEnd();
}

AEGfxVertexList* CircleMesh(u32 segments) {
	f32 r = .5f;

	AEGfxMeshStart();

	for (int i = 0; i < segments; ++i)
	{
		f32 a1 = (f32)i / segments * 2.0f * PI;
		f32 a2 = (f32)(i + 1) / segments * 2.0f * PI;

		f32 x1 = r * AECos(a1);
		f32 y1 = r * AESin(a1);

		f32 x2 = r * AECos(a2);
		f32 y2 = r * AESin(a2);

		AEGfxTriAdd(
			0.0f, 0.0f, 0xFF000000, 0.f, 1.f,
			x1, y1, 0xFF000000, 0.f, 1.f,
			x2, y2, 0xFF000000, 0.f, 1.f
		);
	}

	return AEGfxMeshEnd();
}

AEGfxVertexList* LineMesh() {
	AEGfxMeshStart();
	AEGfxVertexAdd(1.f, 0.f, 0xFF000000, 1.0f, 1.0f);
	AEGfxVertexAdd(0.f, 0.f, 0xFF000000, 1.0f, 1.0f);
	return AEGfxMeshEnd();
}
