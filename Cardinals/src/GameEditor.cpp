#include "GameEditor.hpp"

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
			PRINT("Object touched: %f, %f", currentPos.x, currentPos.y);

			int mx, my;
			AEInputGetCursorPositionDelta(&mx, &my);
			obj->UpdatePosition(AEVec2{ currentPos.x + mx, currentPos.y - my });

			AEInputMouseWheelDelta(&mx);
			if (InputHandler::GetInstance().isMouseBtnPressed(AEVK_LSHIFT)) {
				mx *= 10;
			}
			my = mx;
			obj->UpdateScale(AEVec2{ currentScale.x + mx, currentScale.y + my });
		}
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
	//if (!input.isMouseBtnPressed(AEVK_LCTRL)) return;
	/*if (input.isMouseBtnTriggered(AEVK_E)) */canEdit = !canEdit;
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
