#include "../GameStates/Level1.hpp"
#include "../header/ControlPanel.hpp"

namespace {
	Font digipenFont;
	f32 playerHealth{ 150.f }, playerMovespeed{ 300.f };
	bool playerIsJump{ false };
	//ControlPanel controlPanel;
	Player mainPlayer;
	Texture playerTexture{};
	AEGfxVertexList* squareMesh;
}

void Level1_Load() {
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 1");
	digipenFont.Change_Text_Color(Colors::Black);
	PRINT("Load: %s", "Level 1\n");
}

void Level1_Initialize()
{
	//controlPanel.ControlPanel_Initialize();
	squareMesh = RectMeshCenter();
	mainPlayer.InitializePlayer(playerHealth, playerMovespeed, false, 0.f, 0.f,squareMesh, 0.f, 150.f);
	playerTexture.Load_Image(Images::Player);
	mainPlayer.SetTexture(&playerTexture);
	mainPlayer.UpdateScale(AEVec2{ 100.f, 100.f });
	mainPlayer.UpdateRotation(0.f);

	PRINT("Initialize: %s", "Level 1\n");
}

void Level1_Update() {
	//declare mainPlayer variable
	
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.95f, 0.65f, 0.77f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);
	//controlPanel.ControlPanel_Update();
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	mainPlayer.PlayerMovement(dt);

	//PRINT("Update: %s", "Main Menu\n");
}

void Level1_Draw() {
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);
	mainPlayer.Draw();

	PRINT("Draw: %s", "Level 1\n");
}

void Level1_Free() {

	digipenFont.Free_Font();
	AEGfxMeshFree(squareMesh);
	PRINT("Free: %s", "Level 1\n");
}

void Level1_Unload() {
	PRINT("Unload: %s", "Level 1\n");
	playerTexture.Unload_Image();
}
