#pragma once
#ifndef _Level1_Header_
#define _Level1_Header_

#include <iostream>
#include <AEEngine.h>
#include "../header/GameState.hpp"

class Level1State : public GameState {
public:

	void LoadState() override;
	void InitializeState() override;
	void UpdateState() override;
	void StateDraw() override;
	void FreeState() override;
	void UnloadState() override;

};

#endif // !_Level1_Header_
