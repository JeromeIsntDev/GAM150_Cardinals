#include "GameEditor.hpp"

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
	if (!canEdit) return;
	restart(state);
}

bool IsEditorActive()
{
	return canEdit;
}

bool IsUsingSaveFile() {
	return useSaveFile;
}

int& const EditorTileTag()
{
	return editor_tileTag;
}

int& const EditorTileSlice()
{
	return editor_tileSlice;
}

void ChangeTileTag()
{
	if (InputHandler::GetInstance().isKeyTriggered(AEVK_LEFT) && editor_tileTag > 0) {

		--editor_tileTag;
	}
	else if (InputHandler::GetInstance().isKeyTriggered(AEVK_RIGHT)) {
		++editor_tileTag;
	}
	editor_tileTag = static_cast<int>(AEClamp(static_cast<f32>(editor_tileTag), 0.f,
		static_cast<f32>(GameObjectTag::Obstacle)));
}

void ChangeTileSlice()
{
	if (InputHandler::GetInstance().isKeyTriggered(AEVK_DOWN)) {

		++editor_tileSlice;
	}
	else if (InputHandler::GetInstance().isKeyTriggered(AEVK_UP) && editor_tileSlice > 0) {
		--editor_tileSlice;
	}

	editor_tileSlice = static_cast<int>(AEClamp(static_cast<f32>(editor_tileSlice), 0.f, 8.f));
}
