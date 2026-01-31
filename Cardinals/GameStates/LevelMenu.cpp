#include "../GameStates/LevelMenu.hpp"

namespace {
	Text levelFont;
}


void LevelMenu_Load() {
	PRINT("Load: %s", "Level Menu\n");
	levelFont.Load_Font(Fonts::Main, 160.f);
	levelFont.Input_Text("Level Menu");
}

void LevelMenu_Initialize()
{
	PRINT("Initialize: %s", "Level Menu\n");
}

void LevelMenu_Update() {
	//PRINT("Update: %s", "Main Menu\n");

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	Color bg = Colors::PastelPurple;
	AEGfxSetBackgroundColor(bg.r, bg.g, bg.b);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	AEGfxSetTransparency(1.f);
}

void LevelMenu_Draw() {
	//PRINT("Draw: %s", "Level Menu\n");

	levelFont.Draw_Text(0, 200.f);
}

void LevelMenu_Free() {
	levelFont.Free_Font();
	PRINT("Free: %s", "Level Menu\n");
}

void LevelMenu_Unload() {
	PRINT("Unload: %s", "Level Menu\n");
}
