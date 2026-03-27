#include "GameEditor.hpp"

void EditorUpdate(Grid* grid, GameStateEnum state, std::string const& savePath)
{
	InputHandler& input = InputHandler::GetInstance();

	ChangeTileTag();
	ChangeTileSlice();

	if (input.isKeyTriggered(AEVK_R))
		RestartLevel(UpdateGameManagerState, state);

	if (input.isKeyPressed(AEVK_LCTRL) && input.isKeyTriggered(AEVK_S))
		SaveGameObjectToFilepath(savePath, grid->objs);

	if (input.isKeyTriggered(AEVK_Q))
		ToggleSaveFileMode();

	s32 mwDelta{};
	AEInputMouseWheelDelta(&mwDelta);

	if (InputHandler::GetInstance().isKeyTriggered(AEVK_LBUTTON))
	{
		AEVec2 mousePos{ 0.f,0.f };
		EditorGetMouse(mousePos.x, mousePos.y);
		grid->SpawnNewTile(EditorTileTag(), EditorTileSlice(), mousePos.x, mousePos.y, 0.f);
	}
	else if (InputHandler::GetInstance().isKeyTriggered(AEVK_RBUTTON))
	{
		AEVec2 mousePos{ 0.f,0.f };
		EditorGetMouse(mousePos.x, mousePos.y);
		grid->DeleteTile(mousePos.x, mousePos.y);
	}
	else if (abs(mwDelta) > 0) {
		AEVec2 mousePos{ 0.f,0.f };
		EditorGetMouse(mousePos.x, mousePos.y);
		grid->RotateTile(mousePos.x, mousePos.y, mwDelta);
	}
}

void EditorDraw(Text& font, Grid& grid, Player& player){
	font.Change_Text_Color(Colors::Black);

	if (IsUsingSaveFile()) {
		font.Input_Text("Using Save File");
		font.Draw_Text(AEVec2{ -640.f, 380.f }, .5f, 1.f);
	}
	DrawTileOutline();
	player.DebugDraw();
	DrawDebugObjects(grid.objs);

	font.Input_Text("Editor Active");
	font.Draw_Text(AEVec2{ -650.f, 415.f }, .5f, 1.f);

	font.Input_Text("Tile Tag: ");
	font.Draw_Text(AEVec2{ -100.f, -365.f }, .5f, 1.f);
	switch (EditorTileTag()) {
	case 0:
		font.Input_Text("Static");
		break;
	case 1:
		font.Input_Text("Dynamic");
		break;
	case 2:
		font.Input_Text("Platform");
		break;
	case 3:
		font.Input_Text("Player");
		break;
	case 4:
		font.Input_Text("Win");
		break;
	case 5:
		font.Input_Text("GravUp");
		break;
	case 6:
		font.Input_Text("GravDown");
		break;
	case 7:
		font.Input_Text("GravLeft");
		break;
	case 8:
		font.Input_Text("GravRight");
		break;
	case 9:
		font.Input_Text("Obstacle");
		break;
	case 10:
		font.Input_Text("Special");
		break;
	}
	font.Draw_Text(AEVec2{ 75.f, -365.f }, .5f, 1.f);

	font.Input_Text("Tile Slice: ");
	font.Draw_Text(AEVec2{ -100.f, -415.f }, .5f, 1.f);
	switch (EditorTileSlice()) {
	case 0:
		font.Input_Text("Top-left");
		break;
	case 1:
		font.Input_Text("Top-Middle");
		break;
	case 2:
		font.Input_Text("Top-Right");
		break;
	case 3:
		font.Input_Text("Middle-Left");
		break;
	case 4:
		font.Input_Text("Middle-Middle");
		break;
	case 5:
		font.Input_Text("Middle-Right");
		break;
	case 6:
		font.Input_Text("Bottom-Left");
		break;
	case 7:
		font.Input_Text("Bottom-Middle");
		break;
	case 8:
		font.Input_Text("Bottom-Right");
		break;
	case 9:
		font.Input_Text("Connector 1");
		break;
	case 10:
		font.Input_Text("Connector 2");
		break;
	case 11:
		font.Input_Text("Connector 3");
		break;
	}
	font.Draw_Text_Corner(15.f, -430.f);
	font.Change_Text_Color(Colors::White);
}

void EditorGetMouse(f32& x, f32& y) {
	InputHandler::GetInstance().GetMouseScreenPosition(x, y);
	x = x < 0.f ? 0.f : (x > 1600.f ? 1600.f : x);
	y = y < 0.f ? 0.f : (y > 900.f ? 900.f : y);
}

void InitializeEditor()
{
	lineObject.SetMesh(lineMesh = RectMeshCenter());
}

void ClearEditorMemory()
{
	AEGfxMeshFree(lineMesh);
}

void DragObject(std::vector<GameObject*> objs)
{
	if (!canEdit) return;
	AEVec2 mousePos{ 0.f,0.f };
	InputHandler::GetInstance().GetMouseScreenPosition(mousePos.x, mousePos.y);
	mousePos.x += AEGfxGetWinMaxX() - AEGfxGetWindowWidth();
	mousePos.y += AEGfxGetWinMaxY() - AEGfxGetWindowHeight();
	for (GameObject* obj : objs) {
		AEVec2 currentScale = obj->GetScale();
		AEVec2 currentPos = obj->GetPosition();

		float halfW = currentScale.x / 2.0f, halfH = currentScale.y / 2.0f;

		if (mousePos.x >= currentPos.x - halfW && mousePos.x <= currentPos.x + halfW &&
			-mousePos.y >= currentPos.y - halfH && -mousePos.y <= currentPos.y + halfH) {
			//PRINT("Object touched: %f, %f", currentPos.x, currentPos.y);

			int mx, my;
			AEInputGetCursorPositionDelta(&mx, &my);
			obj->UpdatePosition(AEVec2{ currentPos.x + mx, currentPos.y - my });

			AEInputMouseWheelDelta(&mx);
			if (InputHandler::GetInstance().isKeyTriggered(AEVK_LSHIFT)) {
				mx *= 10;
			}
			my = mx;
			obj->UpdateScale(AEVec2{ currentScale.x + mx, currentScale.y + my });
			break;
		}
	}
}

void DrawTileOutline() {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetTransparency(1.f);
	AEGfxTextureSet(nullptr, 0.f,0.f);
	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);
	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 1.f);

	for (int i{}; i < 18; ++i) {
		lineObject.UpdatePosition({0.f, tileDrawCenter.y - tileWidth * i});
		lineObject.UpdateScale(horizontalScale);
		lineObject.DebugDraw();
	}
	for (int i{}; i < 32; ++i) {
		lineObject.UpdatePosition({ tileDrawCenter.x + tileWidth * i,0.f });
		lineObject.UpdateScale(verticalScale);
		lineObject.DebugDraw();
	}
	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);
}

void DrawDebugObjects(std::vector<GameObject*> objs) {
	for (GameObject* obj : objs) {
		obj->DebugDraw();
	}
}



void SaveGameObjectToFilepath(std::string filepathSave, std::vector<GameObject*> objs)
{
	if (!canEdit) return;
	std::vector<GameObject*> objData;
	objData.clear();
	for (GameObject* obj : objs) {
		objData.push_back(obj);
	}
	SaveObjectsToJson(objData, filepathSave);
}

void ToggleEditorMode()
{
	canEdit = !canEdit;
}

void ToggleSaveFileMode() {
	useSaveFile = !useSaveFile;
}

void RestartLevel(void(*restart)(GameStateEnum), GameStateEnum state)
{
	restart(state);
}

bool IsEditorActive()
{
	return canEdit;
}

bool IsUsingSaveFile() {
	return useSaveFile;
}

int& EditorTileTag()
{
	return editor_tileTag;
}

int& EditorTileSlice()
{
	return editor_tileSlice;
}

void ChangeTileTag()
{
	if (InputHandler::GetInstance().isKeyTriggered(AEVK_LEFT)) {

		--editor_tileTag;
	}
	else if (InputHandler::GetInstance().isKeyTriggered(AEVK_RIGHT)) {
		++editor_tileTag;
	}
	editor_tileTag = static_cast<int>(AEClamp(static_cast<f32>(editor_tileTag), 0.f,
		static_cast<f32>(GameObjectTag::Special)));
}

void ChangeTileSlice()
{
	if (InputHandler::GetInstance().isKeyTriggered(AEVK_DOWN)) {

		++editor_tileSlice;
	}
	else if (InputHandler::GetInstance().isKeyTriggered(AEVK_UP) && editor_tileSlice > 0) {
		--editor_tileSlice;
	}

	editor_tileSlice = static_cast<int>(AEClamp(static_cast<f32>(editor_tileSlice), 0.f, 11.f));
}
