/*!***********************************************************************
\file   InputHandler.cpp
\author Breanne Chua
\date   16/01/2026
\par    Last Updated by: Breanne Chua
\par    Project: Cardinals

\brief  InputHandler is responsible for ALL inputs.

\par    Changes: None
**************************************************************************/

#include "../header/InputHandler.hpp"

// errrr yeah

bool InputHandler::isKeyPressed(u8 key) 
{
	return AEInputCheckCurr(key);
}

bool InputHandler::isKeyTriggered(u8 key)
{
	return AEInputCheckTriggered(key);
}

bool InputHandler::isMouseBtnPressed(u8 button)
{
	return AEInputCheckCurr(button);
}

bool InputHandler::isMouseBtnTriggered(u8 button)
{
	return AEInputCheckTriggered(button);
}

bool InputHandler::isKeyReleased(u8 key)
{
	return AEInputCheckReleased(key);
}

void InputHandler::getMousePos(float& x, float& y)
{
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	x = static_cast<float>(mouseX);
	y = static_cast<float>(mouseY);
}