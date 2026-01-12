/*!***********************************************************************
\file	MeshGenerator.hpp
\author	Jerome Lim
\date	12/01/2026
\par	Last Updated by: Jerome Lim
\par	Project: Cardinals

\brief	MeshGenerator is responsible for creating meshes; square-centered,
		square-corner and circle.

\par	Changes: None
**************************************************************************/
#pragma once
#ifndef _Mesh_Gen_Header_

#include <AEEngine.h>

AEGfxVertexList* RectMeshCentered();
AEGfxVertexList* RectMeshCorner();
AEGfxVertexList* CircleMesh(u32 segments);

#endif // !_Mesh_Gen_Header_
