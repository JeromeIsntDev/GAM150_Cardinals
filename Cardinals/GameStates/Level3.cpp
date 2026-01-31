#include "../GameStates/Level3.hpp"

extern std::vector<Texture> textures;

namespace {
	Text digipenFont;
}

void Level3_Load() {
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 3");
	digipenFont.Change_Text_Color(Colors::Black);

	PRINT("Load: %s", "Level 3\n");
}

void Level3_Initialize()
{
	PRINT("Initialize: %s", "Level 3\n");
}

void Level3_Update() {
	//PRINT("Update: %s", "Main Menu\n");
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.75f, 0.9f, 0.65f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);
}

void Level3_Draw() {
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);

	PRINT("Draw: %s", "Level 3\n");
}

void Level3_Free() {
	digipenFont.Free_Font();

	PRINT("Free: %s", "Level 3\n");
}

void Level3_Unload() {
	PRINT("Unload: %s", "Level 3\n");
}
