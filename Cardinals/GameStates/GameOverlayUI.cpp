/*!***********************************************************************
\file	GameOverlayUI.cpp
\author	Zoey Tan
\date	31/01/2026
\par	Last Updated by: Zoey Tan
\par	Project: Cardinals

\brief	GameOverlayUI is responsible for displaying the overlay menu.

\par	Changes: None
**************************************************************************/

#include "GameOverlayUI.hpp"

VolSlider::VolSlider(AEGfxVertexList* mesh, AudioType type, AEVec2 pos, f32 width)
	: UIElement(mesh, pos, 0.f, AEVec2{ width, 20.f }), // bar mesh
	audioType(type), width(width),
	knob(mesh, AEVec2{ pos.x + (width / 2.f), pos.y }, 0.f, AEVec2{50.f, 50.f})
{
	SetActive(true);
	SetInteractable(true);
	knob.SetActive(true);
}

void VolSlider::SyncKnobToCurrentVolume()
{
	f32 volumePercent = 1.f;
	switch (audioType)
	{
		case AudioType::MUSIC:
			volumePercent = AudioManager::Instance().GetVolume(AudioType::MUSIC);
			break;
		case AudioType::SFX:
			volumePercent = AudioManager::Instance().GetVolume(AudioType::SFX);
			break;
		case AudioType::MASTER:
			volumePercent = AudioManager::Instance().GetVolume(AudioType::MASTER);
			break;
	}
	f32 barStartX = GetPosition().x - (width / 2.f);
	f32 newKnobX = barStartX + (volumePercent * width);
	knob.UpdatePosition(AEVec2{ newKnobX, GetPosition().y });
}

void VolSlider::OnClick()
{
	// start drag on click
	if (!isActive || !isInteractable) return;
	isDragging = true;
}

void VolSlider::Update(f32 dt) 
{
	(void)dt;

	if (!IsActive() || !IsInteractable()) return;

	if (AEInputCheckReleased(AEVK_LBUTTON))
	{
		isDragging = false;
	}

	if (isDragging)
	{
		AEVec2 mousePos = UIManager::GetNewMousePos();

		f32 barStartX = GetPosition().x - (width / 2.f);
		f32 barEndX = GetPosition().x + (width / 2.f);

		// clamp knob position within bar
		f32 newKnobX = AEClamp(mousePos.x, barStartX, barEndX);
		knob.UpdatePosition(AEVec2{ newKnobX, GetPosition().y });

		f32 volumePercent = (newKnobX - barStartX) / width;
		AudioManager::Instance().SetVolume(audioType, volumePercent);
	}
}

void VolSlider::Draw() const
{
	// set color for bar ... igmnore pink texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// pure white with 0 alpha to show texture colour only
	AEGfxSetColorToAdd(1.f, 1.f, 1.f, 0.f);
	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);

	GameObject::Draw(); // draw bar for white

	if (knob.IsActive() && knob.GetTexture() != nullptr) // guard
	{
		AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);
		knob.Draw();
	}
	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f); // reset to default
}

void VolSlider::SetTextures(Texture* barTex, Texture* knobTex)
{
	if (barTex) GameObject::SetTexture(barTex);
	if (knobTex) knob.SetTexture(knobTex);
	SyncKnobToCurrentVolume();
}

///
/// PAUSE MENU
///

Overlay::Overlay(AEGfxVertexList* mesh, GAMEMODE mode, int levelIndex,
	std::function<void()> onResume, std::function<void()> onQuit, std::function<void()> onRestart) 
	: UIElement(mesh, AEVec2{ 0.f, 0.f }, 0.f, AEVec2{ 1600, 900.f }), commonMesh(mesh), _mode(mode), _gameLevel(levelIndex), 
	_onResume(onResume), _onQuit(onQuit), _onRestart(onRestart)
{
	bg.SetMesh(commonMesh);
	bg.UpdateScale(AEVec2{ 1600.f, 900.f });
	bg.UpdatePosition(AEVec2{ 0.f, 0.f });

	cardBG.SetMesh(commonMesh);
	cardBG.UpdateScale(AEVec2{ 800.f, 600.f });
	cardBG.UpdatePosition(AEVec2{ 0.f, 0.f });

	switch (mode) {
		case PAUSE:
			SetupPauseMenu();
			break;

		case SETTINGS:
			SetupSettingsMenu();
			break;

		case LEVEL:
			SetupLevelMenu();
			break;
	}

	SetActive(false);
	SetInteractable(false);
}

void Overlay::SetActive(bool active)
{
	UIElement::SetActive(active);
	isInteractable = active;

	for (auto* child : children)
	{
		if (!child) continue;
		child->SetActive(active);
		child->SetInteractable(active);
	}
}

void Overlay::OnClick()
{
	if (!IsActive() || !IsInteractable()) return;

	AEVec2 mousePos = UIManager::GetNewMousePos();
	for (auto* child : children)
	{
		if (!child) continue;
		if (child->IsActive() && child->IsInteractable() && child->IsPointInside(mousePos.x, mousePos.y))
		{
			if (child == _nextBtn)
			{
				bgm.PlayAudio("NEXT", AudioType::SFX, 1.f, 0);
			} else if (child == _resumeBtn || child == _restartBtn || child == _homeBtn || child == _prevBtn) 
			{
				bgm.PlayAudio("CLICK", AudioType::SFX, 1.f, 0);
			}
			child->OnClick();
			break;
		}
	}
}

void Overlay::Update(f32 dt)
{
	if (!IsActive()) return;

	for (auto* child : children)
	{
		if (!child) continue;
		if (child->IsActive())
		{
			child->ResetTooltip();
			child->Update(dt);
		}
	}
}

void Overlay::Draw() const
{
	if (!IsActive()) return;

	// dimmer
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(0.f, 0.f, 0.f, 0.7f);
	//AEGfxSetTransparency(0.95f);
	AEGfxSetTransform(GetTransform().m);
	AEGfxMeshDraw(commonMesh, AE_GFX_MDM_TRIANGLES);

	// card bg
	if (cardBG.GetTexture() != nullptr) { // guard
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);
		AEGfxSetTransparency(0.9f);
		cardBG.Draw();
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);
	AEGfxSetTransparency(1.f);

	if (candy1.GetTexture()) candy1.Draw();
	if (candy2.GetTexture()) candy2.Draw();
	if (candy3.GetTexture()) candy3.Draw();

	// children
	AEGfxSetTransparency(1.f);
	for (auto* child : children)
	{
		if (!child) continue;
		if (child->IsActive())
		{
			AEGfxSetColorToMultiply(child->GetTint().r, child->GetTint().g, child->GetTint().b, child->GetTint().a);
			child->Draw();
		}
	}

	if (_titleFont) {
		_titleFont->Input_Text(_titleText);
		_titleFont->Change_Text_Color(Colors::White);
		_titleFont->Draw_Text(AEVec2{ 0.f, 220.f },.75f, 1.f);
	}

	if (_mode == LEVEL && _bodyFont) {
		_bodyFont->Input_Text(buffer);
		_bodyFont->Change_Text_Color(Colors::White);
		_bodyFont->Draw_Text(AEVec2{ 0.f, 0.f }, .5f, 1.f);
		//_bodyFont->Change_Text_Color(Colors::Black);
	}

	// reset to default
	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);
	AEGfxSetTransparency(1.f);
}

void Overlay::SetTextures(const PauseMenuTextures& pauseTex)
{
	if (pauseTex.bg) bg.SetTexture(pauseTex.bg);
	if (pauseTex.cardBg) cardBG.SetTexture(pauseTex.cardBg);

	if (_masterSlider) _masterSlider->SetTextures(pauseTex.bar, pauseTex.knobMaster);
	if (_musicSlider)  _musicSlider->SetTextures(pauseTex.bar, pauseTex.knobMusic);
	if (_sfxSlider)    _sfxSlider->SetTextures(pauseTex.bar, pauseTex.knobSFX);
	if (_prevBtn)      _prevBtn->SetTexture(pauseTex.prevBtn);
	if (_resumeBtn)    _resumeBtn->SetTexture(pauseTex.resumeBtn);
	if (_restartBtn)   _restartBtn->SetTexture(pauseTex.restartBtn);
	if (_homeBtn)      _homeBtn->SetTexture(pauseTex.homeBtn);

	if (pauseTex.titleFont) _titleFont = pauseTex.titleFont;
}

void Overlay::SetTextures(const GameOverTextures& gameOverTex)
{
	if (gameOverTex.bg) bg.SetTexture(gameOverTex.bg);
	if (gameOverTex.cardBg) cardBG.SetTexture(gameOverTex.cardBg);
	if (gameOverTex.greyCandy) {
		candy1.SetTexture(gameOverTex.greyCandy);
		candy2.SetTexture(gameOverTex.greyCandy);
		candy3.SetTexture(gameOverTex.greyCandy);
	}
	if (gameOverTex.greyCandy) greyCandy = gameOverTex.greyCandy;
	if (gameOverTex.candy) candy = gameOverTex.candy;

	if (_restartBtn)   _restartBtn->SetTexture(gameOverTex.restartBtn);
	if (_homeBtn)      _homeBtn->SetTexture(gameOverTex.homeBtn);
	if (_nextBtn)      _nextBtn->SetTexture(gameOverTex.nextBtn);

	if (gameOverTex.titleFont) _titleFont = gameOverTex.titleFont;
	if (gameOverTex.bodyFont) _bodyFont = gameOverTex.bodyFont;
}

void Overlay::SetupPauseMenu()
{
	// sliiiders
	auto* masterSlider = new VolSlider(commonMesh, AudioType::MASTER, AEVec2{ 0.f, 100.f }, 500.f);
	_masterSlider = masterSlider;
	AddChild(masterSlider);

	auto* musicSlider = new VolSlider(commonMesh, AudioType::MUSIC, AEVec2{ 0.f, 0.f }, 500.f);
	_musicSlider = musicSlider;
	AddChild(musicSlider);

	auto* sfxSlider = new VolSlider(commonMesh, AudioType::SFX, AEVec2{ 0.f, -100.f }, 500.f);
	_sfxSlider = sfxSlider;
	AddChild(sfxSlider);

	// level pause menu
	_titleText = "GAME PAUSED";

	// restart btn
	auto* restartBtn = new Button(commonMesh, AEVec2{ 240.f,-220.f }, 0.f, AEVec2{ 200.f,60.f });
	restartBtn->SetOnClick([this]()
		{
			SetActive(false);
			if (_onRestart) _onRestart();
		});
	restartBtn->SetActive(true);
	restartBtn->SetInteractable(true);
	_restartBtn = restartBtn;
	AddChild(restartBtn);

	// resume btn
	auto* resumeBtn = new Button(commonMesh, AEVec2{ 0.f, -220.f }, 0.f, AEVec2{ 200.f,60.f });
	resumeBtn->SetOnClick([this]()
		{
			SetActive(false);
			if (_onResume) _onResume();
		});
	resumeBtn->SetActive(true);
	resumeBtn->SetInteractable(true);
	_resumeBtn = resumeBtn;
	AddChild(resumeBtn);

	// home btn
	auto* homeBtn = new Button(commonMesh, AEVec2{ -240.f, -220.f }, 0.f, AEVec2{ 200.f, 60.f });
	homeBtn->SetOnClick([this]()
		{
			SetActive(false);
			if (_onQuit) _onQuit();
		});
	homeBtn->SetActive(true);
	homeBtn->SetInteractable(true);
	_homeBtn = homeBtn;
	AddChild(homeBtn);

}

void Overlay::SetupSettingsMenu()
{
	// sliiiders
	auto* masterSlider = new VolSlider(commonMesh, AudioType::MASTER, AEVec2{ 0.f, 100.f }, 500.f);
	_masterSlider = masterSlider;
	AddChild(masterSlider);

	auto* musicSlider = new VolSlider(commonMesh, AudioType::MUSIC, AEVec2{ 0.f, 0.f }, 500.f);
	_musicSlider = musicSlider;
	AddChild(musicSlider);

	auto* sfxSlider = new VolSlider(commonMesh, AudioType::SFX, AEVec2{ 0.f, -100.f }, 500.f);
	_sfxSlider = sfxSlider;
	AddChild(sfxSlider);

	// main menu pause menu (settings)
	_titleText = "SETTINGS";

	// prev btn
	auto* prevBtn = new Button(commonMesh, AEVec2{ 710.f, -360.f }, 0.f, AEVec2{ 120.f, 120.f });
	prevBtn->SetOnClick([this]()
		{
			SetActive(false);
			if (_onResume) _onResume();
		});

	prevBtn->SetActive(true);
	prevBtn->SetInteractable(true);
	_prevBtn = prevBtn;
	AddChild(prevBtn);
}

void Overlay::SetupLevelMenu()
{
	// level pause menu
	_titleText = "GAME OVER";

	// First candy
	candy1.SetMesh(commonMesh);
	candy1.UpdateScale(AEVec2{ 100.f, 100.f });
	candy1.UpdatePosition(AEVec2{ -100.f, 100.f });

	// Second candy
	candy2.SetMesh(commonMesh);
	candy2.UpdateScale(AEVec2{ 100.f, 100.f });
	candy2.UpdatePosition(AEVec2{ 0.f, 100.f });

	// Second candy
	candy3.SetMesh(commonMesh);
	candy3.UpdateScale(AEVec2{ 100.f, 100.f });
	candy3.UpdatePosition(AEVec2{ 100.f, 100.f });


	// restart btn
	auto* restartBtn = new Button(commonMesh, AEVec2{ 0.f,-220.f }, 0.f, AEVec2{ 200.f,60.f });
	restartBtn->SetOnClick([this]()
		{
			SetActive(false);
			if (_onRestart) _onRestart();
		});
	restartBtn->SetActive(true);
	restartBtn->SetInteractable(true);
	_restartBtn = restartBtn;
	AddChild(restartBtn);

	// resume btn
	auto* nextBtn = new Button(commonMesh, AEVec2{ 240.f, -220.f }, 0.f, AEVec2{ 200.f,60.f });
	nextBtn->SetOnClick([this]()
		{
			SetActive(false);
			if (_onResume) _onResume();
		});
	nextBtn->SetActive(true);
	nextBtn->SetInteractable(true);
	_nextBtn = nextBtn;
	AddChild(nextBtn);

	// home btn
	auto* homeBtn = new Button(commonMesh, AEVec2{ -240.f, -220.f }, 0.f, AEVec2{ 200.f, 60.f });
	homeBtn->SetOnClick([this]()
		{
			SetActive(false);
			if (_onQuit) _onQuit();
		});
	homeBtn->SetActive(true);
	homeBtn->SetInteractable(true);
	_homeBtn = homeBtn;
	AddChild(homeBtn);
}

void Overlay::UpdateGameOverMenu(f32 time, int health)
{
	_timeTaken = time;
	//performance = gameLose ? LOSE : time < 30 ? THREE_STARS : time < 60 ? TWO_STARS : ONE_STAR;
	performance = health <= 0 ? LOSE : health == 3 ? THREE_STARS : health == 2 ? TWO_STARS : ONE_STAR;
	memset(buffer, 0, sizeof(buffer));
	switch (performance) {
	case THREE_STARS:
		sprintf_s(buffer, "Time: %.2fs \nFlawless Victory!\nAmazing survival skills!", _timeTaken);
		candy1.SetTexture(candy);
		candy2.SetTexture(candy);
		candy3.SetTexture(candy);
		_titleText = "LEVEL COMPLETE";
		break;

	case TWO_STARS:
		sprintf_s(buffer, "Time: %.2fs \nStrong Finish!\nYou handled the damage well!", _timeTaken);
		candy1.SetTexture(candy);
		candy2.SetTexture(candy);
		candy3.SetTexture(greyCandy);
		_titleText = "LEVEL COMPLETE";
		break;

	case ONE_STAR:
		sprintf_s(buffer, "Time: %.2fs \nYou Survived!\nThat was a close call!", _timeTaken);
		candy1.SetTexture(candy);
		candy2.SetTexture(greyCandy);
		candy3.SetTexture(greyCandy);
		_titleText = "LEVEL COMPLETE";
		break;

	case LOSE:
		sprintf_s(buffer, "Oops, You Lost!\nDon't give up!\nTry again.");		
		candy1.SetTexture(greyCandy);
		candy2.SetTexture(greyCandy);
		candy3.SetTexture(greyCandy);
		_titleText = "LEVEL FAILED";
		_homeBtn->UpdatePosition({ -150.f, -220.f });
		_restartBtn->UpdatePosition({ 150.f, -220.f });
		_nextBtn->UpdatePosition({ 900.f, -500.f }); // hide outside screen
		break;
	}
}
