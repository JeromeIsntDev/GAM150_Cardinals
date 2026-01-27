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
// all of this is rudimentary as fuck rn so..

void UIManager::Add(UIElement* element)
{
	uiElements.push_back(element);
}

void UIManager::Update(float dt) 
{
    // big fat update
    for (auto* element : uiElements)
    {
        if (!element->IsActive())
            element->Update(dt);
    }

    HandleClick();
}

void UIManager::Draw() const
{
	for (UIElement* e : uiElements)
	{
		if (e->IsActive())
			e->Draw();
	}
}

// omg it handles
void UIManager::HandleClick()
{
    // just tells the compiler to find the instance using referencing olololol
    // thank you prasanna
    auto& input = InputHandler::GetInstance();

    // mouse pointing calculation because screen view is NNNOOTT the same as game view iykwim
    // yk idk if this was a problem i just coded it cuz i felt like it wld have an issue lol
    if (input.isMouseBtnTriggered(AEVK_LBUTTON))
    {
        float rawX, rawY;
        input.getMousePos(rawX, rawY);

        float windowWidth = (float)AEGfxGetWindowWidth();
        float windowHeight = (float)AEGfxGetWindowHeight();

        // convert top left to center
        float translatedX = rawX - (windowWidth / 2.0f);
        float translatedY = (windowHeight / 2.0f) - rawY;

        for (auto* element : uiElements)
        {
            // use the trnalsated mouse coordinates
            if (element->IsActive() && element->IsInteractable())
            {
                if (element->IsPointInside(translatedX, translatedY))
                {
                    element->OnClick();
                    std::cout << "Element clicked at: " << translatedX << ", " << translatedY << std::endl;
                    break;
                }
            }
        }
    }
}

void UIManager::Exit() 
{
    uiElements.clear(); // dangl;ing pointers fark you
}

bool UIElement::IsPointInside(float x, float y) const {
	// calling from the parent class woawow
	AEVec2 currentScale = GetScale();
	AEVec2 currentPos = GetPosition();

	float halfW = currentScale.x / 2.0f;
	float halfH = currentScale.y / 2.0f;

	// wowww calculations zzzzz
	return (x >= currentPos.x - halfW && x <= currentPos.x + halfW &&
		y >= currentPos.y - halfH && y <= currentPos.y + halfH);
}
