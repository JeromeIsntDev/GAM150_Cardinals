#include "../GameStates/LevelMenu.hpp"
#include "../header/TextureManager.hpp"
#include "../header/MeshGenerator.hpp"
#include "../header/PlayerController.hpp"
#include "../header/UIManager.hpp"

namespace {
	Font levelFont;
}


void LevelMenuState::LoadState() {
	PRINT("Load: %s", "Level Menu\n");
	levelFont.Load_Font(Fonts::Main, 160.f);
	levelFont.Input_Text("Level Menu");

}

void LevelMenuState::InitializeState()
{
	PRINT("Initialize: %s", "Level Menu\n");
}

void LevelMenuState::UpdateState() {
	//PRINT("Update: %s", "Main Menu\n");

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	Color bg = Colors::PastelPurple;
	AEGfxSetBackgroundColor(bg.r, bg.g, bg.b);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	AEGfxSetTransparency(1.f);

}

void LevelMenuState::StateDraw() {
	//PRINT("Draw: %s", "Level Menu\n");

	levelFont.Draw_Text(0, 200.f);

}

void LevelMenuState::FreeState() {
	levelFont.Free_Font();
	PRINT("Free: %s", "Level Menu\n");
}

void LevelMenuState::UnloadState() {
	PRINT("Unload: %s", "Level Menu\n");
}
