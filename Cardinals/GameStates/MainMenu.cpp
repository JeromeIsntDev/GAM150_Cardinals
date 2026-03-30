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

namespace {

	AEGfxVertexList* squareMesh;
	Text cardinalsFont, digipenFont, tooltipFont, settingsFont;

	// note frm jelly texUnload kinda unloads everything without needing memorymanager after i integrate everytthing saur
	Texture prevTexture{}, barTexture{}, knobMasterTexture{}, knobMusicTexture{}, knobEFXTexture{}, cardBgTexture{};
	Texture menuBGTexture{};
	Texture yesTexture{}, noTexture{};

	//Digipen logo transition
	Texture tex_digipenLogo{};
	GameObject go_digipenLogo, menuBG;
	f32 logo_transparency = 0.f;
	s8 isDigipenLogoDrawn = 0;
	s32 mouseX, mouseY;
	bool quitMenuOpen = false;

	// UI stuff
	UIManager menuUI;
	std::unique_ptr<Button> startBtn, exitBtn, creditsBtn;
	UIManager quitUI;
	std::unique_ptr<Button> yesBtn, noBtn;
	std::unique_ptr<Overlay> settingsMenu;
	std::unique_ptr<Button> settingsBtn;

	AudioManager& bgm = AudioManager::Instance();
	TextureManager& go_texture = TextureManager::GetInstance();
	InputHandler& input = InputHandler::GetInstance();
}


void MainMenu_Load() {
	PRINT("[DEBUG | LEVEL] => Load: %s", "Main Menu\n");

	bgm.LoadManifest("../../Assets/AudioManifest.txt");
	go_texture.LoadManifest("../../Assets/TextureManifest.txt");

	// for persistent objs beyond manifest
	go_texture.SetPersistent("TileSheet");
	go_texture.SetPersistent("playerIdle");
	go_texture.SetPersistent("BG_BACKMOST");
	go_texture.SetPersistent("BG_BACKMOUNTAIN");
	go_texture.SetPersistent("BG_CLOUDS");
	go_texture.SetPersistent("BG_FRONTMOUNTAIN");

	cardinalsFont.Load_Font(Fonts::Alt, 160.f);
	cardinalsFont.Input_Text("cardinals");

	digipenFont.Load_Font(Fonts::Main, 30.f);
	digipenFont.Input_Text("All content (C) 2026 Digipen Institute of Technology Singapore. All Rights Reserved");

	menuUI.InitTooltipFont(Fonts::Main, 30.f);
	quitUI.InitTooltipFont(Fonts::Main, 30.f);

	settingsFont.Load_Font(Fonts::Alt, 72.f);

	menuBGTexture.Load_Image(Images::Background);
	tex_digipenLogo.Load_Image(Images::DigipenLogo);

	prevTexture.Load_Image(Images::PrevBtn);
	barTexture.Load_Image(Images::Bar);
	knobMasterTexture.Load_Image(Images::KnobMaster);
	knobMusicTexture.Load_Image(Images::KnobMusic);
	knobEFXTexture.Load_Image(Images::KnobEFX);
	cardBgTexture.Load_Image(Images::CardBg);

	yesTexture.Load_Image(Images::Button);
	noTexture.Load_Image(Images::Button);

	//Store Textures to Unload
	if (!memoryManager.textures.empty()) memoryManager.textures.clear();
	memoryManager.AddTexture(&tex_digipenLogo);
	memoryManager.AddTexture(&menuBGTexture);
	
	memoryManager.AddTexture(&yesTexture);
	memoryManager.AddTexture(&noTexture);

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
	memoryManager.AddFont(&settingsFont);
}

void MainMenu_Initialize()
{	
	PRINT("[DEBUG | LEVEL] => Initialize: %s", "Main Menu\n");

	bgm.Init();
	bgm.PlayAudio("BGM", AudioType::MUSIC, 1.f, -1);

	squareMesh = RectMeshCenter();
	go_digipenLogo.SetMesh(squareMesh);

	menuBG.SetMesh(squareMesh); menuBG.SetTexture(&menuBGTexture);
	menuBG.UpdatePosition(AEVec2{ 0.f, 0.f }); 
	menuBG.UpdateScale(AEVec2{ menuBGTexture.Get_Image_Width(), menuBGTexture.Get_Image_Height() }); 
	menuBG.UpdateRotation(0.f);

	settingsMenu = std::make_unique<Overlay>(squareMesh, SETTINGS, -1, [](){}, nullptr);

	settingsMenu->SetTextures(
		PauseMenuTextures{ 
			&menuBGTexture, &cardBgTexture, &barTexture, 
			&knobMasterTexture, &knobMusicTexture, &knobEFXTexture, 
			&prevTexture, nullptr, nullptr, nullptr, &settingsFont 
		});

	// BUTTONS

	// follow new format hahaha... ptrs.. ahhahahah
	// im crine bro
	Texture* startTex = go_texture.Get("StartBtn");
	Texture* exitTex = go_texture.Get("ExitBtn");
	Texture* creditsTex = go_texture.Get("CreditsBtn");
	Texture* settingsTex = go_texture.Get("SettingsBtn");

	startBtn = std::make_unique<Button>(
		squareMesh, 
		AEVec2{ 0.f, 0.f }, 
		0.f, 
		AEVec2{ startTex->Get_Image_Width(), startTex->Get_Image_Height() }
	);
	startBtn->SetTexture(startTex); startBtn->SetActive(true); startBtn->SetInteractable(true);
	startBtn->SetTooltipText("Intro Level Select!");

	creditsBtn = std::make_unique<Button>(
		squareMesh, 
		AEVec2{ 0.f, -300.f }, 
		0.f, 
		AEVec2{ creditsTex->Get_Image_Width(), creditsTex->Get_Image_Height() }
	);
	creditsBtn->SetTexture(creditsTex); creditsBtn->SetActive(true); creditsBtn->SetInteractable(true);
	creditsBtn->SetTooltipText("Credits & Thanks!");

	exitBtn = std::make_unique<Button>(
		squareMesh, 
		AEVec2{ 0.f, -150.f },
		0.f, AEVec2{ exitTex->Get_Image_Width(), exitTex->Get_Image_Height() }
	);
	exitBtn->SetTexture(exitTex); exitBtn->SetActive(true); exitBtn->SetInteractable(true);
	exitBtn->SetTooltipText("Quit Menu!");

	// HAVENT CHANGED
	yesBtn = std::make_unique<Button>(
		squareMesh, 
		AEVec2{ 0.f, -50.f }, 
		0.f, 
		AEVec2{ 200.f, 100.f }
	);
	yesBtn->SetTexture(&yesTexture); yesBtn->SetActive(true); yesBtn->SetInteractable(true);
	yesBtn->SetTooltipText("");

	// HAVENT CHANGED
	noBtn = std::make_unique<Button>(
		squareMesh, 
		AEVec2{ 0.f, -175.f }, 
		0.f, 
		AEVec2{ 200.f, 100.f }
	);
	noBtn->SetTexture(&noTexture); noBtn->SetActive(true); noBtn->SetInteractable(true);
	noBtn->SetTooltipText("");

	settingsBtn = std::make_unique<Button>(
		squareMesh, 
		AEVec2{710.f,360.f}, 
		0.f, 
		AEVec2{ settingsTex->Get_Image_Width(), settingsTex->Get_Image_Height()}
	);
	settingsBtn->SetTexture(settingsTex); settingsBtn->SetActive(true); settingsBtn->SetInteractable(true);
	settingsBtn->SetTooltipText("Settings!");
	settingsBtn->SetOnClick(
		[]() { settingsMenu->SetActive(true); }
	);

	quitUI.Add(yesBtn.get());
	quitUI.Add(noBtn.get());

	// BUTTON CLICKS

	startBtn->SetOnClick([]() {
		PRINT("Start Pressed");
		UpdateGameManagerState(GameStateEnum::LevelMenu);
		});

	creditsBtn->SetOnClick([]() {
		PRINT("Credits Pressed");
		UpdateGameManagerState(GameStateEnum::Credits);
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
	menuUI.Add(startBtn.get()); 
	menuUI.Add(exitBtn.get()); 
	menuUI.Add(creditsBtn.get()); 
	menuUI.Add(settingsBtn.get());

	go_digipenLogo.SetTexture(&tex_digipenLogo);
	go_digipenLogo.UpdatePosition({0.f,0.f});
	go_digipenLogo.UpdateScale(
		AEVec2{ 
			tex_digipenLogo.Get_Image_Width(),
			tex_digipenLogo.Get_Image_Height() 
		});
	go_digipenLogo.UpdateRotation(0.f);
}

void MainMenu_Update() {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(Colors::PastelPurple.r, Colors::PastelPurple.g, Colors::PastelPurple.b);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	AEGfxSetTransparency(logo_transparency);

	float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());

	if (input.isKeyTriggered(AEVK_SPACE)) {
		logo_transparency = 1.f;
		isDigipenLogoDrawn = 1;
	}

	if (isDigipenLogoDrawn) 
	{
		if (logo_transparency < 1.f) {
			logo_transparency += dt * 4.f;
		}
		if (quitMenuOpen) { quitUI.Update(dt); }
		else if (settingsMenu->IsActive()) {
			settingsMenu->Update(dt);

			if (input.isKeyTriggered(AEVK_LBUTTON)) {
				settingsMenu->OnClick();
			}
		}
		else { menuUI.Update(dt); }
		AEInputGetCursorPosition(&mouseX, &mouseY);
	}
	else 
	{
		if (AEFrameRateControllerGetFrameCount() < 100) {
			logo_transparency += dt;
		}
		else if (AEFrameRateControllerGetFrameCount() > 100 && (AEFrameRateControllerGetFrameCount() < 200)) {
			logo_transparency -= dt;
		}
		else if (AEFrameRateControllerGetFrameCount() > 200) {
			isDigipenLogoDrawn = 1;
		}
	}
}

void MainMenu_Draw() {

	if (!isDigipenLogoDrawn) {
		go_digipenLogo.Draw();
		digipenFont.Draw_Text(AEVec2{ 0, -330.f }, 1.f, logo_transparency);
		return;
	}

	menuBG.Draw();
	cardinalsFont.Draw_Text(0, 200.f);
	menuUI.Draw();
	if (settingsMenu->IsActive()) {
		settingsMenu->Draw();
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
	PRINT("[DEBUG | LEVEL] => Free: %s", "Main Menu\n");

	bgm.StopAudio();
	//bgm.Free();

	quitUI.Exit();
	menuUI.Exit();
}

void MainMenu_Unload() {
	PRINT("[DEBUG | LEVEL] => Unload: %s", "Main Menu\n");
	settingsMenu.reset();

	//tex.UnloadAll(); // this will unload all tex so duneed memmanager but like snsnoorkk

	if (memoryManager.MassUnload()) PRINT("[MEM DEBUG] => Main Menu Unload Success\n");
}
