#include "../GameStates/MainMenu.hpp"
#include "../header/TextureManager.hpp"
#include "../header/MeshGenerator.hpp"
#include "../header/PlayerController.hpp"
#include "../header/UIManager.hpp"

#include "../header/GameObject.hpp"
#include "../header/GameSaver.hpp"
#include <vector>

extern int gGameRunning;

namespace {
	Font cardinalsFont;
	Font digipenFont;
	Texture startTexture{}, exitTexture{}, creditsTexture{};
	Texture menuBGTexture{};
	
	//Digipen logo transition
	Texture tex_digipenLogo{};
	GameObject go_digipenLogo, menuBG;
	f32 logo_transparency = 0.f;
	s8 isLogoDrawn = 0;
	s32 mouseX, mouseY;
	AEGfxVertexList* squareMesh;

	// UI stuff
	UIManager menuUI;
	std::unique_ptr<Button> startBtn, exitBtn, creditsBtn;

	GameObjectIDAssigner assigner;
}


void MainMenuState::LoadState() {
	PRINT("Load: %s", "Main Menu\n");
	cardinalsFont.Load_Font(Fonts::Alt, 160.f);
	cardinalsFont.Input_Text("cardinals");

	digipenFont.Load_Font(Fonts::Main, 30.f);
	digipenFont.Input_Text("All content (C) 2025 Digipen Institute of Technology Singapore. All Rights Reserved");

	menuBGTexture.Load_Image(Images::Background);
	tex_digipenLogo.Load_Image(Images::DigipenLogo);
	startTexture.Load_Image(Images::StartBtn);
	exitTexture.Load_Image(Images::ExitBtn);
	creditsTexture.Load_Image(Images::CreditsBtn);
}

void MainMenuState::InitializeState()
{
	squareMesh = RectMeshCenter();
	go_digipenLogo.SetMesh(squareMesh);
	go_digipenLogo.SetID(assigner.GetID());

	menuBG.SetMesh(squareMesh);
	menuBG.SetTexture(menuBGTexture);
	menuBG.UpdatePosition(AEVec2{ 0.f, 0.f });
	menuBG.UpdateScale(AEVec2{ menuBGTexture.Get_Image_Width(), menuBGTexture.Get_Image_Height() });
	menuBG.UpdateRotation(0.f);
	menuBG.SetID(assigner.GetID());

	// creates button w relevant stuff stolen from gameobej
	startBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, 0.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	startBtn->SetTexture(startTexture); startBtn->SetActive(true); startBtn->SetInteractable(true);
	startBtn->SetID(assigner.GetID());

	exitBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, -300.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	exitBtn->SetTexture(exitTexture); exitBtn->SetActive(true); exitBtn->SetInteractable(true);
	exitBtn->SetID(assigner.GetID());

	creditsBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, -150.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	creditsBtn->SetTexture(creditsTexture); creditsBtn->SetActive(true); creditsBtn->SetInteractable(true);
	creditsBtn->SetID(assigner.GetID());

	// use them like this ik its weird asf but like. i'll make it better soon
	startBtn->SetOnClick([]() {
		PRINT("Start Pressed");
		});
	
	creditsBtn->SetOnClick([]() {
		PRINT("Credits Pressed");
		});

	exitBtn->SetOnClick([]() {
		PRINT("Exit Pressed");
		gGameRunning = 0;
		PRINT("Game Running = %d", gGameRunning);
		});

	// so we can control all buttons with a menuUI hehehehe
	menuUI.Add(startBtn.get());
	menuUI.Add(exitBtn.get());
	menuUI.Add(creditsBtn.get());

	//Save to JSON
	std::vector<GameObject> objs;
	objs.push_back(go_digipenLogo);
	objs.push_back(menuBG);
	objs.push_back(*startBtn);
	objs.push_back(*creditsBtn);
	objs.push_back(*exitBtn);
	SaveObjectsToJson(objs);

	//Read From JSON
	objs = ReadJsonToObjects("../../Saves/GameObjects.json");

	for (GameObject& go : objs) {
		std::cout << go << "\n";
	}

	PRINT("Initialize: %s", "Main Menu\n");
}

void MainMenuState::UpdateState() {
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

		menuUI.Update(0.f);

		startTexture.Set_Image(0, 0);
		exitTexture.Set_Image(0, 0);
		creditsTexture.Set_Image(0, 0);
		menuBGTexture.Set_Image(0, 0);

		AEInputGetCursorPosition(&mouseX, &mouseY);
	}
}

void MainMenuState::StateDraw() {
	//PRINT("Draw: %s", "Main Menu\n");

	if (!isLogoDrawn) {
		go_digipenLogo.Draw();
		digipenFont.Draw_Text(AEVec2{ 0, -330.f }, 1.f, logo_transparency);
		return;
	}

	menuBG.Draw();
	cardinalsFont.Draw_Text(0, 200.f);
	startBtn->Draw();
	exitBtn->Draw();
	creditsBtn->Draw();
}

void MainMenuState::FreeState() {
	
	cardinalsFont.Free_Font();
	digipenFont.Free_Font();

	tex_digipenLogo.Free_Image();

	menuBGTexture.Free_Image();
	startTexture.Free_Image();
	exitTexture.Free_Image();
	creditsTexture.Free_Image();

	AEGfxMeshFree(squareMesh);
	menuUI.Exit();
	PRINT("Free: %s", "Main Menu\n");
}

void MainMenuState::UnloadState() {
	PRINT("Unload: %s", "Main Menu\n");
}
