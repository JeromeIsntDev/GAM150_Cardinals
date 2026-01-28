/*!***********************************************************************
\file	.hpp
\author	Jerome Lim
\date	12/01/2026
\par	Last Updated by: Jerome Lim
\par	Project: Cardinals

\brief	HEADER File for GameObject class

\par	Changes: None
**************************************************************************/
#include "../GameStates/MainMenu.hpp"

extern int gGameRunning;

namespace {

	Font cardinalsFont;
	Font digipenFont;
	Texture startTexture{}, exitTexture{}, creditsTexture{}, yesTexture{}, noTexture{}, popUpTexture{};
	Texture menuBGTexture{};
	
	//Digipen logo transition
	Texture tex_digipenLogo{};
	GameObject go_digipenLogo, menuBG;
	f32 logo_transparency = 0.f;
	s8 isLogoDrawn = 0;
	s32 mouseX, mouseY;
	AEGfxVertexList* squareMesh;

	// UI stuff
	UIManager menuUI, popUpUI;
	std::unique_ptr<Button> startBtn, exitBtn, creditsBtn, yesBtn, noBtn;
	std::unique_ptr<PopUp> quitPopUp;

	GameObjectIDAssigner assigner;
	Audio& bgm = Audio::Instance();
	//Audio backgroundMusic;
	//AEAudioGroup bgmGroup;
}


void MainMenu_Load() {
	PRINT("Load: %s", "Main Menu\n");
	assigner.currIdCount = 0;

	cardinalsFont.Load_Font(Fonts::Alt, 160.f);
	cardinalsFont.Input_Text("cardinals");

	digipenFont.Load_Font(Fonts::Main, 30.f);
	digipenFont.Input_Text("All content (C) 2025 Digipen Institute of Technology Singapore. All Rights Reserved");

	menuBGTexture.Load_Image(Images::Background);
	tex_digipenLogo.Load_Image(Images::DigipenLogo);
	startTexture.Load_Image(Images::StartBtn);
	exitTexture.Load_Image(Images::ExitBtn);
	creditsTexture.Load_Image(Images::CreditsBtn);

	// testing
	yesTexture.Load_Image(Images::StartBtn); noTexture.Load_Image(Images::ExitBtn);
	popUpTexture.Load_Image(Images::DigipenLogo); 

	//bgmGroup = AEAudioCreateGroup();
	//backgroundMusic.Load_Audio(Audios::BGM);
}

void MainMenu_Initialize()
{
	bgm.Init_Audio_System();
	bgm.Load_Audio("BGM", Audios::BGM);
	bgm.Play_Audio("BGM", AudioType::MUSIC);
	
	//backgroundMusic.SetAudioGroup(bgmGroup);
	//backgroundMusic.Play_Audio();

	squareMesh = RectMeshCenter();
	go_digipenLogo.SetMesh(squareMesh);
	go_digipenLogo.SetID(assigner.GetID());

	menuBG.SetMesh(squareMesh);
	menuBG.SetTexture(&menuBGTexture);
	menuBG.UpdatePosition(AEVec2{ 0.f, 0.f });
	menuBG.UpdateScale(AEVec2{ menuBGTexture.Get_Image_Width(), menuBGTexture.Get_Image_Height() });
	menuBG.UpdateRotation(0.f);
	menuBG.SetID(assigner.GetID());

	// creates button w relevant stuff stolen from gameobej
	startBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, 0.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	startBtn->SetTexture(&startTexture); startBtn->SetActive(true); startBtn->SetInteractable(true);
	startBtn->SetID(assigner.GetID());

	exitBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, -300.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	exitBtn->SetTexture(&exitTexture); exitBtn->SetActive(true); exitBtn->SetInteractable(true);
	exitBtn->SetID(assigner.GetID());

	creditsBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, -150.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	creditsBtn->SetTexture(&creditsTexture); creditsBtn->SetActive(true); creditsBtn->SetInteractable(true);
	creditsBtn->SetID(assigner.GetID());

	//testing

	yesBtn = std::make_unique<Button>(squareMesh, AEVec2{ -100.f, -50.f }, 0.f, AEVec2{ yesTexture.Get_Image_Width(), yesTexture.Get_Image_Height() });
	yesBtn->SetTexture(&yesTexture); yesBtn->SetActive(true); yesBtn->SetInteractable(true);
	yesBtn->SetID(assigner.GetID());

	noBtn = std::make_unique<Button>(squareMesh, AEVec2{ -100.f, -250.f }, 0.f, AEVec2{ noTexture.Get_Image_Width(), noTexture.Get_Image_Height() });
	noBtn->SetTexture(&noTexture); noBtn->SetActive(true); noBtn->SetInteractable(true);
	noBtn->SetID(assigner.GetID());

	quitPopUp = std::make_unique<PopUp>(squareMesh, AEVec2{ 0,0 }, 0.f, AEVec2{ 600.f, 400.f });
	quitPopUp->SetTexture(&popUpTexture);
	quitPopUp->SetID(assigner.GetID());

	quitPopUp->SetActive(false);

	quitPopUp->AddChild(yesBtn.get());
	quitPopUp->AddChild(noBtn.get());

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

	yesBtn->SetOnClick([]() {
		PRINT("fuck you");
		});

	noBtn->SetOnClick([]() {
		PRINT("ni ma");
		});

	// so we can control all buttons with a menuUI hehehehe
	menuUI.Add(startBtn.get());
	menuUI.Add(exitBtn.get());
	menuUI.Add(creditsBtn.get());

	popUpUI.Add(quitPopUp.get());
	popUpUI.Add(yesBtn.get());
	popUpUI.Add(noBtn.get());

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

	go_digipenLogo.SetTexture(&tex_digipenLogo);
	go_digipenLogo.UpdatePosition(AEVec2{ 0.f, 0.f });
	go_digipenLogo.UpdateScale(AEVec2{ tex_digipenLogo.Get_Image_Width(), tex_digipenLogo.Get_Image_Height() });
	go_digipenLogo.UpdateRotation(0.f);

	PRINT("Initialize: %s", "Main Menu\n");
}

void MainMenu_Update() {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	Color bg = Colors::PastelPurple;
	AEGfxSetBackgroundColor(bg.r, bg.g, bg.b);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	
	AEGfxSetTransparency(logo_transparency);

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

		//menuUI.Update(0.f); // for handling button clicks

		// toggle
		quitPopUp->ToggleActive(AEVK_6);

		if (quitPopUp->IsActive()) {
			popUpUI.Update(0.f); 
		}
		else {
			// freezes menuUI but i'll update this in the future to freeze the whole game
			menuUI.Update(0.f);
		}

		AEInputGetCursorPosition(&mouseX, &mouseY);
	}
}

void MainMenu_Draw() {
	//PRINT("Draw: %s", "Main Menu\n");

	if (!isLogoDrawn) {
		go_digipenLogo.Draw();
		digipenFont.Draw_Text(AEVec2{ 0, -330.f }, 1.f, logo_transparency);
		return;
	}
	//else {
	//	startTexture.Set_Image(0, 0);
	//	exitTexture.Set_Image(0, 0);
	//	creditsTexture.Set_Image(0, 0);
	//	menuBGTexture.Set_Image(0, 0);
	//}

	menuBG.Draw();

	menuUI.Draw();
	popUpUI.Draw();

	cardinalsFont.Draw_Text(0, 200.f);
}

void MainMenu_Free() {
	PRINT("Free: %s", "Main Menu\n");

	cardinalsFont.Free_Font();
	digipenFont.Free_Font();

	bgm.Stop_Audio();
	bgm.Free_Audio_System();
	//backgroundMusic.Free_Audio();
	//backgroundMusic.Free_Audio_Group();

	AEGfxMeshFree(squareMesh);
	popUpUI.Exit();
	menuUI.Exit();
}

void MainMenu_Unload() {
	PRINT("Unload: %s", "Main Menu\n");

	tex_digipenLogo.Unload_Image();
	menuBGTexture.Unload_Image();
	startTexture.Unload_Image();
	exitTexture.Unload_Image();
	creditsTexture.Unload_Image();
}
