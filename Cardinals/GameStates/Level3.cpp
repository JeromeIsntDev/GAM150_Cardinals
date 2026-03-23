#include "../GameStates/Level3.hpp"
#include "GameEditor.hpp"

void Level3_EditorUpdate();
void Level3_EditorDraw();
void Level3_Parallax();
void Level3_Tutorial();

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

	GameObject gamelives[3]{};

	Texture* backgrounds[4]{};
	GameObject backgroundObjs[4]{};

	Grid grid;
	Player mainPlayer;

	int checkPoints = 0;
}

void Level3_Load() {
	PRINT("Load: %s", "Level 3\n");

	squareMesh = RectMeshCenter();
	digipenFont.Load_Font(Fonts::Alt, 72.f);
	digipenFont.Input_Text("Level 3");
	digipenFont.Change_Text_Color(Colors::Black);

	pauseTitleFont.Load_Font(Fonts::Alt, 72.f);

	if (!memoryManager.textures.empty()) memoryManager.textures.clear();

	if (!memoryManager.textures.empty()) memoryManager.textures.clear();
	memoryManager.AddMesh(&squareMesh);

	if (!memoryManager.fonts.empty()) memoryManager.fonts.clear();
	memoryManager.AddFont(&digipenFont);
	memoryManager.AddFont(&pauseTitleFont);

	std::vector<GameObject> objData = ReadJsonToObjects("../../Saves/Level3_GameObjects.json");
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

void Level3_Initialize()
{
	checkPoints = 0;
	PRINT("Initialize: %s", "Level 3\n");
	bgm.PlayAudio("GAMEPLAY", AudioType::MUSIC, 1.f, -1);
	bgm.SetVolume(AudioType::MUSIC, 0.8f);

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
		[]() {isGamePaused = 0; UpdateGameManagerState(GameStateEnum::Level3); });

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
		LEVEL, 3,
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Level4); },
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::MainMenu); },
		[]() {isGameOver = 0; UpdateGameManagerState(GameStateEnum::Level3); });

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

	for (int i{}; i < 3; ++i) {
		GameObject* live = &gamelives[i];
		live->SetTexture(candyTexture);
		live->SetMesh(squareMesh);
		live->UpdateScale(AEVec2{ 70.f, 70.f });
		live->UpdatePosition(AEVec2{ -70.f + i * 70.f, 380.f });
	}

	for (GameObject* obj : grid.objs) {
		//obj->Draw();
		if (obj->_tag == GameObjectTag::GravUp ||
			obj->_tag == GameObjectTag::GravDown ||
			obj->_tag == GameObjectTag::GravLeft ||
			obj->_tag == GameObjectTag::Gravright)
			gravityEffect.ResizeGrav();
	}
}

void Level3_Update() {
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	if (pauseMenu) //Checks if pauseMenu Exists
	{
		if (AEInputCheckTriggered(AEVK_P) && !isGameOver)
		{
			bgm.PauseAudio();
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

	Level3_EditorUpdate();
	Level3_Parallax();

	if (isGamePaused || isGameOver) return;

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
	mainPlayer.PlayerCollision(grid.objs);
	mainPlayer.audioPlayed = mainPlayer.hasCollided;

	playerEffect.Update(dt, mainPlayer.GetPosition());
	playerEffect.ResizePlayer(mainPlayer.isSprint);
}

void Level3_Draw() {

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

	digipenFont.Input_Text("Time: ");
	digipenFont.Draw_Text(675.f, 400.f);
	digipenFont.Input_Float(levelTime);
	digipenFont.Draw_Text_Corner(725.f, 387.f);

	digipenFont.Input_Text("Level 3");
	digipenFont.Draw_Text(AEVec2{ -700.f, -430.f }, .5f, 1.f);

	for (GameObject& live : gamelives) {
		live.Draw();
	}

	if (pauseMenu) pauseMenu->Draw();
	if (gameOverMenu) gameOverMenu->Draw();

	Level3_Tutorial();

	Level3_EditorDraw();

	//PRINT("Draw: %s", "Level 3\n");
}

void Level3_Free() {
	PRINT("Free: %s", "Level 3\n");
	playerEffect.Clear();
	gravityEffect.Clear();
	bgm.StopAudio();
}

void Level3_Unload() {
	PRINT("Unload: %s", "Level 3\n");
	pauseMenu.reset();
	gameOverMenu.reset();
	//pauseUI.Exit();
	if (isGamePaused) isGamePaused = 0;
	if (isGameOver) isGameOver = 0;
	if (mainPlayer.getGameOver()) mainPlayer.SetGameOver(false);
	if (memoryManager.MassUnload()) PRINT("Unload Success");
	grid.Clear();
}

void Level3_Parallax() {
	//Reduce Colour -> To Bright
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

void Level3_EditorUpdate() {
	if (IsEditorActive() && !isGamePaused && !isGameOver) {

		//DragObject(levelObjList);
		ChangeTileTag();
		ChangeTileSlice();

		if (input.isKeyTriggered(AEVK_R))
			RestartLevel(UpdateGameManagerState, Level3);

		if (input.isKeyPressed(AEVK_LCTRL) && input.isKeyTriggered(AEVK_S))
			SaveGameObjectToFilepath("../../Saves/Level3_GameObjects.json", grid.objs);

		if (input.isKeyTriggered(AEVK_Q))
			ToggleSaveFileMode();

		if (InputHandler::GetInstance().isKeyTriggered(AEVK_LBUTTON))
		{
			AEVec2 mousePos{ 0.f,0.f };
			InputHandler::GetInstance().GetMouseScreenPosition(mousePos.x, mousePos.y);
			if (mousePos.x < 0.f || mousePos.x > 1600.f ||
				mousePos.y < 0.f || mousePos.y > 900.f) return;
			grid.SpawnNewTile(EditorTileTag(), EditorTileSlice(), mousePos.x, mousePos.y, 0.f);
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

void Level3_EditorDraw() {
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


		if (IsUsingSaveFile()) {
			digipenFont.Input_Text("Using Save File");
			digipenFont.Draw_Text(AEVec2{ -640.f, 380.f }, .5f, 1.f);
		}
		DrawTileOutline();
		mainPlayer.DebugDraw();
		DrawDebugObjects(grid.objs);
	}
}

void Level3_Tutorial() {
	if (checkPoints > 0) return;
	digipenFont.Change_Text_Color(Colors::White);
	if (mainPlayer.GetPosition().y > 0.f) ++checkPoints;

	digipenFont.Input_Text("Use 'W' and 'S' to move Vertically!");
	digipenFont.Draw_Text(-200.f, -375.f);
	digipenFont.Change_Text_Color(Colors::White);
}