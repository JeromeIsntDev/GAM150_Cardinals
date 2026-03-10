/*!***********************************************************************
\file   InputHandler.hpp
\author Breanne Chua
\date    16/01/2026
\par    Last Updated by: Breanne Chua
\par    Project: Cardinals

\brief  InputHandler is responsible for ALL inputs. Uses a singleton instance.

\par    Changes: 24/1/26
**************************************************************************/

#pragma once
#ifndef _Input_Handler_Header_
#define _Input_Handler_Header_

#include <AEEngine.h>

class InputHandler {
private:
	InputHandler() = default;

public:
	// is this a singleton!? yes it is and im gonna kill someone
	// clearing shit up: singleton is for manager, polymorph is for my cutesy elements
	static InputHandler& GetInstance()
	{
		static InputHandler instance; // created ONLY the first time this is called
		return instance; // returns the same obj lalala
	}

	// so we only have ONE input manager ok. okay . maybe if i have time
	// i'll do this for other things too

	InputHandler(const InputHandler&) = delete; // just makes sure theres only ONE manager
	void operator=(const InputHandler&) = delete; // assignment operator, literally tells you auto input = instance is nuh uh

	// PSA!! if u wanna access the handler do:
	// auto& myInput = InputHandler::GetInstance();
	// okayyy? :3

	bool isKeyPressed(u8 key); // held
	bool isKeyTriggered(u8 key); // jst once

	bool isKeyReleased(u8 key);

	void GetMouseScreenPosition(float& x, float& y);
	void GetMouseWorldPosition(float& x, float& y);
};

#endif