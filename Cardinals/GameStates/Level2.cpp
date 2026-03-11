#include "Level2.hpp"
#include "GameEditor.hpp"

void Level2_EditorUpdate();
void Level2_EditorDraw();
void Level2_Parallax();

extern bool isGamePaused;
extern bool isGameOver;

namespace {
	AEGfxVertexList* squareMesh;
	Text digipenFont, pauseTitleFont;

	
	//PLAYER VARIABLES
	f32 playerMovespeed{ 300.f };
	int  playerHealth{ 3 };
	Texture *playerTexture[2]{};
	ParticleEffect playerEffect;
	ParticleEffect gravityEffect;

	TextureManager& tex = TextureManager::GetInstance();

	Texture *placeholderTexture;
	Texture verticalPlatformTex;
	Texture horizontalPlatformTex;

	auto& input = InputHandler::GetInstance();

	//UIManager pauseUI;
	//std::unique_ptr<Button> conBtn, quitBtn;

	std::unique_ptr <Overlay> pauseMenu;
	std::unique_ptr<Overlay> gameOverMenu;
	Texture *barTexture{}, *knobMasterTexture{}, *knobMusicTexture{},
        *knobEFXTexture{}, *cardBgTexture{}, *prevTexture{},
        *restartTexture{}, *resumeTexture{}, *homeTexture{},
        *nextTexture{}, *retryTexture{}, *candyTexture{},
        *greyCandyTexture{};

	f32 levelTime{};

	GameObject gamelives[3]{};

	Grid grid;
	Player mainPlayer;
}

void Level2_Load() {
	PRINT("Load: %s", "Level 2\n");
	
	squareMesh = RectMeshCenter();
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 2");
	digipenFont.Change_Text_Color(Colors::Black);

	pauseTitleFont.Load_Font(Fonts::Alt, 72.f);

	if (!memoryManager.textures.empty()) memoryManager.textures.clear();

	/*memoryManager.AddTextures({
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
	}*/
	if (!memoryManager.textures.empty()) memoryManager.textures.clear();
	memoryManager.AddMesh(&squareMesh);
	//memoryManager.AddMesh(&tileMesh);

	if (!memoryManager.fonts.empty()) memoryManager.fonts.clear();
	memoryManager.AddFont(&digipenFont);
	memoryManager.AddFont(&pauseTitleFont);

	std::vector<GameObject> objData = ReadJsonToObjects("../../Saves/Level2_GameObjects.json");

	grid.Load(tex.Get("TileSheet"), 900, 900, 300, 300);
	grid.Initialize();
	for (int i{}; i < objData.size(); ++i) {
		objData[i].SetMesh(grid.tileMesh);
		objData[i].SetTexture(&grid.tileTexture);

		AEVec2& pos = objData[i].GetPosition();
		objData[i].UpdatePosition(pos);
		u32 tag = static_cast<u32>(objData[i]._tag);
		grid.SpawnNewTile(tag, objData[i].GetSlice(), pos.x + 800.f, 450.f - pos.y);
	}
}

void Level2_Initialize()
{
	PRINT("Initialize: %s", "Level 2\n");

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
	//rectMesh = RectMeshCenter();

	//placeholderObj.SetMesh(rectMesh);
	//placeholderObj.SetTexture(&placeholder);
	//placeholderObj.UpdateScale(AEVec2{ 1600.f, 900.f });
	//placeholderObj.UpdateRotation(0.f);

	mainPlayer.InitializePlayer(playerHealth, playerMovespeed, false,
		0.f, 0.f, squareMesh, AEGfxGetWinMinX() + 225.f, -25.f, false);
	mainPlayer.SetMesh(squareMesh);
	mainPlayer.SetTexture(playerTexture[0]);
	mainPlayer.UpdateScale(mainPlayer.p_Scale);
	mainPlayer.UpdateRotation(0.f);
	mainPlayer.UpdatePosition(mainPlayer.GetPosition());
	mainPlayer._tag = GameObjectTag::Player;
	mainPlayer.SetHealth(3);

	levelTime = 0.f;

	pauseMenu = std::make_unique<Overlay>(squareMesh, PAUSE, -1,
		[]() {isGamePaused = 0; },
		[]() {isGamePaused = 0; UpdateGameManagerState(GameStateEnum::MainMenu); },
		[]() {isGamePaused = 0; UpdateGameManagerState(GameStateEnum::Level2); });

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
		LEVEL, 2,
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Level3); },
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::MainMenu); },
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Level2); });

	gameOverMenu->SetTextures(
		GameOverTextures{
			placeholderTexture, cardBgTexture, candyTexture,
			greyCandyTexture, retryTexture, homeTexture,
			nextTexture, &pauseTitleFont, &digipenFont
		});
	isGameOver = 0;

	for (int i{}; i < 3; ++i) {
		GameObject* live = &gamelives[i];
		live->SetTexture(candyTexture);
		live->SetMesh(squareMesh);
		live->UpdateScale(AEVec2{ 70.f, 70.f });
		live->UpdatePosition(AEVec2{ -70.f + i * 70.f, 380.f });
	}
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

	if (!IsEditorActive() || IsEditorActive())
		if (input.isKeyTriggered(AEVK_E))
			ToggleEditorMode();

	Level2_EditorUpdate();

	if (isGamePaused || isGameOver) return;

	//if (input.isMouseBtnTriggered(AEVK_P)) {
	//	isGamePaused = ~isGamePaused;
	//}

	
	//if (!isGamePaused) return;
	//pauseUI.Update(0.f);

	for (GameObject* obj : grid.objs) {
		if (obj->_tag == GameObjectTag::GravUp ||
			obj->_tag == GameObjectTag::GravDown ||
			obj->_tag == GameObjectTag::GravLeft ||
			obj->_tag == GameObjectTag::Gravright)
			gravityEffect.Update(dt, obj->GetPosition(), obj->_tag);
	}

	for (int i{}; i < 3; ++i) {
		GameObject* live = &gamelives[i];
		if (i < mainPlayer.GetPlayerHealth()) {
			live->SetTexture(candyTexture);
		}
		else {
			live->SetTexture(greyCandyTexture);
		}
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
	mainPlayer.PlayerCollision(grid.objs, mainPlayer.grav);

	playerEffect.Update(dt, mainPlayer.GetPosition());

}

void Level2_Draw() {
	//playerEffect.DrawParticle();

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

	digipenFont.Input_Text("Time: ");
	digipenFont.Draw_Text(675.f, 400.f);
	digipenFont.Input_Float(levelTime);
	digipenFont.Draw_Text_Corner(725.f, 387.f);

	digipenFont.Input_Text("Level 2");
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);

	for (GameObject& live : gamelives) {
		live.Draw();
	}

	if (pauseMenu) pauseMenu->Draw();
	if (gameOverMenu) gameOverMenu->Draw();

	Level2_EditorDraw();

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
	grid.Clear();
}

void Level2_EditorUpdate() {
	if (IsEditorActive() && !isGamePaused && !isGameOver) {

		//DragObject(levelObjList);
		ChangeTileTag();
		ChangeTileSlice();

		if (input.isKeyTriggered(AEVK_R))
			RestartLevel(UpdateGameManagerState, Level2);

		if (input.isKeyPressed(AEVK_LCTRL) && input.isKeyTriggered(AEVK_S))
			SaveGameObjectToFilepath("../../Saves/Level2_GameObjects.json", grid.objs);

		if (input.isKeyTriggered(AEVK_Q))
			ToggleSaveFileMode();

		if (InputHandler::GetInstance().isKeyTriggered(AEVK_LBUTTON))
		{
			AEVec2 mousePos{ 0.f,0.f };
			InputHandler::GetInstance().GetMouseScreenPosition(mousePos.x, mousePos.y);
			if (mousePos.x < 0.f || mousePos.x > 1600.f ||
				mousePos.y < 0.f || mousePos.y > 900.f) return;
			grid.SpawnNewTile(EditorTileTag(), EditorTileSlice(), mousePos.x, mousePos.y);
		}

		if (InputHandler::GetInstance().isKeyTriggered(AEVK_RBUTTON))
		{
			AEVec2 mousePos{ 0.f,0.f };
			InputHandler::GetInstance().GetMouseScreenPosition(mousePos.x, mousePos.y);
			if (mousePos.x < 0.f || mousePos.x > 1600.f ||
				mousePos.y < 0.f || mousePos.y > 900.f) return;
			grid.DeleteTile(mousePos.x, mousePos.y);
		}
	}
}


void Level2_EditorDraw() {
	if (pauseMenu->IsActive() || gameOverMenu->IsActive()) return;
	if (IsEditorActive()) {
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
		}
		digipenFont.Draw_Text_Corner(15.f, -430.f);


		if (IsUsingSaveFile()) {
			digipenFont.Input_Text("Using Save File");
			digipenFont.Draw_Text(AEVec2{ -640.f, 380.f }, .5f, 1.f);
		}
		DrawTileOutline();
		mainPlayer.DebugDraw();
		DrawDebugObjects(grid.objs);
	}
}
