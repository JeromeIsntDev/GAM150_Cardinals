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
	InputHandler::GetInstance().getMousePos(mousePos.x, mousePos.y);
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
			if (InputHandler::GetInstance().isMouseBtnPressed(AEVK_LSHIFT)) {
				mx *= 10;
			}
			my = mx;
			obj->UpdateScale(AEVec2{ currentScale.x + mx, currentScale.y + my });
			break;
		}
	}
}

void DrawTileOutline() {
	for (int i{}; i < 18; ++i) {
		lineObject.UpdatePosition({0.f,450.f - 50.f * i});
		lineObject.UpdateScale({1600.f, 1.f});
		lineObject.DebugDraw();
	}
	for (int i{}; i < 32; ++i) {
		lineObject.UpdatePosition({ -800.f + 50.f * i,0.f });
		lineObject.UpdateScale({ 1.f,900.f });
		lineObject.DebugDraw();
	}
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
