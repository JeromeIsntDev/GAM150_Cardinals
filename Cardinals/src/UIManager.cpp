/*!***********************************************************************
\file    UIManager.cpp
\author Breanne Chua
\date    16/01/2026
\par    Last Updated by: Breanne Chua
\par    Project: Cardinals

\brief    UIManager is responsible for the UI interfaces.

\par    Changes: None
**************************************************************************/

#include "../header/UIManager.hpp"

// adds every new element to the dynamic array 
// so we can explode ALL the UIs whenever we want
void UIManager::Add(UIElement* element)
{
	uiElements.push_back(element);
}

// all of this is rudimentary as fuck rn so..
void UIManager::Update(float dt)
{
	for (UIElement* e : uiElements)
	{
		if (e->IsActive())
			e->Update(dt);
	}
}

void UIManager::Draw() const
{
	for (UIElement* e : uiElements)
	{
		if (e->IsActive())
			e->Draw();
	}
}

// doesn't work yet hehe oops
void UIManager::HandleClick()
{
	for (UIElement* e : uiElements)
	{
		if (e->IsInteractable())
			e->OnClick();
	}
}
