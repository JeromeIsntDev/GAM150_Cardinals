#pragma once
#ifndef _Game_Manager_HEADER_
#define _Game_Manager_HEADER_

#include "TextureManager.hpp"

int gGameRunning = 1;
std::vector<Texture*> textures;
std::vector<AEGfxVertexList**> meshes;
std::vector<Text*> texts;

bool MassUnload() {
	if (!textures.empty()) {
		for (Texture* t : textures) {
			t->Unload_Image();
		}
	}

	if (!meshes.empty()) {
		for (AEGfxVertexList** m : meshes) {
			AEGfxMeshFree(*m);
		}
	}

	if (!texts.empty()) {
		for (Text* t : texts) {
			t->Free_Font();
		}
	}

	return true;
}

#endif // !_Game_Manager_HEADER_
