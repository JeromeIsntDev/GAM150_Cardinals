#include "../GameStates/MainMenu.hpp"

void MainMenuState::LoadState() {
	PRINT("Load: %s", "Main Menu\n");
}

void MainMenuState::InitializeState()
{
	PRINT("Initialize: %s", "Main Menu\n");
}

void MainMenuState::UpdateState() {
	PRINT("Update: %s", "Main Menu\n");
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);
}

void MainMenuState::StateDraw() {
	PRINT("Draw: %s", "Main Menu\n");
}

void MainMenuState::FreeState() {
	PRINT("Free: %s", "Main Menu\n");
}

void MainMenuState::UnloadState() {
	PRINT("Unload: %s", "Main Menu\n");
}
