/*!***********************************************************************
\file   UIManager.hpp
\author Breanne Chua
\date   16/01/2026
\par    Last Updated by: Breanne Chua
\par    Project: Cardinals

\brief    UIManager is responsible for the UI interfaces.

\par    Changes: None
**************************************************************************/

#pragma once
#ifndef _UI_Manager_Header_
#define _UI_Manager_Header_

#include <AEEngine.h>
#include <vector>
#include <functional>

#include "../header/GameObject.hpp"
#include "../header/InputHandler.hpp"

// this guy IS every ui element
class UIElement : public GameObject
{
protected:
	bool isActive = false;
	bool isInteractable = false;

	Color tint = Colors::White; // full brightness
	std::vector<UIElement*> children;
	Texture* currentTexture{};

	std::string tooltipToDraw = "";
	AEVec2 tooltipPos = { 0,0 };
	bool isToolTipShown = false;

public:

	// inheriting gameobject constructors
	using GameObject::GameObject;
	virtual ~UIElement() = default; // destructor

	UIElement(AEGfxVertexList* mesh, AEVec2 pos, float rot, AEVec2 scale) : GameObject(mesh, pos, rot, scale) {}

	virtual void SetActive(bool active) { isActive = active; }

	// doesnt work
	void SetInteractable(bool interactable) 
	{ 
		isInteractable = interactable;
		if (!isInteractable)
		{
			tint = { 0.f, 0.f, 0.f, 1.0f };
		}
	}

	void AddChild(UIElement* child) // for kiddos, mostly for popup to attach
	{
		children.push_back(child);
		child->SetActive(false);
	}

	void SetTooltip(std::string text, AEVec2 pos)
	{
		tooltipToDraw = text;
		tooltipPos = pos;
		isToolTipShown = true;
	}

	// getters
	bool IsActive() const { return isActive; }
	bool IsInteractable() const { return isInteractable; }
	bool IsPointInside(float x, float y) const;
	bool ToggleActive(u8 key);
	Color GetTint() const { return tint; }

	// tool tip shit
	bool IsShowingTooltip() const { return isToolTipShown; }
	std::string GetTooltipText() const { return tooltipToDraw; }
	AEVec2 GetTooltipPos() const { return tooltipPos; }
	void ResetTooltip() { isToolTipShown = false; } // reset every frame so tooltips dont smooch

	virtual void Update(float dt) {}
	virtual void OnClick() {}
};

// this guy just handles all the diff elements
// kinda stores everything and thats it
class UIManager {

private:
	// dynamic array to store different elements
	std::vector<UIElement*> uiElements;

	// cuz text rn is const and . i need to tickle it
	// apparently mutable exists to edit const variables anyway
	// i spent too long wondering how to wiggle around this sauuur
	mutable Text tooltipRenderer;
	bool isFontLoaded = false;

public:

	virtual ~UIManager() = default; // destructor!! IMPORTANT!!!

	static AEVec2 GetNewMousePos(); // ssingleton

	void InitTooltipFont(const FontFile& font, float size) 
	{ 
		tooltipRenderer.Load_Font(font, size);
		isFontLoaded = true;
	}

	void Add(UIElement* element); // just adds it so it Exists
	void Update(float dt); // loops thru all elements and calls their updaate
	void Draw() const;

	void HandleClick();
	void Exit(); // for the VECTORS

};

///
/// elements heeere
/// 

class Button : public UIElement
{
private:
	std::function<void()> onClick;
	std::string toolTipText = "u didn't gimme a name";
	float hoverTimer = 0.0f;

public:
	using UIElement::UIElement;
	Button() = default;

	// use like startBtn->SetTooltipText("ur mom"); okayyy :3
	void SetTooltipText(const std::string& text) { toolTipText = text; }
	void SetOnClick(std::function<void()> func) { onClick = func; }

	void Update(float dt) override
	{
		currentTexture = tex; // pointer to text in gameobj
		if (!isActive || !isInteractable || currentTexture == nullptr) return;

		float scaleMultiplier = 1.1f;
		AEVec2 mousePos = UIManager::GetNewMousePos();

		// hover code, combined tints + scales texture when mouse is within button pos
		if (IsPointInside(mousePos.x, mousePos.y) && currentTexture)
		{
			//std::cout << "bruh " << mousePos.x << " " << mousePos.y << std::endl;
			tint = { Colors::PastelPurple };
			GameObject::UpdateScale({ currentTexture->Get_Image_Width() * scaleMultiplier, currentTexture->Get_Image_Height() * scaleMultiplier });

			// small delay because having it shoved in ur face is ass
			hoverTimer += dt;
			if (hoverTimer > 0.5f) 
			{
				this->SetTooltip(toolTipText, mousePos);
			}
		}
		else
		{
			tint = { 1.0f, 1.0f, 1.0f, 1.0f };
			scaleMultiplier = 1.0f;

			GameObject::UpdateScale({ currentTexture->Get_Image_Width() * scaleMultiplier, currentTexture->Get_Image_Height() * scaleMultiplier });
			hoverTimer = 0.0f;
		}
	}

	void OnClick() override
	{
		// literally just error checks and then handles onclick lmao
		if (!isActive || !isInteractable)
			return;

		if (onClick) onClick();
	}
};

class PopUp : public UIElement
{
public:
	using UIElement::UIElement;

	// overrides setactive for the kiddos
	// refactor stuff using this in the future -2/2/26
	void SetActive(bool active) override
	{
		isActive = active;
		for (auto* child : children) 
		{
			child->SetActive(active);
			child->SetInteractable(active);
		}
	}
};

#endif // !_UI_Manager_Header_