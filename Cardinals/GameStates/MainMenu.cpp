#include "../GameStates/MainMenu.hpp"
#include "../header/TextureManager.hpp"
#include "../header/MeshGenerator.hpp"
#include "../header/PlayerController.hpp"
#include "../header/UIManager.hpp"

namespace {
	Font cardinalsFont;

	Texture startTexture{}, exitTexture{}, creditsTexture{};
	
	//Digipen logo transition
	Texture tex_digipenLogo{};
	GameObject go_digipenLogo;
	f32 logo_transparency = 0.f;
	s8 isLogoDrawn = 0;
	s32 mouseX, mouseY;
	AEGfxVertexList* squareMesh;

	// UI stuff
	UIManager menuUI;
	std::unique_ptr<Button> startBtn, exitBtn, creditsBtn;
}


void MainMenuState::LoadState() {
	PRINT("Load: %s", "Main Menu\n");
	cardinalsFont.Load_Font(Fonts::Alt, 160.f);
	cardinalsFont.Input_Text("cardinals");

	tex_digipenLogo.Load_Image(Images::DigipenLogo);
	startTexture.Load_Image(Images::StartBtn);
	exitTexture.Load_Image(Images::ExitBtn);
	creditsTexture.Load_Image(Images::CreditsBtn);
}

void MainMenuState::InitializeState()
{
	squareMesh = RectMeshCenter();
	go_digipenLogo.SetMesh(squareMesh);

	//AEGfxVertexList* startMesh = RectMeshCenter();
	//startBtn.SetMesh(startMesh);

	// creates button w relevant stuff stolen from gameobej
	startBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, 0.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	startBtn->SetTexture(startTexture); startBtn->SetActive(true); startBtn->SetInteractable(true);

	exitBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, -300.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	exitBtn->SetTexture(exitTexture); exitBtn->SetActive(true); exitBtn->SetInteractable(true);

	creditsBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, -150.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	creditsBtn->SetTexture(creditsTexture); creditsBtn->SetActive(true); creditsBtn->SetInteractable(true);

	// use them like this ik its weird asf but like. i'll make it better soon
	exitBtn->SetOnClick([]() {
		PRINT("fuck yall im out");
		// exit func or whatever
		});

	// so we can control all buttons with a menuUI hehehehe
	menuUI.Add(startBtn.get());
	menuUI.Add(exitBtn.get());
	menuUI.Add(creditsBtn.get());

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

	if (!isLogoDrawn) {
		go_digipenLogo.SetTexture(tex_digipenLogo);
		go_digipenLogo.UpdatePosition(AEVec2{ 0.f, 0.f });
		go_digipenLogo.UpdateScale(AEVec2{ tex_digipenLogo.Get_Image_Width(), tex_digipenLogo.Get_Image_Height() });
		go_digipenLogo.UpdateRotation(0.f);

		tex_digipenLogo.Set_Image(0, 0);
	}

	if (AEFrameRateControllerGetFrameCount() < 100) {
		logo_transparency += 0.02f;
	}
	else if (AEFrameRateControllerGetFrameCount() > 100 && (AEFrameRateControllerGetFrameCount() < 200)) {
		logo_transparency -= 0.02f;
	}
	else if (AEFrameRateControllerGetFrameCount() > 200) {
		logo_transparency = 1.f;
		isLogoDrawn = 1;
	}

	if (isLogoDrawn) {

		menuUI.Update(0.f); // dt = 0 for now lala

	startTexture.Set_Image(0, 0);

		//exitBtn.SetTexture(exitTexture);
		//exitBtn.UpdatePosition(AEVec2{ 0.f, -300.f });
		//exitBtn.UpdateScale(AEVec2{ exitTexture.Get_Image_Width(), exitTexture.Get_Image_Height() });
		//exitBtn.UpdateRotation(0.f);

		exitTexture.Set_Image(0, 0);

		//creditsBtn.SetTexture(creditsTexture);
		//creditsBtn.UpdatePosition(AEVec2{ 0.f, -150.f });
		//creditsBtn.UpdateScale(AEVec2{ creditsTexture.Get_Image_Width(), creditsTexture.Get_Image_Height() });
		//creditsBtn.UpdateRotation(0.f);

		creditsTexture.Set_Image(0, 0);

	AEInputGetCursorPosition(&mouseX, &mouseY);

		//if (startBtn.HoverMesh(mouseX, mouseY))
		//{
		//	startBtn.UpdateScale(AEVec2{ creditsTexture.Get_Image_Width() * 1.1f, creditsTexture.Get_Image_Height() * 1.1f });
		//	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		//		//start game
		//	}
		//}
		//else 
		//if (creditsBtn.HoverMesh(mouseX, mouseY))
		//{
		//	creditsBtn.UpdateScale(AEVec2{ creditsTexture.Get_Image_Width() * 1.1f, creditsTexture.Get_Image_Height() * 1.1f });
		//	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		//		//credits page
		//	}
		//}
		//else if (exitBtn.HoverMesh(mouseX, mouseY))
		//{
		//	exitBtn.UpdateScale(AEVec2{ creditsTexture.Get_Image_Width() * 1.1f, creditsTexture.Get_Image_Height() * 1.1f });
		//	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		//		//exit
		//	}
		//}
	}
}

void MainMenuState::StateDraw() {
	//PRINT("Draw: %s", "Main Menu\n");

	if (!isLogoDrawn) {
		go_digipenLogo.Draw();
		return;
	}

	cardinalsFont.Draw_Text(AEVec2{ 0, 200.f }, 1.f);

		startBtn->Draw();
		exitBtn->Draw();
		creditsBtn->Draw();
	}

void MainMenuState::FreeState() {
	cardinalsFont.Free_Font();
	tex_digipenLogo.Free_Image();
	startTexture.Free_Image();
	exitTexture.Free_Image();
	creditsTexture.Free_Image();

	AEGfxMeshFree(squareMesh);
	PRINT("Free: %s", "Main Menu\n");
}

void MainMenuState::UnloadState() {
	PRINT("Unload: %s", "Main Menu\n");
}
