#include "../GameStates/Level2.hpp"

namespace {
	Font digipenFont;
}

void Level2_Load() {
	PRINT("Load: %s", "Level 2\n");

	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 2");
	digipenFont.Change_Text_Color(Colors::Black);
}

void Level2_Initialize()
{
	PRINT("Initialize: %s", "Level 2\n");
}

void Level2_Update() {
	//PRINT("Update: %s", "Main Menu\n");
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.9f, 0.85f, 0.65f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);
}

void Level2_Draw() {
	PRINT("Draw: %s", "Level 2\n");

	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);
}

void Level2_Free() {
	PRINT("Free: %s", "Level 2\n");

	digipenFont.Free_Font();
}

void Level2_Unload() {
	PRINT("Unload: %s", "Level 2\n");
}
