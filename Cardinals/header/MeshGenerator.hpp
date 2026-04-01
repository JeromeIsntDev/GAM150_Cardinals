/**
 * @file		MeshGenerator.hpp
 * @brief		Exists as a global way to get meshes
 * @details		Self-explanatory
 * @author		Jerome Lim 
 * @date		14.01.2026
 */
#pragma once
#ifndef _Mesh_Gen_Header_
#define _Mesh_Gen_Header_

#include <AEEngine.h>

 /**
  * @brief			Returns a AEGfxVertexList* that is a rectangular mesh that is centered
  */
AEGfxVertexList* RectMeshCenter();
/**
 * @brief			Returns a AEGfxVertexList* that is a rectangular mesh that has been sliced based on; mainly used for spritesheets
 *					the given params
 * @param rows		The number of outcome rows when sliced
 * @param columns	The number of outcome columns when sliced
 */
AEGfxVertexList* RectMeshCenterSliced(u32 rows, u32 columns);
/**
 * @brief			Returns a AEGfxVertexList* that is a rectangular mesh that is drawn from bottom-left corner
 */
AEGfxVertexList* RectMeshCorner();
/**
 * @brief			Returns a AEGfxVertexList* that is a circular mesh that is centered
 */
AEGfxVertexList* CircleMesh(u32 segments);

#endif // !_Mesh_Gen_Header_
