#include "../GameStates/Level1.hpp"
#include "../header/MeshGenerator.hpp"
#include "../header/TextureManager.hpp"
#include "../header/AudioManager.hpp"
#include "../header/GStateManager.hpp"
#include "../header/PlayerController.hpp"
#include <AEEngine.h>

namespace {
	Font digipenFont;
	f32 playerHealth{ 150.f }, playerMovespeed{ 100.f }, playerJumpForce{ 0.5f };
	bool playerIsJump{ false };
	Player mainPlayer;
	Texture digipenTexture{};
}

void Level1State::LoadState() {
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 1");
	digipenFont.Change_Text_Color(Colors::Black);

	PRINT("Load: %s", "Level 1\n");
}

void Level1State::InitializeState()
{
	AEGfxVertexList* squareMesh;
	squareMesh = RectMeshCenter();
	mainPlayer.InitializePlayer(playerHealth, playerMovespeed, playerJumpForce, false, 0.f, 0.f,squareMesh);
	digipenTexture.Load_Image(Images::DigipenLogo);
	PRINT("Initialize: %s", "Level 1\n");
}

void Level1State::UpdateState() {
	f32 logo_transparency = 0.f;
	//declare mainPlayer variable

	mainPlayer.SetTexture(digipenTexture);
	mainPlayer.UpdateScale(AEVec2{ digipenTexture.Get_Image_Width(), digipenTexture.Get_Image_Height() });
	//PRINT("Update: %s", "Main Menu\n");
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);

	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	mainPlayer.PlayerMovement(dt, playerJumpForce);
	mainPlayer.Draw();
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
