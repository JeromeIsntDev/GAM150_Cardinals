#include "MemoryManager.hpp"

void MemoryManager::ReserveTextures(size_t s)
{
	textures.reserve(s);
}

void MemoryManager::ReserveMeshes(size_t s)
{
	meshes.reserve(s);
}

void MemoryManager::ReserveFonts(size_t s)
{
	fonts.reserve(s);
}

void MemoryManager::AddTexture(Texture* t)
{
	textures.push_back(t);
}

void MemoryManager::AddTextures(std::initializer_list<Texture*> textureList)
{
	for (Texture* t : textureList) {
		AddTexture(t);
	}
}

void MemoryManager::AddMesh(AEGfxVertexList** m)
{
	meshes.push_back(m);
}

void MemoryManager::AddMeshes(std::initializer_list<AEGfxVertexList**> meshList)
{
	for (AEGfxVertexList** m : meshList) {
		AddMesh(m);
	}
}

void MemoryManager::AddFont(Text* t)
{
	fonts.push_back(t);
}

void MemoryManager::AddFonts(std::initializer_list<Text*> fontList)
{
	for (Text* f : fontList) {
		AddFont(f);
	}
}

bool MemoryManager::MassUnload()
{
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

	if (!fonts.empty()) {
		for (Text* t : fonts) {
			t->Free_Font();
		}
	}

	textures.clear();
	meshes.clear();
	fonts.clear();

	return true;
}
