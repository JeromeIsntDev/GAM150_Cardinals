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
#include "../header/MeshGenerator.hpp"
#include "../header/UIManager.hpp"
#include "../header/GStateManager.hpp"
#include "../header/InputHandler.hpp"

extern int isGamePaused;

VolSlider::VolSlider(AEGfxVertexList* mesh, AudioType type, AEVec2 pos, f32 width)
	: UIElement(mesh, pos, 0.f, AEVec2{ width, 20.f }), // bar mesh
	_audioType(type),
	_width(width),
	_knob(mesh, AEVec2{ pos.x + (width / 2.f), pos.y }, 0.f, AEVec2{30.f, 30.f})
{

	SetActive(true);
	SetInteractable(true);
	_knob.SetActive(true);
}

void VolSlider::Update(f32 dt) {
	if (!IsActive() || !IsInteractable()) return;
	// check for dragging
	AEVec2 mousePos = UIManager::GetNewMousePos();
	bool hitKnob = _knob.IsPointInside(mousePos.x, mousePos.y);
	if (AEInputCheckTriggered(AEVK_LBUTTON) && hitKnob) {
		_isDragging = true;
	}
	if (AEInputCheckReleased(AEVK_LBUTTON)) {
		_isDragging = false;
	}
	if (_isDragging) {
		f32 barStartX = GetPosition().x - (_width / 2.f);
		f32 barEndX = GetPosition().x + (_width / 2.f);
		// clamp knob position within bar
		f32 newKnobX = mousePos.x;
		if (newKnobX < barStartX) newKnobX = barStartX;
		if (newKnobX > barEndX) newKnobX = barEndX;
		// move knob
		_knob.UpdatePosition(AEVec2{ newKnobX, GetPosition().y });
		// update volume based on knob position
		f32 volumePercent = (newKnobX - barStartX) / _width;
		Audio::Instance().Set_Volume(_audioType, volumePercent);
	}
}

void VolSlider::Draw() const
{
	GameObject::Draw(); // draw bar
	if (_knob.IsActive())
	{
		// set knob texture
		// colour for now ... 
		AEGfxSetColorToMultiply(1.0f, 0.f, 0.f, 1.f);
		_knob.Draw(); // draw knob
		AEGfxSetColorToMultiply(1.0f, 1.f, 1.f, 1.f); // reset
	}
}

void VolSlider::SetTextures(Texture* barTex, Texture* knobTex)
{
	if (barTex) GameObject::SetTexture(barTex);
	if (knobTex) _knob.SetTexture(knobTex);
}

PauseMenu::~PauseMenu()
{
	_pauseMenuUI.Exit();
	if (_commonMesh) {
		AEGfxMeshFree(_commonMesh);
		_commonMesh = nullptr;
	}
}

PauseMenu::PauseMenu(bool isInGame)
{
	_commonMesh = RectMeshCenter();

	// setup background
	_bg.SetMesh(_commonMesh);
	_bg.UpdateScale(AEVec2{ 1600.f, 900.f });
	_bg.UpdatePosition(AEVec2{ 0.f, 0.f });

	// setup volume sliders
	auto masterSlider = std::make_unique<VolSlider>(_commonMesh,AudioType::MASTER, AEVec2{ 0.f, 100.f }, 300.f);
	_pauseMenuUI.Add(masterSlider.get());
	_elements.push_back(std::move(masterSlider));

	auto musicSlider = std::make_unique<VolSlider>(_commonMesh,AudioType::MUSIC, AEVec2{ 0.f, 0.f }, 300.f);
	_pauseMenuUI.Add(musicSlider.get());
	_elements.push_back(std::move(musicSlider));

	auto sfxSlider = std::make_unique<VolSlider>(_commonMesh,AudioType::SFX, AEVec2{ 0.f, -100.f }, 300.f);
	_pauseMenuUI.Add(sfxSlider.get());
	_elements.push_back(std::move(sfxSlider));

	// back btn
	auto backBtn = std::make_unique<Button>(_commonMesh, AEVec2{0.f, -200.f}, 0.f, AEVec2{200.f, 50.f});
	backBtn->SetOnClick([this, isInGame]() {
		this->HideMenu();
		// unpause game if in-game
		if (isInGame) {
			isGamePaused = 0;
		}
	});
	backBtn->SetActive(true);
	backBtn->SetInteractable(true);
	_pauseMenuUI.Add(backBtn.get()); // add to pause menu UI
	_elements.push_back(std::move(backBtn)); // store

	// quit btn
	if (isInGame) {
		auto quitBtn = std::make_unique<Button>(_commonMesh, AEVec2{ 0.f, -300.f }, 0.f, AEVec2{ 200.f, 50.f });
		quitBtn->SetOnClick([]() {
			isGamePaused = 0;
			UpdateGameManagerState(GameStateEnum::MainMenu);
			});
		quitBtn->SetActive(true);
		quitBtn->SetInteractable(true);
		_pauseMenuUI.Add(quitBtn.get()); // add to pause menu UI
		_elements.push_back(std::move(quitBtn)); // store
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
		// draw pause menu UI
		AEGfxSetTransparency(1.f);
		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f); // reset
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		_pauseMenuUI.Draw();
	}
}

void PauseMenu::SetTextures(Texture* bg, Texture* bar, Texture* knob)
{
	// set background texture
	if (bg) _bg.SetTexture(bg);

	// set textures for volume sliders
	for(auto& element : _elements) {
		// check if element is VolSlider
		VolSlider* slider = dynamic_cast<VolSlider*>(element.get());
		if (slider) {
			slider->SetTextures(bar, knob);
			continue; // skip to next element
		}
		// for buttons, we can set a generic texture if needed
		Button* btn = dynamic_cast<Button*>(element.get());
		if (btn) {
			// set button texture if desired
			btn->SetTexture(bar);
		}
	}
}
