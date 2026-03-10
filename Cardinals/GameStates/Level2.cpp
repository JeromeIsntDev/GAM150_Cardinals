#include "Level2.hpp"
#include "GameEditor.hpp"

extern bool isGamePaused;
extern bool isGameOver;

namespace {
	AEGfxVertexList* squareMesh;
	Text digipenFont, pauseTitleFont;

	
	//PLAYER VARIABLES
	f32 playerMovespeed{ 300.f };
	int  playerHealth{ 3 };
	Texture playerTexture[2]{};
	ParticleEffect playerEffect;
	ParticleEffect gravityEffect;
	Player mainPlayer;

	Texture placeholderTexture;
	Texture verticalPlatformTex;
	Texture horizontalPlatformTex;

	f32 levelTime{};
	auto& input = InputHandler::GetInstance();

	//UIManager pauseUI;
	//std::unique_ptr<Button> conBtn, quitBtn;

	std::unique_ptr <Overlay> pauseMenu;
	std::unique_ptr<Overlay> gameOverMenu;
	Texture barTexture{}, knobMasterTexture{}, knobMusicTexture{},
		knobEFXTexture{}, cardBgTexture{}, prevTexture{},
		restartTexture{}, resumeTexture{}, homeTexture{},
		nextTexture{}, retryTexture{}, candyTexture{},
		greyCandyTexture{};
}

void Level2_Load() {
	PRINT("Load: %s", "Level 2\n");
	
	squareMesh = RectMeshCenter();
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 2");
	digipenFont.Change_Text_Color(Colors::Black);

	//placeholder.Load_Image(Images::Placeholder);
	playerTexture[0].Load_Image(Images::Player);
	playerTexture[1].Load_Image(Images::PlayerJump);
	placeholderTexture.Load_Image(Images::Placeholder);


	pauseTitleFont.Load_Font(Fonts::Alt, 72.f);
	prevTexture.Load_Image(Images::PrevBtn);
	barTexture.Load_Image(Images::Bar);
	knobMasterTexture.Load_Image(Images::KnobMaster);
	knobMusicTexture.Load_Image(Images::KnobMusic);
	knobEFXTexture.Load_Image(Images::KnobEFX);
	cardBgTexture.Load_Image(Images::CardBg);
	restartTexture.Load_Image(Images::RestartBtn);
	resumeTexture.Load_Image(Images::ResumeBtn);
	homeTexture.Load_Image(Images::HomeBtn);
	nextTexture.Load_Image(Images::NextBtn);
	retryTexture.Load_Image(Images::RetryBtn);
	candyTexture.Load_Image(Images::Candy);
	greyCandyTexture.Load_Image(Images::CandyGrey);

	if (!memoryManager.textures.empty()) memoryManager.textures.clear();

	memoryManager.AddTextures({
			&placeholderTexture,
			&verticalPlatformTex,
			&horizontalPlatformTex,
			&prevTexture,
			&barTexture,
			&knobMasterTexture,
			&knobMusicTexture,
			&knobEFXTexture,
			&cardBgTexture,
			&restartTexture,
			&resumeTexture,
			&homeTexture,
			&nextTexture,
			&retryTexture,
			&candyTexture,
			&greyCandyTexture
		});

	for (Texture& T : playerTexture) {
		memoryManager.AddTexture(&T);
	}
	if (!memoryManager.textures.empty()) memoryManager.textures.clear();
	memoryManager.AddMesh(&squareMesh);
	//memoryManager.AddMesh(&tileMesh);

	if (!memoryManager.fonts.empty()) memoryManager.fonts.clear();
	memoryManager.AddFont(&digipenFont);
	memoryManager.AddFont(&pauseTitleFont);
}

void Level2_Initialize()
{
	PRINT("Initialize: %s", "Level 2\n");

	playerEffect.Init();
	gravityEffect.Init();

	//rectMesh = RectMeshCenter();

	//placeholderObj.SetMesh(rectMesh);
	//placeholderObj.SetTexture(&placeholder);
	//placeholderObj.UpdateScale(AEVec2{ 1600.f, 900.f });
	//placeholderObj.UpdateRotation(0.f);

	mainPlayer.InitializePlayer(playerHealth, playerMovespeed, false,
		0.f, 0.f, squareMesh, AEGfxGetWinMinX() + 225.f, -25.f, false);
	mainPlayer.SetMesh(squareMesh);
	mainPlayer.SetTexture(&playerTexture[0]);
	mainPlayer.UpdateScale(mainPlayer.p_Scale);
	mainPlayer.UpdateRotation(0.f);
	mainPlayer.UpdatePosition(mainPlayer.GetPosition());
	mainPlayer._tag = GameObjectTag::Player;
	mainPlayer.SetHealth(3);

	levelTime = 0.f;

	pauseMenu = std::make_unique<Overlay>(squareMesh, PAUSE, -1,
		[]() {isGamePaused = 0; },
		[]() {isGamePaused = 0; UpdateGameManagerState(GameStateEnum::MainMenu); },
		[]() {isGamePaused = 0; UpdateGameManagerState(GameStateEnum::Level1); });

	pauseMenu->SetTextures(
		PauseMenuTextures{
			&placeholderTexture, &cardBgTexture, &barTexture,
			&knobMasterTexture, &knobMusicTexture, &knobEFXTexture,
			&prevTexture, &resumeTexture, &restartTexture,
			&homeTexture, &pauseTitleFont
		});
	isGamePaused = 0;

	gameOverMenu = std::make_unique<Overlay>(
		squareMesh,
		LEVEL, 2,
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Level3); },
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::MainMenu); },
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Level2); });

	gameOverMenu->SetTextures(
		GameOverTextures{
			&placeholderTexture, &cardBgTexture, &candyTexture,
			&greyCandyTexture, &retryTexture, &homeTexture,
			&nextTexture, &pauseTitleFont, &digipenFont
		});
	isGameOver = 0;
	//conBtn = std::make_unique<Button>(rectMesh,
	//	AEVec2{ 0.f, 50.f }, 
	//	0.f, 
	//	AEVec2{ 200.f, 100.f });
	//conBtn->SetTexture(&placeholder); conBtn->SetActive(true); conBtn->SetInteractable(true);
	//conBtn->SetOnClick([]() {
	//	PRINT("Continue Pressed\n");
	//	isGamePaused = 0;
	//	});

	//quitBtn = std::make_unique<Button>(rectMesh,
	//	AEVec2{ 0.f, -100.f },
	//	0.f,
	//	AEVec2{ 200.f, 100.f });
	//quitBtn->SetTexture(&placeholder); quitBtn->SetActive(true); quitBtn->SetInteractable(true);
	//quitBtn->SetOnClick([]() {
	//	PRINT("Exit Pressed\n");
	//	UpdateGameManagerState(GameStateEnum::MainMenu);
	//	});

	//pauseUI.Add(conBtn.get());
	//pauseUI.Add(quitBtn.get());

}

void Level2_Update() {
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	if (pauseMenu) //Checks if pauseMenu Exists
	{
		if (AEInputCheckTriggered(AEVK_P) && !isGameOver)
		{
			isGamePaused = !isGamePaused;
			pauseMenu->SetActive(isGamePaused);
		}

		pauseMenu->Update(dt);
			if (pauseMenu->IsActive() && input.isKeyTriggered(AEVK_LBUTTON))
			pauseMenu->OnClick();
	}
	else {
		PRINT("[WARNING] => Pause Menu Unique Pointer Not Initialized!");
	}


	if (gameOverMenu)
	{
		if (AEInputCheckTriggered(AEVK_O) && !isGameOver)
		{
			isGameOver = true;
			isGamePaused = false;
			gameOverMenu->UpdateGameOverMenu(levelTime, mainPlayer.GetPlayerHealth());

			pauseMenu->SetActive(false);
			gameOverMenu->SetActive(true);
		}

		gameOverMenu->Update(dt);

		if (gameOverMenu->IsActive() && input.isKeyTriggered(AEVK_LBUTTON))
			gameOverMenu->OnClick();
	}
	else {
		PRINT("[WARNING] => Game Over Menu Unique Pointer Not Initialized!");
	}

	if (mainPlayer.getGameOver()) {
		isGameOver = true;
		gameOverMenu->UpdateGameOverMenu(levelTime, mainPlayer.GetPlayerHealth());
		if (gameOverMenu)
			gameOverMenu->SetActive(true);
		mainPlayer.SetGameOver(false);
	}

	//PRINT("Update: %s", "Main Menu\n");
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.9f, 0.85f, 0.65f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);

	if (!isGamePaused && !isGameOver) {
		levelTime += static_cast<f32>(AEFrameRateControllerGetFrameTime());
	}

	if (isGamePaused || isGameOver) return;

	//if (input.isMouseBtnTriggered(AEVK_P)) {
	//	isGamePaused = ~isGamePaused;
	//}

	
	//if (!isGamePaused) return;
	//pauseUI.Update(0.f);

	//PLAYER ITEMS//
	mainPlayer.PlayerMovement(dt, mainPlayer.grav);
	if (mainPlayer.GetPlayerJump()) {
		mainPlayer.SetTexture(&playerTexture[1]);
	}
	else {
		mainPlayer.SetTexture(&playerTexture[0]);
	}
	mainPlayer.p_isGrounded = false;
	mainPlayer.hasCollided = false;
	mainPlayer.canMoveLeft = true;
	mainPlayer.canMoveRight = true;
	//mainPlayer.CheckBinaryCollision(&grid, mainPlayer.grav);
	//mainPlayer.PlayerCollision(grid.objs, mainPlayer.grav);

	playerEffect.Update(dt, mainPlayer.GetPosition());

}

void Level2_Draw() {
	mainPlayer.Draw();
	//playerEffect.DrawParticle();

	digipenFont.Input_Text("Time: ");
	digipenFont.Draw_Text(675.f, 400.f);
	digipenFont.Input_Float(levelTime);
	digipenFont.Draw_Text_Corner(725.f, 387.f);

	digipenFont.Input_Text("Level 2");
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);

	if (pauseMenu) pauseMenu->Draw();
	if (gameOverMenu) gameOverMenu->Draw();

}

void Level2_Free() {
	PRINT("Free: %s", "Level 2\n");
	playerEffect.Clear();
	gravityEffect.Clear();
}

void Level2_Unload() {
	PRINT("Unload: %s", "Level 2\n");
	pauseMenu.reset();
	gameOverMenu.reset();
	//pauseUI.Exit();
	if (isGamePaused) isGamePaused = 0;
	if (isGameOver) isGameOver = 0;
	if (mainPlayer.getGameOver()) mainPlayer.SetGameOver(false);
	if (memoryManager.MassUnload()) PRINT("Unload Success");
}
