/*!***********************************************************************
\file	MainMenu.hpp
\author	Jerome Lim
\date	12/01/2026
\par	Last Updated by: Jerome Lim
\par	Project: Cardinals

\brief	HEADER File for GameObject class

\par	Changes: None
**************************************************************************/
#include "MainMenu.hpp"

extern int isGameRunning;
extern std::vector<Texture*> textures;
extern std::vector<AEGfxVertexList**> meshes;
extern std::vector<Text*> texts;
extern bool MassUnload();

//auto& input = InputHandler::GetInstance();

namespace {

	AEGfxVertexList* squareMesh;
	Text cardinalsFont, digipenFont, tooltipFont;

	Texture startTexture{}, exitTexture{}, prevTexture{}, creditsTexture{}, yesTexture{}, noTexture{}, popUpTexture{}, settingsTexture{};
	Texture menuBGTexture{};

	//Digipen logo transition
	Texture tex_digipenLogo{};
	GameObject go_digipenLogo, menuBG;
	f32 logo_transparency = 0.f;
	s8 isLogoDrawn = 0;
	s32 mouseX, mouseY;

	// UI stuff
	UIManager menuUI, popUpUI;
	std::unique_ptr<Button> startBtn, exitBtn, creditsBtn, yesBtn, noBtn, settingsBtn;
	std::unique_ptr<PopUp> quitPopUp;
	std::unique_ptr<PauseMenu> pauseMenu;

	GameObjectIDAssigner assigner;
	Audio& bgm = Audio::Instance();
}


void MainMenu_Load() {
	PRINT("Load: %s", "Main Menu\n");
	assigner.currIdCount = 0;

	cardinalsFont.Load_Font(Fonts::Alt, 160.f);
	cardinalsFont.Input_Text("cardinals");

	digipenFont.Load_Font(Fonts::Main, 30.f);
	digipenFont.Input_Text("All content (C) 2025 Digipen Institute of Technology Singapore. All Rights Reserved");

	menuUI.InitTooltipFont(Fonts::Main, 30.f); popUpUI.InitTooltipFont(Fonts::Main, 30.f);

	menuBGTexture.Load_Image(Images::Background);
	tex_digipenLogo.Load_Image(Images::DigipenLogo);
	startTexture.Load_Image(Images::StartBtn);
	exitTexture.Load_Image(Images::ExitBtn);
	creditsTexture.Load_Image(Images::CreditsBtn);
	settingsTexture.Load_Image(Images::SettingsBtn);
	prevTexture.Load_Image(Images::PrevBtn);

	// testing
	yesTexture.Load_Image(Images::YesBtn); noTexture.Load_Image(Images::NoBtn);
	popUpTexture.Load_Image(Images::PopUp);

	//Store Textures to Unload
	if (!textures.empty()) textures.clear();
	textures.push_back(&tex_digipenLogo);
	textures.push_back(&menuBGTexture);
	textures.push_back(&startTexture);
	textures.push_back(&exitTexture);
	textures.push_back(&creditsTexture);
	textures.push_back(&yesTexture);
	textures.push_back(&noTexture);
	textures.push_back(&popUpTexture);
	textures.push_back(&settingsTexture);
	textures.push_back(&prevTexture);

	if (!meshes.empty()) meshes.clear();
	meshes.push_back(&squareMesh);

	if (!texts.empty()) texts.clear();
	texts.push_back(&cardinalsFont);
	texts.push_back(&digipenFont);

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

	menuBG.SetMesh(squareMesh); menuBG.SetTexture(&menuBGTexture);
	menuBG.UpdatePosition(AEVec2{ 0.f, 0.f }); menuBG.UpdateScale(AEVec2{ menuBGTexture.Get_Image_Width(), menuBGTexture.Get_Image_Height() }); menuBG.UpdateRotation(0.f);
	menuBG.SetID(assigner.GetID());

	// creates button w relevant stuff stolen from gameobej
	startBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, 0.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	startBtn->SetTexture(&startTexture); startBtn->SetActive(true); startBtn->SetInteractable(true);
	startBtn->SetTooltipText("ur mom");
	startBtn->SetID(assigner.GetID());

	exitBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, -300.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	exitBtn->SetTexture(&exitTexture); exitBtn->SetActive(true); exitBtn->SetInteractable(true);
	exitBtn->SetID(assigner.GetID());

	creditsBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, -150.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	creditsBtn->SetTexture(&creditsTexture); creditsBtn->SetActive(true); creditsBtn->SetInteractable(true);
	creditsBtn->SetID(assigner.GetID());

	// 

	yesBtn = std::make_unique<Button>(squareMesh, AEVec2{ -100.f, -50.f }, 0.f, AEVec2{ yesTexture.Get_Image_Width(), yesTexture.Get_Image_Height() });
	yesBtn->SetTexture(&yesTexture); yesBtn->SetActive(true); yesBtn->SetInteractable(true);
	yesBtn->SetID(assigner.GetID());

	noBtn = std::make_unique<Button>(squareMesh, AEVec2{ -100.f, -250.f }, 0.f, AEVec2{ noTexture.Get_Image_Width(), noTexture.Get_Image_Height() });
	noBtn->SetTexture(&noTexture); noBtn->SetActive(true); noBtn->SetInteractable(true);
	noBtn->SetID(assigner.GetID());

	// not in game yet
	pauseMenu = std::make_unique<PauseMenu>(false);
	pauseMenu->SetTextures(&menuBGTexture, &startTexture, &exitTexture, &prevTexture);
	settingsBtn = std::make_unique<Button>(squareMesh, AEVec2{710.f,360.f}, 0.f, AEVec2{settingsTexture.Get_Image_Width(), settingsTexture.Get_Image_Height()});
	
	settingsBtn->SetTexture(&settingsTexture); settingsBtn->SetActive(true); settingsBtn->SetInteractable(true);
	settingsBtn->SetID(assigner.GetID());

	quitPopUp = std::make_unique<PopUp>(squareMesh, AEVec2{ 0,0 }, 0.f, AEVec2{ 600.f, 400.f });
	quitPopUp->SetTexture(&popUpTexture);
	quitPopUp->SetID(assigner.GetID());

	quitPopUp->AddChild(yesBtn.get());
	quitPopUp->AddChild(noBtn.get());

	// use them like this ik its weird asf but like. i'll make it better soon
	startBtn->SetOnClick([]() {
		PRINT("Start Pressed");
		UpdateGameManagerState(GameStateEnum::LevelMenu);
		});

	creditsBtn->SetOnClick([]() {
		PRINT("Credits Pressed");
		});

	exitBtn->SetOnClick([]() {
		PRINT("Exit Pressed");
		isGameRunning = 0;
		PRINT("Game Running = %d", isGameRunning);
		});

	settingsBtn->SetOnClick([]() {
		PRINT("Settings Pressed");
		pauseMenu->ShowMenu();
		menuUI.ForceResetToolTips();
		});

	yesBtn->SetOnClick([]() {
		PRINT("I WORK");
		});

	noBtn->SetOnClick([]() {
		PRINT("ME TOO!!!!");
		});

	// so we can control all buttons with a menuUI hehehehe
	menuUI.Add(startBtn.get());
	menuUI.Add(exitBtn.get());
	menuUI.Add(creditsBtn.get());
	menuUI.Add(settingsBtn.get());

	popUpUI.Add(quitPopUp.get());
	popUpUI.Add(yesBtn.get());
	popUpUI.Add(noBtn.get());

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

	float dt = AEFrameRateControllerGetFrameTime();

	if (AEInputCheckTriggered(AEVK_SPACE)) {
		logo_transparency = 1.f;
		isLogoDrawn = 1;
	}

	if (isLogoDrawn) {

		if (quitPopUp->ToggleActive(AEVK_6))
		{
			// reset only once im not gonna call ts 60 frames a sec
			bool active = quitPopUp->IsActive();
			if (active) {
				menuUI.ForceResetToolTips();
			}
		}

		if (pauseMenu->isVisible()) {
			pauseMenu->Update(dt);
		}
		else if (quitPopUp->IsActive()) {
			popUpUI.Update(dt);
		}
		else {
			menuUI.Update(dt); // freezes menuUI but i'll update this in the future to freeze the whole game
		}

		AEInputGetCursorPosition(&mouseX, &mouseY);
	}
	else {
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
	}
}

void MainMenu_Draw() {
	//PRINT("Draw: %s", "Main Menu\n");

	if (!isLogoDrawn) {
		go_digipenLogo.Draw();
		digipenFont.Draw_Text(AEVec2{ 0, -330.f }, 1.f, logo_transparency);
		return;
	}

	bool isOverlayActive = quitPopUp->IsActive() || pauseMenu->isVisible();
	if (isOverlayActive) AEGfxSetColorToMultiply(0.5f, 0.5f, 0.5f, 1.0f);

	menuBG.Draw();
	cardinalsFont.Draw_Text(0, 200.f);
	menuUI.Draw();

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	pauseMenu->Draw();
	popUpUI.Draw();
}

void MainMenu_Free() {
	PRINT("Free: %s", "Main Menu\n");

	bgm.Stop_Audio();
	bgm.Free_Audio_System();

	popUpUI.Exit();
	menuUI.Exit();
}

void MainMenu_Unload() {
	PRINT("Unload: %s", "Main Menu\n");
	pauseMenu.reset();

	if (MassUnload()) PRINT("Unload Success");
}
