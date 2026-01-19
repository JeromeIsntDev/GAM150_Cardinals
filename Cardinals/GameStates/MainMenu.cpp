#include "../header/GameState.hpp"
#include "../header/GStateManager.hpp"

class MainMenuState : public GameState {
public:
	MainMenuState(const char* name) {
		_name = name;
		PRINT("%s created.", _name);
	}

	void LoadState() {
		PRINT("Load: %s", _name);
	}

	void InitializeState() {
		PRINT("Initialize: %s", _name);
	}

	void UpdateState() {
		PRINT("Update: %s", _name);
	}

	void StateDraw() {
		PRINT("Draw: %s", _name);
	}

	void FreeState() {
		PRINT("Free: %s", _name);
	}

	void UnloadState() {
		PRINT("Unload: %s", _name);
	}

};