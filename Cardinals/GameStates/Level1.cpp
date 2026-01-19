#include "../GameStates/Level1.hpp"
#include "../header/TextureManager.hpp"

namespace {
	Font digipenFont;
}

void Level1State::LoadState() {
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 1");
	digipenFont.Change_Text_Color(Colors::Black);

	PRINT("Load: %s", "Level 1\n");
}

void Level1State::InitializeState()
{
	PRINT("Initialize: %s", "Level 1\n");
}

void Level1State::UpdateState() {
	//PRINT("Update: %s", "Main Menu\n");
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);
}

void Level1State::StateDraw() {
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f);

	PRINT("Draw: %s", "Level 1\n");
}

void Level1State::FreeState() {
	PRINT("Free: %s", "Level 1\n");
}

void Level1State::UnloadState() {
	PRINT("Unload: %s", "Level 1\n");
}
