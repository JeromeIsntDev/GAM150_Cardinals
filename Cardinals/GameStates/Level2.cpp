#include "../GameStates/Level2.hpp"
#include "../header/TextureManager.hpp"

namespace {
	Font digipenFont;
}

void Level2State::LoadState() {
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 2");
	digipenFont.Change_Text_Color(Colors::Black);

	PRINT("Load: %s", "Level 2\n");
}

void Level2State::InitializeState()
{
	PRINT("Initialize: %s", "Level 2\n");
}

void Level2State::UpdateState() {
	//PRINT("Update: %s", "Main Menu\n");
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.9f, 0.85f, 0.65f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);
}

void Level2State::StateDraw() {
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f);

	PRINT("Draw: %s", "Level 2\n");
}

void Level2State::FreeState() {
	PRINT("Free: %s", "Level 2\n");
}

void Level2State::UnloadState() {
	PRINT("Unload: %s", "Level 2\n");
}
