/**
 * @file		MemoryManager.hpp
 * @brief		Contains structs and functions that assist in unloading memory at the end 
 *				of a game state.
 * @details		Has vectors of types that will assist to store objects that need to be freed/deallocated
 *				at the end of a game state
 * @author		Jerome Lim / Breanne Chua
 * @date		23.02.2026
 */

#pragma once
#ifndef _MEM_MANAGER_
#define _MEM_MANAGER_

#include  <initializer_list>
#include "TextureManager.hpp"

 /**
  * @class MemoryManager
  * @brief Simple Struct that has vectors of types that need to be freed/deallocated
  */
struct MemoryManager {
	std::vector<Texture*> textures;
	std::vector<AEGfxVertexList**> meshes;
	std::vector<Text*> fonts;

	void ReserveTextures(size_t);
	void ReserveMeshes(size_t);
	void ReserveFonts(size_t);


/**
 * @brief			Adds a pointer to a texture that needs to be freed to the textures vector
 */
	void AddTexture(Texture*);
	/**
 * @brief			Adds a list of textures that needs to be freed to the textures vector; used to add multiple textures
 */
	void AddTextures(std::initializer_list<Texture*>);
	/**
 * @brief			Adds a double-pointer to a AEGfxVertexList* that needs to be freed to the meshes vector
 */
	void AddMesh(AEGfxVertexList**);
	/**
 * @brief			Adds a list of double-pointers that point to AEGfxVertexList* that needs to be freed to the meshes vector
 */
	void AddMeshes(std::initializer_list<AEGfxVertexList**>);
	/**
 * @brief			Adds a pointer to Text that needs to be freed to the texts vector
 */
	void AddFont(Text*);
	/**
 * @brief			Adds a list of pointers to Text that needs to be freed to the texts vector
 */
	void AddFonts(std::initializer_list<Text*>);
	/**
 * @brief			Calls the unload/free functions of all the objects that are within the vectors
 */
	bool MassUnload();
};

//Used as a global decleration
extern MemoryManager memoryManager;

#endif