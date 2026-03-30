#include "../GameStates/Level5.hpp"
#include "GameEditor.hpp"

void Level5_Parallax();
void Level5_UIDraw();

extern bool isGamePaused;
extern bool isGameOver;

namespace {
	AEGfxVertexList* squareMesh;
	Text digipenFont, pauseTitleFont;


	//PLAYER VARIABLES
	f32 playerMovespeed{ 300.f };
	int  playerHealth{ 3 };
	Texture* playerTexture[2]{};
	ParticleEffect playerEffect;
	ParticleEffect gravityEffect;
	CameraEffects camEffect;

	TextureManager& tex = TextureManager::GetInstance();
	AudioManager& bgm = AudioManager::Instance();

	Texture* placeholderTexture;
	Texture verticalPlatformTex;
	Texture horizontalPlatformTex;

	auto& input = InputHandler::GetInstance();

	//UIManager pauseUI;
	//std::unique_ptr<Button> conBtn, quitBtn;

	std::unique_ptr <Overlay> pauseMenu;
	std::unique_ptr<Overlay> gameOverMenu;
	Texture* barTexture{}, * knobMasterTexture{}, * knobMusicTexture{},
		* knobEFXTexture{}, * cardBgTexture{}, * prevTexture{},
		* restartTexture{}, * resumeTexture{}, * homeTexture{},
		* nextTexture{}, * retryTexture{}, * candyTexture{},
		* greyCandyTexture{};

	f32 levelTime{};

	GameObject gamelives;

	Texture* backgrounds[4]{};
	GameObject backgroundObjs[4]{};

	Grid grid;
	Player mainPlayer;

	std::string const& savePath{ "../../Saves/Level5_GameObjects.json" };
}

void Level5_Load() {
	PRINT("Load: %s", "Level 5\n");

	squareMesh = RectMeshCenter();
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Change_Text_Color(Colors::Black);

	pauseTitleFont.Load_Font(Fonts::Alt, 72.f);

	if (!memoryManager.textures.empty()) memoryManager.textures.clear();

	if (!memoryManager.textures.empty()) memoryManager.textures.clear();
	memoryManager.AddMesh(&squareMesh);

	if (!memoryManager.fonts.empty()) memoryManager.fonts.clear();
	memoryManager.AddFont(&digipenFont);
	memoryManager.AddFont(&pauseTitleFont);
	mainPlayer.newAnim.SetAnimArr();

	std::vector<GameObject> objData = ReadJsonToObjects(savePath.c_str());
	grid.Load(tex.Get("TileSheet"), 900, 1200, 300, 300);
	grid.Initialize();
	for (int i{}; i < objData.size(); ++i) {
		objData[i].SetMesh(grid.tileMesh);
		objData[i].SetTexture(&grid.tileTexture);

		AEVec2& pos = objData[i].GetPosition();
		objData[i].UpdatePosition(pos);
		u32 tag = static_cast<u32>(objData[i]._tag);
		grid.WorldToGrid(&pos.x, &pos.y);
		grid.SpawnNewTile(tag, objData[i].GetSlice(), pos.x, pos.y, objData[i].GetRotation());
	}
}

void Level5_Initialize() {
	PRINT("Initialize: %s", "Level 5\n");
	bgm.PlayAudio("GAMEPLAY", AudioType::MUSIC, 1.f, -1);

	playerEffect.Init();
	gravityEffect.Init();

	placeholderTexture = tex.Get("Placeholder");
	cardBgTexture = tex.Get("CardBg");
	prevTexture = tex.Get("PrevBtn");
	barTexture = tex.Get("Bar");
	knobMasterTexture = tex.Get("KnobMaster");
	knobMusicTexture = tex.Get("KnobMusic");
	knobEFXTexture = tex.Get("KnobEFX");
	restartTexture = tex.Get("RestartBtn");
	resumeTexture = tex.Get("ResumeBtn");
	homeTexture = tex.Get("HomeBtn");
	nextTexture = tex.Get("NextBtn");
	retryTexture = tex.Get("RetryBtn");
	candyTexture = tex.Get("Candy");
	greyCandyTexture = tex.Get("CandyGrey");
	playerTexture[0] = tex.Get("Player");
	playerTexture[1] = tex.Get("PlayerJump");
	backgrounds[0] = tex.Get("BG_BACKMOST");
	backgrounds[1] = tex.Get("BG_CLOUDS");
	backgrounds[2] = tex.Get("BG_MOUNTAIN");
	backgrounds[3] = tex.Get("BG_FRONTMOUNTAIN");

	mainPlayer.InitializePlayer(playerHealth, playerMovespeed, false,
		0.f, 0.f, squareMesh, AEGfxGetWinMinX() + 75.f, AEGfxGetWinMinY() + 100.f, false);
	mainPlayer.SetMesh(squareMesh);
	mainPlayer.SetTexture(playerTexture[0]);
	mainPlayer.UpdateScale(AEVec2{ 50.f,50.f });
	mainPlayer.UpdateRotation(0.f);
	mainPlayer.UpdatePosition(mainPlayer.GetPosition());
	mainPlayer._tag = GameObjectTag::Player;
	mainPlayer.SetHealth(3);

	camEffect.Init(mainPlayer.grav);

	levelTime = 0.f;

	pauseMenu = std::make_unique<Overlay>(squareMesh, PAUSE, -1,
		[]() {isGamePaused = 0; },
		[]() {isGamePaused = 0; UpdateGameManagerState(GameStateEnum::MainMenu); },
		[]() {isGamePaused = 0; UpdateGameManagerState(GameStateEnum::Level5); });

	pauseMenu->SetTextures(
		PauseMenuTextures{
			placeholderTexture, cardBgTexture, barTexture,
			knobMasterTexture, knobMusicTexture, knobEFXTexture,
			prevTexture, resumeTexture, restartTexture,
			homeTexture, &pauseTitleFont
		});
	isGamePaused = 0;
	if (counter::specialCount < 4) {
		std::cout << "Initializing without special" << std::endl;
		gameOverMenu = std::make_unique<Overlay>(
			squareMesh,
			LEVEL, 5,
			[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Credits); },
			[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::MainMenu); },
			[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Level5); });
	}else{
		std::cout << "Initializing with special" << std::endl;
		gameOverMenu = std::make_unique<Overlay>(
			squareMesh,
			LEVEL, 5,
			[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Special); },
			[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::MainMenu); },
			[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Level5); });
	}

	gameOverMenu->SetTextures(
		GameOverTextures{
			placeholderTexture, cardBgTexture, candyTexture,
			greyCandyTexture, retryTexture, homeTexture,
			nextTexture, &pauseTitleFont, &digipenFont
		});
	isGameOver = 0;

	for (int i{}; i < 4; ++i) {
		GameObject* obj = &backgroundObjs[i];
		obj->SetMesh(squareMesh);
		obj->SetTexture(backgrounds[i]);
		obj->UpdateScale(AEVec2{ 1700.f, 960.f });
		obj->UpdateRotation(0.f);
		obj->UpdatePosition(AEVec2{ 0.f, 0.f });
	}

	gamelives.SetTexture(candyTexture);
	gamelives.SetMesh(squareMesh);
	gamelives.UpdateScale(AEVec2{ 50.f, 50.f });
	gamelives.UpdatePosition(AEVec2{ -750.f,-415.f });

	for (GameObject* obj : grid.objs) {
		//obj->Draw();
		if (obj->_tag == GameObjectTag::GravUp ||
			obj->_tag == GameObjectTag::GravDown ||
			obj->_tag == GameObjectTag::GravLeft ||
			obj->_tag == GameObjectTag::Gravright)
			gravityEffect.ResizeGrav();
	}
}
void Level5_Update() {
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	if (isGameOver) {
		if (input.isKeyTriggered(AEVK_R))
			RestartLevel(UpdateGameManagerState, Level5);
	}

	if (pauseMenu) //Checks if pauseMenu Exists
	{
		if (AEInputCheckTriggered(AEVK_ESCAPE) && !isGameOver)
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
			bgm.PauseAudio();
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

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetBackgroundColor(0.9f, 0.85f, 0.65f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(1.f);

	if (!isGamePaused && !isGameOver) {
		bgm.ResumeAudio();
		levelTime += static_cast<f32>(AEFrameRateControllerGetFrameTime());
	}

	if (isGamePaused || isGameOver) return;

	for (GameObject* obj : grid.objs) {
		if (obj->_tag == GameObjectTag::GravUp ||
			obj->_tag == GameObjectTag::GravDown ||
			obj->_tag == GameObjectTag::GravLeft ||
			obj->_tag == GameObjectTag::Gravright)
			gravityEffect.Update(dt, obj->GetPosition(), obj->_tag);
	}

	//PLAYER ITEMS//
	mainPlayer.PlayerMovement(dt, mainPlayer.grav, grid);
	if (mainPlayer.GetPlayerJump()) {
		mainPlayer.SetTexture(playerTexture[1]);
	}
	//else {
	//	mainPlayer.SetTexture(playerTexture[0]);
	//}
	mainPlayer.p_isGrounded = false;
	mainPlayer.hasCollided = false;
	mainPlayer.canMoveLeft = true;
	mainPlayer.canMoveRight = true;
	mainPlayer.CheckBinaryCollision(&grid, mainPlayer.grav);
	mainPlayer.PlayerCollision(grid.objs);

	camEffect.Update(dt, mainPlayer.grav,mainPlayer.isHit);
	camEffect.Apply();

	mainPlayer.audioPlayed = mainPlayer.hasCollided;

	playerEffect.Update(dt, mainPlayer.GetPosition());
	playerEffect.ResizePlayer(mainPlayer.isSprint);

	if (input.isKeyPressed(AEVK_LCTRL) && input.isKeyTriggered(AEVK_E))
		ToggleEditorMode();

	if (IsEditorActive())
		EditorUpdate(&grid, Level5, savePath);
}

void Level5_Draw() {
	Level5_Parallax();
	for (GameObject* obj : grid.objs) {
		//obj->Draw();
		if (obj->_tag == GameObjectTag::GravUp ||
			obj->_tag == GameObjectTag::GravDown ||
			obj->_tag == GameObjectTag::GravLeft ||
			obj->_tag == GameObjectTag::Gravright)
			gravityEffect.DrawParticle();
	}
	grid.Draw();
	mainPlayer.Draw();
	playerEffect.DrawParticle();

	digipenFont.Change_Text_Color(Colors::White);
	digipenFont.Input_Text("x");
	digipenFont.Draw_Text(AEVec2{ -720.f, -420.f }, .5f, 1.f);
	digipenFont.Input_Float((float)mainPlayer.GetPlayerHealth());
	digipenFont.Draw_Text(AEVec2{ -700.f, -420.f }, .5f, 1.f);

	gamelives.Draw();

	if (pauseMenu) pauseMenu->Draw();
	if (gameOverMenu) gameOverMenu->Draw();

	if (pauseMenu->IsActive() || gameOverMenu->IsActive()) return;
	if (IsEditorActive())
		EditorDraw(digipenFont, grid, mainPlayer);
}

void Level5_Free() {
	PRINT("Free: %s", "Level 5\n");
	playerEffect.Clear();
	gravityEffect.Clear();
	camEffect.Clear();
	bgm.StopAudio();
}

void Level5_Unload() {
	PRINT("Unload: %s", "Level 5\n");
	pauseMenu.reset();
	gameOverMenu.reset();
	mainPlayer.newAnim.UnloadFrames();
	//pauseUI.Exit();
	if (isGamePaused) isGamePaused = 0;
	if (isGameOver) isGameOver = 0;
	if (mainPlayer.getGameOver()) mainPlayer.SetGameOver(false);
	if (memoryManager.MassUnload()) PRINT("Unload Success");
	grid.Clear();
}

void Level5_Parallax() {
	//Reduce Colour -> To Bright
	AEGfxSetColorToMultiply(.75f, .75f, .75f, 1.0f);
	f32 offsetX = mainPlayer.GetPosition().x;
	f32 offsetY = mainPlayer.GetPosition().y;

	f32 offsetFirstX = offsetX / 200.f;
	f32 offsetSecondX = offsetX / 75.f;
	f32 offsetThirdX = offsetX / 25.f;

	f32 offsetFirstY = offsetY / 237.f;
	f32 offsetSecondY = offsetY / 112.f;
	f32 offsetThirdY = offsetY / 62.f;

	backgroundObjs[1].UpdatePosition(AEVec2{ -offsetFirstX + camEffect.GetBgOffset().x * 0.20f, -offsetFirstY + camEffect.GetBgOffset().y * 0.35f });
	backgroundObjs[2].UpdatePosition(AEVec2{ -offsetSecondX + camEffect.GetBgOffset().x * 0.35f, -offsetSecondY + camEffect.GetBgOffset().y * 0.55f });
	backgroundObjs[3].UpdatePosition(AEVec2{ -offsetThirdX + camEffect.GetBgOffset().x * 0.50f, -offsetThirdY + camEffect.GetBgOffset().y * 0.80f });
	for (GameObject& obj : backgroundObjs) {
		obj.Draw();
	}
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
}

void Level5_UIDraw() {
	digipenFont.Input_Text("Time: ");
	digipenFont.Draw_Text(675.f, 400.f);
	digipenFont.Input_Float(levelTime);
	digipenFont.Draw_Text_Corner(725.f, 387.f);

	digipenFont.Input_Text("Level 5");
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);
}