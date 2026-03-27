#include "../GameStates/Level5.hpp"
#include "GameEditor.hpp"

void Level5_EditorUpdate();
void Level5_EditorDraw();
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

	std::vector<GameObject> objData = ReadJsonToObjects("../../Saves/Level5_GameObjects.json");
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

	gameOverMenu = std::make_unique<Overlay>(
		squareMesh,
		LEVEL, 5,
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Credits); },
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::MainMenu); },
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Level5); });

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

	if (!IsEditorActive() || IsEditorActive())
		if (input.isKeyTriggered(AEVK_E))
			ToggleEditorMode();

	Level5_EditorUpdate();
	Level5_Parallax();

	if (isGamePaused || isGameOver) return;

	for (GameObject* obj : grid.objs) {
		if (obj->_tag == GameObjectTag::GravUp ||
			obj->_tag == GameObjectTag::GravDown ||
			obj->_tag == GameObjectTag::GravLeft ||
			obj->_tag == GameObjectTag::Gravright)
			gravityEffect.Update(dt, obj->GetPosition(), obj->_tag);
	}

	//PLAYER ITEMS//
	mainPlayer.PlayerMovement(dt, mainPlayer.grav);
	if (mainPlayer.GetPlayerJump()) {
		mainPlayer.SetTexture(playerTexture[1]);
	}
	else {
		mainPlayer.SetTexture(playerTexture[0]);
	}
	mainPlayer.p_isGrounded = false;
	mainPlayer.hasCollided = false;
	mainPlayer.canMoveLeft = true;
	mainPlayer.canMoveRight = true;
	mainPlayer.CheckBinaryCollision(&grid, mainPlayer.grav);
	mainPlayer.PlayerCollision(grid.objs);
	mainPlayer.audioPlayed = mainPlayer.hasCollided;

	playerEffect.Update(dt, mainPlayer.GetPosition());
	playerEffect.ResizePlayer(mainPlayer.isSprint);
}
void Level5_Draw() {
	AEGfxSetColorToMultiply(.75f, .75f, .75f, 1.0f);
	for (GameObject& obj : backgroundObjs) {
		obj.Draw();
	}
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
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


	Level5_EditorDraw();

}
void Level5_Free() {
	PRINT("Free: %s", "Level 5\n");
	playerEffect.Clear();
	gravityEffect.Clear();
	bgm.StopAudio();
}
void Level5_Unload() {
	PRINT("Unload: %s", "Level 5\n");
	pauseMenu.reset();
	gameOverMenu.reset();
	//pauseUI.Exit();
	if (isGamePaused) isGamePaused = 0;
	if (isGameOver) isGameOver = 0;
	if (mainPlayer.getGameOver()) mainPlayer.SetGameOver(false);
	if (memoryManager.MassUnload()) PRINT("Unload Success");
	grid.Clear();
}


void Level5_EditorUpdate() {
	if (IsEditorActive() && !isGamePaused && !isGameOver) {

		//DragObject(levelObjList);
		ChangeTileTag();
		ChangeTileSlice();

		if (input.isKeyTriggered(AEVK_R))
			RestartLevel(UpdateGameManagerState, Level5);

		if (input.isKeyPressed(AEVK_LCTRL) && input.isKeyTriggered(AEVK_S))
			SaveGameObjectToFilepath("../../Saves/Level5_GameObjects.json", grid.objs);

		if (input.isKeyTriggered(AEVK_Q))
			ToggleSaveFileMode();

		s32 mwDelta{};
		AEInputMouseWheelDelta(&mwDelta);

		if (InputHandler::GetInstance().isKeyTriggered(AEVK_LBUTTON))
		{
			AEVec2 mousePos{ 0.f,0.f };
			InputHandler::GetInstance().GetMouseScreenPosition(mousePos.x, mousePos.y);
			if (mousePos.x < 0.f || mousePos.x > 1600.f ||
				mousePos.y < 0.f || mousePos.y > 900.f) return;
			grid.SpawnNewTile(EditorTileTag(), EditorTileSlice(), mousePos.x, mousePos.y, 0.f);
		}
		else if (InputHandler::GetInstance().isKeyTriggered(AEVK_RBUTTON))
		{
			AEVec2 mousePos{ 0.f,0.f };
			InputHandler::GetInstance().GetMouseScreenPosition(mousePos.x, mousePos.y);
			if (mousePos.x < 0.f || mousePos.x > 1600.f ||
				mousePos.y < 0.f || mousePos.y > 900.f) return;
			grid.DeleteTile(mousePos.x, mousePos.y);
		}
		else if (abs(mwDelta) > 0) {
			AEVec2 mousePos{ 0.f,0.f };
			InputHandler::GetInstance().GetMouseScreenPosition(mousePos.x, mousePos.y);
			if (mousePos.x < 0.f || mousePos.x > 1600.f ||
				mousePos.y < 0.f || mousePos.y > 900.f) return;
			grid.RotateTile(mousePos.x, mousePos.y, mwDelta);
		}
	}
}
void Level5_EditorDraw() {
	if (pauseMenu->IsActive() || gameOverMenu->IsActive()) return;
	if (IsEditorActive()) {
		if (IsUsingSaveFile()) {
			digipenFont.Input_Text("Using Save File");
			digipenFont.Draw_Text(AEVec2{ -640.f, 380.f }, .5f, 1.f);
		}
		DrawTileOutline();
		mainPlayer.DebugDraw();
		DrawDebugObjects(grid.objs);

		digipenFont.Change_Text_Color(Colors::Black);
		digipenFont.Input_Text("Editor Active");
		digipenFont.Draw_Text(AEVec2{ -650.f, 415.f }, .5f, 1.f);


		digipenFont.Input_Text("Tile Tag: ");
		digipenFont.Draw_Text(AEVec2{ -100.f, -365.f }, .5f, 1.f);
		switch (EditorTileTag()) {
		case 0:
			digipenFont.Input_Text("Static");
			break;
		case 1:
			digipenFont.Input_Text("Dynamic");
			break;
		case 2:
			digipenFont.Input_Text("Platform");
			break;
		case 3:
			digipenFont.Input_Text("Player");
			break;
		case 4:
			digipenFont.Input_Text("Win");
			break;
		case 5:
			digipenFont.Input_Text("GravUp");
			break;
		case 6:
			digipenFont.Input_Text("GravDown");
			break;
		case 7:
			digipenFont.Input_Text("GravLeft");
			break;
		case 8:
			digipenFont.Input_Text("GravRight");
			break;
		case 9:
			digipenFont.Input_Text("Obstacle");
			break;
		}
		digipenFont.Draw_Text(AEVec2{ 75.f, -365.f }, .5f, 1.f);

		digipenFont.Input_Text("Tile Slice: ");
		digipenFont.Draw_Text(AEVec2{ -100.f, -415.f }, .5f, 1.f);
		switch (EditorTileSlice()) {
		case 0:
			digipenFont.Input_Text("Top-left");
			break;
		case 1:
			digipenFont.Input_Text("Top-Middle");
			break;
		case 2:
			digipenFont.Input_Text("Top-Right");
			break;
		case 3:
			digipenFont.Input_Text("Middle-Left");
			break;
		case 4:
			digipenFont.Input_Text("Middle-Middle");
			break;
		case 5:
			digipenFont.Input_Text("Middle-Right");
			break;
		case 6:
			digipenFont.Input_Text("Bottom-Left");
			break;
		case 7:
			digipenFont.Input_Text("Bottom-Middle");
			break;
		case 8:
			digipenFont.Input_Text("Bottom-Right");
			break;
		case 9:
			digipenFont.Input_Text("Connector 1");
			break;
		case 10:
			digipenFont.Input_Text("Connector 2");
			break;
		case 11:
			digipenFont.Input_Text("Connector 3");
			break;
		}
		digipenFont.Draw_Text_Corner(15.f, -430.f);
		digipenFont.Change_Text_Color(Colors::White);
	}
}
void Level5_Parallax() {
	AEGfxSetColorToMultiply(.75f, .75f, .75f, 1.0f);
	f32 offset = mainPlayer.GetPosition().x;
	f32 offsetFirst = offset / 200.f;
	f32 offsetSecond = offset / 75.f;
	f32 offsetThird = offset / 25.f;

	backgroundObjs[1].UpdatePosition(AEVec2{ -offsetFirst, backgroundObjs[1].GetPosition().y });
	backgroundObjs[2].UpdatePosition(AEVec2{ -offsetSecond, backgroundObjs[1].GetPosition().y });
	backgroundObjs[3].UpdatePosition(AEVec2{ -offsetThird, backgroundObjs[1].GetPosition().y });
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