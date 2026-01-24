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

bool InputHandler::isKeyPressed(u32 key) 
{
	return AEInputCheckCurr(key);
}

bool InputHandler::isKeyTriggered(u32 key)
{
	return AEInputCheckTriggered(key);
}

bool InputHandler::isMouseBtnPressed(u32 button) 
{
	return AEInputCheckCurr(button);
}

bool InputHandler::isMouseBtnTriggered(u32 button)
{
	return AEInputCheckTriggered(button);
}

void InputHandler::getMousePos(float& x, float& y)
{
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	x = static_cast<float>(mouseX);
	y = static_cast<float>(mouseY);
}