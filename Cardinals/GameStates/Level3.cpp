#include "../GameStates/Level3.hpp"
#include "../header/TextureManager.hpp"

namespace {
	Font digipenFont;
}

void Level3State::LoadState() {
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 3");
	digipenFont.Change_Text_Color(Colors::Black);

	PRINT("Load: %s", "Level 3\n");
}

void Level3State::InitializeState()
{
	PRINT("Initialize: %s", "Level 3\n");
}

void Level3State::UpdateState() {
	//PRINT("Update: %s", "Main Menu\n");
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);
}

void Level3State::StateDraw() {
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f);

	PRINT("Draw: %s", "Level 3\n");
}

void Level3State::FreeState() {
	PRINT("Free: %s", "Level 3\n");
}

void Level3State::UnloadState() {
	PRINT("Unload: %s", "Level 3\n");
}
