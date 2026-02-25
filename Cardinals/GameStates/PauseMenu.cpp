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
	audioType(type), width(width),
	knob(mesh, AEVec2{ pos.x + (width / 2.f), pos.y }, 0.f, AEVec2{50.f, 50.f})
{
	SetActive(true);
	SetInteractable(true);
	knob.SetActive(true);
}

void VolSlider::OnClick()
{
	// start drag on click
	if (!isActive || !isInteractable) return;
	isDragging = true;
}

void VolSlider::Update(f32 dt) 
{
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
}

///
/// PAUSE MENU
///

PauseMenu::PauseMenu(AEGfxVertexList* mesh,
	bool isInGame, std::function<void()> onResume, std::function<void()> onQuit, std::function<void()> onRestart) :
	UIElement(mesh, AEVec2{ 0.f, 0.f }, 0.f, AEVec2{ 1600, 900.f }), commonMesh(mesh)
{
	bg.SetMesh(commonMesh);
	bg.UpdateScale(AEVec2{ 1600.f, 900.f });
	bg.UpdatePosition(AEVec2{ 0.f, 0.f });

	cardBG.SetMesh(commonMesh);
	cardBG.UpdateScale(AEVec2{ 800.f, 600.f });
	cardBG.UpdatePosition(AEVec2{ 0.f, 0.f });

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


	if (isInGame) {

		// level pause menu
		_titleText = "GAME PAUSED";

		// restart btn
		auto* restartBtn = new Button(commonMesh, AEVec2{ -240.f,-220.f }, 0.f, AEVec2{200.f,60.f});
		restartBtn->SetOnClick([this, onRestart]() 
			{
				SetActive(false);
				if (onRestart) onRestart();
			});
		restartBtn->SetActive(true);
		restartBtn->SetInteractable(true);
		_restartBtn = restartBtn;
		AddChild(restartBtn);

		// resume btn
		auto* resumeBtn = new Button(commonMesh, AEVec2{ 0.f, -220.f},0.f, AEVec2{200.f,60.f});
		resumeBtn->SetOnClick([this, onResume]() 
			{
				SetActive(false);
				if (onResume) onResume();
			});
		resumeBtn->SetActive(true);
		resumeBtn->SetInteractable(true);
		_resumeBtn = resumeBtn;
		AddChild(resumeBtn);

		// home btn
		auto* homeBtn = new Button(commonMesh, AEVec2{ 240.f, -220.f }, 0.f, AEVec2{ 200.f, 60.f });
		homeBtn->SetOnClick([this, onQuit]()
			{
				SetActive(false);
				if (onQuit) onQuit();
			});
		homeBtn->SetActive(true);
		homeBtn->SetInteractable(true);
		_homeBtn = homeBtn;
		AddChild(homeBtn);
	}
	else {

		// main menu pause menu (settings)
		_titleText = "SETTINGS";

		// prev btn
		auto* prevBtn = new Button(commonMesh, AEVec2{ 710.f, -360.f }, 0.f, AEVec2{ 120.f, 120.f });
		prevBtn->SetOnClick([this, onResume]()
			{
				SetActive(false);
				if (onResume) onResume();
			});

		prevBtn->SetActive(true);
		prevBtn->SetInteractable(true);
		_prevBtn = prevBtn;
		AddChild(prevBtn);
	}

	SetActive(false);
	SetInteractable(false);
}

void PauseMenu::SetActive(bool active)
{
	UIElement::SetActive(active);
	isInteractable = active;

	for (auto* child : children)
	{
		child->SetActive(active);
		child->SetInteractable(active);
	}
}

void PauseMenu::OnClick()
{
	if (!IsActive() || !IsInteractable()) return;

	AEVec2 mousePos = UIManager::GetNewMousePos();
	for (auto* child : children)
	{
		if (child->IsActive() && child->IsInteractable() && child->IsPointInside(mousePos.x, mousePos.y))
		{
			child->OnClick();
			break;
		}
	}
}

void PauseMenu::Update(f32 dt)
{
	if (AEInputCheckTriggered(AEVK_P)) { SetActive(!IsActive()); isGamePaused = IsActive() ? 1 : 0; }
	if (!IsActive()) return;

	for (auto* child : children)
	{
		if (child->IsActive())
		{
			child->ResetTooltip();
			child->Update(dt);
		}
	}
}

void PauseMenu::Draw() const
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

	// children
	AEGfxSetTransparency(1.f);
	for (auto* child : children)
	{
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
		//_titleFont->Change_Text_Color(Colors::White);
	}

	// reset to default
	AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);
	AEGfxSetTransparency(1.f);
}

void PauseMenu::SetTextures(const PauseMenuTextures& tex)
{
	if (tex.bg) bg.SetTexture(tex.bg);
	if (tex.cardBg) cardBG.SetTexture(tex.cardBg);

	if (_masterSlider) _masterSlider->SetTextures(tex.bar, tex.knobMaster);
	if (_musicSlider)  _musicSlider->SetTextures(tex.bar, tex.knobMusic);
	if (_sfxSlider)    _sfxSlider->SetTextures(tex.bar, tex.knobSFX);
	if (_prevBtn)      _prevBtn->SetTexture(tex.prevBtn);
	if (_resumeBtn)    _resumeBtn->SetTexture(tex.resumeBtn);
	if (_restartBtn)   _restartBtn->SetTexture(tex.restartBtn);
	if (_homeBtn)      _homeBtn->SetTexture(tex.homeBtn);

	if (tex.titleFont) _titleFont = tex.titleFont;
}