#include "../GameStates/MainMenu.hpp"
#include "../header/TextureManager.hpp"
#include "../header/MeshGenerator.hpp"
#include "../header/PlayerController.hpp"

namespace {
	Font cardinalsFont;

	Texture startTexture{};
	GameObject startBtn;
	Texture exitTexture{};
	GameObject exitBtn;
	Texture creditsTexture{};
	GameObject creditsBtn;
	
	//Digipen logo transition
	Texture digipenTexture{};
	GameObject digipenLogo;
	f32 logo_transparency = 0.f;
	s8 logo_drawn = 0;
}


void MainMenuState::LoadState() {
	PRINT("Load: %s", "Main Menu\n");
	cardinalsFont.Load_Font(Fonts::Alt, 72.f);
	cardinalsFont.Input_Text("cardinals");

	digipenTexture.Load_Image(Images::DigipenLogo);
	startTexture.Load_Image(Images::StartBtn);
	exitTexture.Load_Image(Images::ExitBtn);
	creditsTexture.Load_Image(Images::CreditsBtn);
}

void MainMenuState::InitializeState()
{
	AEGfxVertexList* squareMesh = RectMeshCenter();
	digipenLogo.SetMesh(squareMesh);

	AEGfxVertexList* startMesh = RectMeshCenter();
	startBtn.SetMesh(startMesh);
	AEGfxVertexList* exitMesh = RectMeshCenter();
	exitBtn.SetMesh(exitMesh); 
	AEGfxVertexList* creditsMesh = RectMeshCenter();
	creditsBtn.SetMesh(creditsMesh);
	PRINT("Initialize: %s", "Main Menu\n");
}

void MainMenuState::UpdateState() {
	//PRINT("Update: %s", "Main Menu\n");

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	Color bg = Colors::PastelPurple;
	AEGfxSetBackgroundColor(bg.r, bg.g, bg.b);
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
		startBtn.SetTexture(startTexture);
		startBtn.UpdatePosition(AEVec2{ 0.f, 0.f });
		startBtn.UpdateScale(AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
		startBtn.UpdateRotation(0.f);

		startTexture.Set_Image(0, 0);

		exitBtn.SetTexture(exitTexture);
		exitBtn.UpdatePosition(AEVec2{ 0.f, -300.f });
		exitBtn.UpdateScale(AEVec2{ exitTexture.Get_Image_Width(), exitTexture.Get_Image_Height() });
		exitBtn.UpdateRotation(0.f);

		exitTexture.Set_Image(0, 0);

		creditsBtn.SetTexture(creditsTexture);
		creditsBtn.UpdatePosition(AEVec2{ 0.f, -150.f });
		creditsBtn.UpdateScale(AEVec2{ creditsTexture.Get_Image_Width(), creditsTexture.Get_Image_Height() });
		creditsBtn.UpdateRotation(0.f);

		creditsTexture.Set_Image(0, 0);
	}
}

void MainMenuState::StateDraw() {
	PRINT("Draw: %s", "Main Menu\n");

	if (!logo_drawn) {
		digipenLogo.Draw();
	}
	else {
		cardinalsFont.Draw_Text(AEVec2{ 0, 200.f }, 2.5f);

		startBtn.Draw();
		exitBtn.Draw();
		creditsBtn.Draw();
	}

}

void MainMenuState::FreeState() {
	cardinalsFont.Free_Font();
	digipenTexture.Free_Image();
	startTexture.Free_Image();
	exitTexture.Free_Image();
	creditsTexture.Free_Image();
	PRINT("Free: %s", "Main Menu\n");
}

void MainMenuState::UnloadState() {
	PRINT("Unload: %s", "Main Menu\n");
}
