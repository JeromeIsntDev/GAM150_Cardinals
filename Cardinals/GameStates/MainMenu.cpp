#include "../GameStates/MainMenu.hpp"
#include "../header/TextureManager.hpp"
#include "../header/MeshGenerator.hpp"
#include "../header/PlayerController.hpp"

namespace {
	Font digipenFont;
	
	GameObject startBtn;
	GameObject exitBtn;
	GameObject creditsBtn;
	
	//Digipen logo transition
	Texture digipenTexture{};
	GameObject digipenLogo;
	f32 logo_transparency = 0.f;
	s8 logo_drawn = 0;
}


void MainMenuState::LoadState() {
	PRINT("Load: %s", "Main Menu\n");
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Main Menu");
	digipenFont.Change_Text_Color(Colors::Black);

	digipenTexture.Load_Image(Images::DigipenLogo);
}

void MainMenuState::InitializeState()
{
	AEGfxVertexList* squareMesh = RectMeshCenter();
	digipenLogo.SetMesh(squareMesh);
	PRINT("Initialize: %s", "Main Menu\n");
}

void MainMenuState::UpdateState() {
	//PRINT("Update: %s", "Main Menu\n");

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(logo_transparency);

	if (!logo_drawn) {
		digipenLogo.SetTexture(digipenTexture);
		digipenLogo.UpdatePosition(AEVec2{ 0.f, 0.f });
		digipenLogo.UpdateScale(AEVec2{ digipenTexture.Get_Image_Width(), digipenTexture.Get_Image_Height() });
		digipenLogo.UpdateRotation(0.f);

		digipenTexture.Set_Image(0, 0);
	}

	if (AEFrameRateControllerGetFrameCount() < 100) {
		logo_transparency += 0.02f;
	}
	else if (AEFrameRateControllerGetFrameCount() > 100 && (AEFrameRateControllerGetFrameCount() < 200)) {
		logo_transparency -= 0.02f;
	}
	else if (AEFrameRateControllerGetFrameCount() > 200) {
		logo_transparency = 1.f;
		logo_drawn = 1;
	}

	if (logo_drawn) {

	}
}

void MainMenuState::StateDraw() {
	PRINT("Draw: %s", "Main Menu\n");

	digipenFont.Draw_Text(AEVec2{-700.f, -430.f}, .5f);

	if (!logo_drawn) {
		digipenLogo.Draw();
	}

}

void MainMenuState::FreeState() {
	digipenFont.Free_Font();
	digipenTexture.Free_Image();
	PRINT("Free: %s", "Main Menu\n");
}

void MainMenuState::UnloadState() {
	PRINT("Unload: %s", "Main Menu\n");
}
