#pragma once
#ifndef _Level_Menu_Header_
#define _Level_Menu_Header_

#include <iostream>
#include <AEEngine.h>
#include "../header/GameState.hpp"

class LevelMenuState : public GameState {
public:

	void LoadState() override;
	void InitializeState() override;
	void UpdateState() override;
	void StateDraw() override;
	void FreeState() override;
	void UnloadState() override;

};

#endif // !_Main_Menu_Header_
