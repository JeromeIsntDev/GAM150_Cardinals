/*!***********************************************************************
\file	PauseMenu.cpp
\author	Zoey Tan
\date	31/01/2026
\par	Last Updated by: Zoey Tan
\par	Project: Cardinals

\brief	PauseMenu is responsible for displaying the pause menu.

\par	Changes: None
**************************************************************************/

#include "PauseMenu.hpp"

VolSlider::VolSlider(AEGfxVertexList* mesh, AudioType type, AEVec2 pos, f32 width)
	: UIElement(mesh, pos, 0.f, AEVec2{ width, 20.f }), // bar mesh
	_audioType(type),
	_width(width),
	_knob(mesh, AEVec2{ pos.x + (width / 2.f), pos.y }, 0.f, AEVec2{50.f, 50.f})
{

	SetActive(true);
	SetInteractable(true);
	_knob.SetActive(true);
}

void VolSlider::OnClick()
{
	// start drag on click
	if (!isActive || !isInteractable) return;
	_isDragging = true;
}

void VolSlider::Update(f32 dt) 
{
	if (!IsActive() || !IsInteractable()) return;

	if (AEInputCheckReleased(AEVK_LBUTTON))
	{
		_isDragging = false;
	}

	if (_isDragging) 
	{
		AEVec2 mousePos = UIManager::GetNewMousePos();

		f32 barStartX = GetPosition().x - (_width / 2.f);
		f32 barEndX = GetPosition().x + (_width / 2.f);

		// clamp knob position within bar
		f32 newKnobX = AEClamp(mousePos.x, barStartX, barEndX);
		_knob.UpdatePosition(AEVec2{ newKnobX, GetPosition().y });

		f32 volumePercent = (newKnobX - barStartX) / _width;
		AudioManager::Instance().SetVolume(_audioType, volumePercent);
	}
}

void VolSlider::Draw() const
{
	// set color for bar ... igmnore pink texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// pure white with 0 alpha to show texture colour only
	AEGfxSetColorToAdd(1.f, 1.f, 1.f, 0.f);

	GameObject::Draw(); // draw bar for white

	if (_knob.IsActive())
	{
		// set knob texture
		// colour for now ... 
		AEGfxSetColorToAdd(0.0f, 0.f, 0.f, 0.f);
		_knob.Draw(); // draw knob
	}
}

void VolSlider::SetTextures(Texture* barTex, Texture* knobTex)
{
	if (barTex) GameObject::SetTexture(barTex);
	if (knobTex) _knob.SetTexture(knobTex);
}

///
/// PAUSE MENU
///

PauseMenu::PauseMenu(bool isInGame)
{
	_commonMesh = RectMeshCenter();

	// setup background
	_bg.SetMesh(_commonMesh);
	_bg.UpdateScale(AEVec2{ 1600.f, 900.f });
	_bg.UpdatePosition(AEVec2{ 0.f, 0.f });

	// setup card bg
	_cardBg.SetMesh(_commonMesh);
	_cardBg.UpdateScale(AEVec2{ 800.f, 600.f });
	_cardBg.UpdatePosition(AEVec2{ 0.f, 0.f });

	// setup volume sliders
	auto masterSlider = std::make_unique<VolSlider>(_commonMesh, AudioType::MASTER, AEVec2{ 0.f, 100.f }, 500.f);
	_pauseMenuUI.Add(masterSlider.get());
	_elements.push_back(std::move(masterSlider));

	auto musicSlider = std::make_unique<VolSlider>(_commonMesh, AudioType::MUSIC, AEVec2{ 0.f, 0.f }, 500.f);
	_pauseMenuUI.Add(musicSlider.get());
	_elements.push_back(std::move(musicSlider));

	auto sfxSlider = std::make_unique<VolSlider>(_commonMesh, AudioType::SFX, AEVec2{ 0.f, -100.f }, 500.f);
	_pauseMenuUI.Add(sfxSlider.get());
	_elements.push_back(std::move(sfxSlider));

	// prev btn
	auto prevBtn = std::make_unique<Button>(_commonMesh, AEVec2{ 710.f, -360.f }, 0.f, AEVec2{ 120.f, 120.f });
	prevBtn->SetOnClick([this, isInGame]() {
		this->HideMenu();
		// unpause game if in-game
		if (isInGame) {
			isGamePaused = 0;
		}
		});

	prevBtn->SetActive(true);
	prevBtn->SetInteractable(true);
	_pauseMenuUI.Add(prevBtn.get()); // add to pause menu UI
	_elements.push_back(std::move(prevBtn)); // store

	// quit btn
	if (isInGame) {
		{
			auto quitBtn = std::make_unique<Button>(_commonMesh, AEVec2{ 0.f, -300.f }, 0.f, AEVec2{ 200.f, 50.f });
			quitBtn->SetOnClick([]()
				{
					isGamePaused = 0;
					UpdateGameManagerState(GameStateEnum::MainMenu);
				});

			quitBtn->SetActive(true);
			quitBtn->SetInteractable(true);
			_pauseMenuUI.Add(quitBtn.get()); // add to pause menu UI
			_elements.push_back(std::move(quitBtn)); // store
		}
	}
}

void PauseMenu::ShowMenu() { _isVisible = true; }
void PauseMenu::HideMenu() { _isVisible = false; }

void PauseMenu::Update(f32 dt)
{
	// press 'P' to toggle pause menu
	if (AEInputCheckTriggered(AEVK_P)) {
		if (_isVisible) {
			HideMenu();
		}
		else {
			ShowMenu();
		}
		// sync game pause state
		isGamePaused = _isVisible ? 1 : 0;
	}
	if (_isVisible) {
		_pauseMenuUI.Update(dt);
	}
}

void PauseMenu::Draw() 
{
	if (_isVisible) {
		// draw dimmer bg (semi-transparent)
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToMultiply(0.f, 0.f, 0.f, 0.5f); // 50% black
		AEGfxSetTransparency(0.5f);
		_bg.Draw();

		// card bg
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f); // reset
		AEGfxSetTransparency(0.75f);
		_cardBg.Draw();

		// draw UI elements
		AEGfxSetTransparency(1.f);
		_pauseMenuUI.Draw();
	}
}

void PauseMenu::SetTextures(Texture* bg, Texture* bar, Texture* knobMaster, Texture* knobMusic, Texture* knobEFX, Texture* prevBtn, Texture* cardBg)
{// set background texture
	if (bg) _bg.SetTexture(bg);
	if (cardBg) _cardBg.SetTexture(cardBg);

	// set textures for volume sliders
	for(auto& element : _elements) {
		// check if element is a slider (bar+knob)
		VolSlider* slider = dynamic_cast<VolSlider*>(element.get());
		if (slider) {
			switch (slider->GetAudioType())
			{
			case AudioType::MASTER:
				slider->SetTextures(bar, knobMaster);
				break;
			case AudioType::MUSIC:
				slider->SetTextures(bar, knobMusic);
				break;
			case AudioType::SFX:
				slider->SetTextures(bar, knobEFX);
				break;
			}
			continue; // skip to next element
		}
		// check if element is a button (prevBtn)
		Button* btn = dynamic_cast<Button*>(element.get());
		if (btn) {
			// set button texture if desired
			btn->SetTexture(prevBtn);
		}
	}
}

PauseMenu::~PauseMenu()
{
	_elements.clear();
	if (_commonMesh)
	{
		AEGfxMeshFree(_commonMesh);
		_commonMesh = nullptr;
	}
}
