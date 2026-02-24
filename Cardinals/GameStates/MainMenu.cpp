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
//auto& input = InputHandler::GetInstance();

namespace {

	AEGfxVertexList* squareMesh;
	Text cardinalsFont, digipenFont, tooltipFont;

	Texture startTexture{}, exitTexture{}, prevTexture{}, creditsTexture{}, yesTexture{}, noTexture{}, popUpTexture{}, settingsTexture{}, barTexture{}, knobMasterTexture{}, knobMusicTexture{}, knobEFXTexture{}, cardBgTexture{};
	Texture menuBGTexture{};

	//Digipen logo transition
	Texture tex_digipenLogo{};
	GameObject go_digipenLogo, menuBG;
	f32 logo_transparency = 0.f;
	s8 isLogoDrawn = 0;
	s32 mouseX, mouseY;
	bool quitMenuOpen = false;

	// UI stuff
	UIManager menuUI;
	std::unique_ptr<Button> startBtn, exitBtn, creditsBtn;
	UIManager quitUI;
	std::unique_ptr<Button> yesBtn, noBtn;
	std::unique_ptr<PauseMenu> pauseMenu;
	std::unique_ptr<Button> settingsBtn;

	AudioManager& bgm = AudioManager::Instance();
	InputHandler& input = InputHandler::GetInstance();
}


void MainMenu_Load() {
	PRINT("Load: %s", "Main Menu\n");

	cardinalsFont.Load_Font(Fonts::Alt, 160.f);
	cardinalsFont.Input_Text("cardinals");

	digipenFont.Load_Font(Fonts::Main, 30.f);
	digipenFont.Input_Text("All content (C) 2025 Digipen Institute of Technology Singapore. All Rights Reserved");

	menuUI.InitTooltipFont(Fonts::Main, 30.f);
	quitUI.InitTooltipFont(Fonts::Main, 30.f);

	menuBGTexture.Load_Image(Images::Background);
	tex_digipenLogo.Load_Image(Images::DigipenLogo);
	startTexture.Load_Image(Images::StartBtn);
	exitTexture.Load_Image(Images::ExitBtn);
	creditsTexture.Load_Image(Images::CreditsBtn);
	settingsTexture.Load_Image(Images::SettingsBtn);
	prevTexture.Load_Image(Images::PrevBtn);
	barTexture.Load_Image(Images::Bar);
	knobMasterTexture.Load_Image(Images::KnobMaster);
	knobMusicTexture.Load_Image(Images::KnobMusic);
	knobEFXTexture.Load_Image(Images::KnobEFX);
	cardBgTexture.Load_Image(Images::CardBg);

	// testing
	yesTexture.Load_Image(Images::Button);
	noTexture.Load_Image(Images::Button);
	popUpTexture.Load_Image(Images::Button);

	//Store Textures to Unload
	if (!memoryManager.textures.empty()) memoryManager.textures.clear();
	memoryManager.AddTexture(&tex_digipenLogo);
	memoryManager.AddTexture(&menuBGTexture);
	memoryManager.AddTexture(&startTexture);
	memoryManager.AddTexture(&exitTexture);
	memoryManager.AddTexture(&creditsTexture);
	memoryManager.AddTexture(&yesTexture);
	memoryManager.AddTexture(&noTexture);
	memoryManager.AddTexture(&popUpTexture);
	memoryManager.AddTexture(&settingsTexture);
	memoryManager.AddTexture(&prevTexture);
	memoryManager.AddTexture(&barTexture);
	memoryManager.AddTexture(&knobMasterTexture);
	memoryManager.AddTexture(&knobMusicTexture);
	memoryManager.AddTexture(&knobEFXTexture);
	memoryManager.AddTexture(&cardBgTexture);

	if (!memoryManager.meshes.empty()) memoryManager.meshes.clear();
	memoryManager.AddMesh(&squareMesh);

	if (!memoryManager.fonts.empty()) memoryManager.fonts.clear();
	memoryManager.AddFont(&cardinalsFont);
	memoryManager.AddFont(&digipenFont);
}

void MainMenu_Initialize()
{
	bgm.Init();
	bgm.LoadManifest("../../Assets/AudioManifest.txt");
	// play the bgm for the main menu, looped
	bgm.PlayAudio("BGM", AudioType::MUSIC, 1.f, -1);

	squareMesh = RectMeshCenter();
	go_digipenLogo.SetMesh(squareMesh);

	menuBG.SetMesh(squareMesh); menuBG.SetTexture(&menuBGTexture);
	menuBG.UpdatePosition(AEVec2{ 0.f, 0.f }); menuBG.UpdateScale(AEVec2{ menuBGTexture.Get_Image_Width(), menuBGTexture.Get_Image_Height() }); menuBG.UpdateRotation(0.f);

	pauseMenu = std::make_unique<PauseMenu>(squareMesh, false, [](){}, nullptr);

	pauseMenu->SetTextures(PauseMenuTextures{ &menuBGTexture, &cardBgTexture, &barTexture, 
		&knobMasterTexture, &knobMusicTexture, &knobEFXTexture, &prevTexture,nullptr,nullptr, nullptr, &digipenFont });

	// BUTTONS

	startBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, 0.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	startBtn->SetTexture(&startTexture); startBtn->SetActive(true); startBtn->SetInteractable(true);
	startBtn->SetTooltipText("ur mom");

	exitBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, -300.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	exitBtn->SetTexture(&exitTexture); exitBtn->SetActive(true); exitBtn->SetInteractable(true);

	creditsBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, -150.f }, 0.f, AEVec2{ startTexture.Get_Image_Width(), startTexture.Get_Image_Height() });
	creditsBtn->SetTexture(&creditsTexture); creditsBtn->SetActive(true); creditsBtn->SetInteractable(true); 

	yesBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, -50.f }, 0.f, AEVec2{ 200.f, 100.f });
	yesBtn->SetTexture(&yesTexture); yesBtn->SetActive(true); yesBtn->SetInteractable(true);
	yesBtn->SetTooltipText("");

	noBtn = std::make_unique<Button>(squareMesh, AEVec2{ 0.f, -175.f }, 0.f, AEVec2{ 200.f, 100.f });
	noBtn->SetTexture(&noTexture); noBtn->SetActive(true); noBtn->SetInteractable(true);
	noBtn->SetTooltipText("");

	settingsBtn = std::make_unique<Button>(squareMesh, AEVec2{710.f,360.f}, 0.f, AEVec2{settingsTexture.Get_Image_Width(), settingsTexture.Get_Image_Height()});
	
	settingsBtn->SetTexture(&settingsTexture); settingsBtn->SetActive(true); settingsBtn->SetInteractable(true);
	settingsBtn->SetOnClick([]() 
		{
			pauseMenu->SetActive(true);
			//menuUI.ForceResetToolTips();
		});
	
	quitUI.Add(yesBtn.get());
	quitUI.Add(noBtn.get());

	// BUTTON CLICKS

	startBtn->SetOnClick([]() {
		PRINT("Start Pressed");
		UpdateGameManagerState(GameStateEnum::LevelMenu);
		});

	creditsBtn->SetOnClick([]() {
		PRINT("Credits Pressed");
		});

	exitBtn->SetOnClick([]() {
		PRINT("Exit Pressed");
		quitMenuOpen = true;
		});

	yesBtn->SetOnClick([]() {
		isGameRunning = 0;
		});

	noBtn->SetOnClick([]() {
		quitMenuOpen = false;
		});

	// control all buttons with the manager
	// menuUI.Add(pauseMenu.get());
	menuUI.Add(startBtn.get()); menuUI.Add(exitBtn.get()); menuUI.Add(creditsBtn.get()); menuUI.Add(settingsBtn.get());

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

	float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());

	if (input.isKeyTriggered(AEVK_SPACE)) {
		logo_transparency = 1.f;
		isLogoDrawn = 1;
	}

	//if (pauseMenu->IsActive())
	//	pauseMenu->Update(dt);
	//else
	//	menuUI.Update(dt);

	if (isLogoDrawn) 
	{
		if (quitMenuOpen) { quitUI.Update(dt); }
		else if (pauseMenu->IsActive()) {
			pauseMenu->Update(dt);
			// if pause menu is open, only update the pause menu and check for its clicks
			if (input.isMouseBtnTriggered(AEVK_LBUTTON)) {
				pauseMenu->OnClick();
			}
		}
		else { menuUI.Update(dt); }
		AEInputGetCursorPosition(&mouseX, &mouseY);
	}
	else 
	{
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

	menuBG.Draw();
	cardinalsFont.Draw_Text(0, 200.f);
	menuUI.Draw();
	if (pauseMenu->IsActive()) {
		pauseMenu->Draw();
	}

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	if (!quitMenuOpen) return;
	AEGfxSetColorToMultiply(.2f, .2f, .2f, 1.0f);
	menuBG.Draw();
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	digipenFont.Input_Text("Are you sure?");
	digipenFont.Draw_Text(AEVec2{ 0, 150.f }, 1.f, logo_transparency);
	quitUI.Draw();
	digipenFont.Change_Text_Color(Colors::Black);
	digipenFont.Input_Text("Yeah!");
	digipenFont.Draw_Text(AEVec2{ 0, -50.f }, 1.f, logo_transparency);
	digipenFont.Input_Text("Nah...");
	digipenFont.Draw_Text(AEVec2{ 0, -175.f }, 1.f, logo_transparency);
	digipenFont.Change_Text_Color(Colors::White);
}

void MainMenu_Free() {
	PRINT("Free: %s", "Main Menu\n");

	bgm.StopAudio();
	bgm.Free();

	quitUI.Exit();
	menuUI.Exit();
}

void MainMenu_Unload() {
	PRINT("Unload: %s", "Main Menu\n");
	pauseMenu.reset();

	if (memoryManager.MassUnload()) PRINT("Unload Success");
}
