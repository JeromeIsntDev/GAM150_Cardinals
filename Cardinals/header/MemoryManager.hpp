#pragma once
#ifndef _MEM_MANAGER_
#define _MEM_MANAGER_

#include "TextureManager.hpp"

struct MemoryManager {
	std::vector<Texture*> textures;
	std::vector<AEGfxVertexList**> meshes;
	std::vector<Text*> fonts;

	void ReserveTextures(size_t);
	void ReserveMeshes(size_t);
	void ReserveFonts(size_t);

	void AddTexture(Texture*);
	void AddMesh(AEGfxVertexList**);
	void AddFont(Text*);
	bool MassUnload();
};

extern MemoryManager memoryManager;

#endif