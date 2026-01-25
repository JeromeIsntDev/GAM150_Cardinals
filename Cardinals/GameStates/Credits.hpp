#pragma once
#ifndef _Credits_Header_
#define _Credits_Header_

#include <iostream>
#include <AEEngine.h>
#include "../header/GameState.hpp"

class CreditsState : public GameState {
public:

	void LoadState() override;
	void InitializeState() override;
	void UpdateState() override;
	void StateDraw() override;
	void FreeState() override;
	void UnloadState() override;

};

#endif // !_Credits_Header_
