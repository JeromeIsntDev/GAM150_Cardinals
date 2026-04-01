#include "UIManager.hpp"

///
/// UI ELEMENTS
/// 

void UIElement::SetActive(bool active) { isActive = active; }

void UIElement::SetInteractable(bool interactable)
{
    isInteractable = interactable;
    if (!isInteractable) { tint = { 0.f, 0.f, 0.f, 1.0f }; }
}

void UIElement::SetTooltip(std::string text, AEVec2 pos)
{
    tooltipToDraw = text; tooltipPos = pos; isToolTipShown = true;
}

void UIElement::AddChild(UIElement* child)
{
    children.push_back(child);
    child->SetActive(false);
}

void UIElement::ResetTooltip() { isToolTipShown = false; }

bool UIElement::IsPointInside(float x, float y) const 
{
    return HoverMesh(static_cast<s32>(x), static_cast<s32>(y));
}

bool UIElement::ToggleActive(u8 key)
{
    if (AEInputCheckTriggered(key))
    {
        bool newState = !this->IsActive();
        this->SetActive(newState);

        for (auto child : children) 
        {
            child->SetActive(newState);
            child->SetInteractable(newState);
        }

        return newState;
    }

    return this->IsActive();
}

/// 
/// UI BUTTONS
/// 

void Button::SetTooltipText(const std::string& text) { toolTipText = text; }
void Button::SetOnClick(std::function<void()> func) { onClick = func; }

void Button::Update(float dt)
{
    if (!isActive || !isInteractable)
    {
        ResetTooltip();
        hoverTimer = 0.0f;
        return;
    }

    float scaleMultiplier = 1.1f;
    AEVec2 mousePos = UIManager::GetNewMousePos();

    if (IsPointInside(mousePos.x, mousePos.y))
    {
        tint = { Colors::PastelPurple };
        GameObject::UpdateScale({ initialScale.x * scaleMultiplier, initialScale.y * scaleMultiplier });

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

        GameObject::UpdateScale({ initialScale.x * scaleMultiplier, initialScale.y * scaleMultiplier });

        hoverTimer = 0.0f;
        ResetTooltip();
    }
}

void Button::OnClick()
{
    if (!isActive || !isInteractable)
        return;

    if (onClick) onClick();
}

///
/// UI MANAGER
/// 

AEVec2 UIManager::GetNewMousePos()
{
    auto& input = InputHandler::GetInstance();
    float rawX, rawY;
    input.GetMouseScreenPosition(rawX, rawY);

    float windowWidth = (float)AEGfxGetWindowWidth(), windowHeight = (float)AEGfxGetWindowHeight();

    return AEVec2 { rawX - (windowWidth / 2.0f), (windowHeight / 2.0f) - rawY };
}

void UIManager::InitTooltipFont(const FontFile& font, float size)
{
    tooltipRenderer.Load_Font(font, size);
    isFontLoaded = true;
}

void UIManager::Add(UIElement* element)
{
	uiElements.push_back(element);
}

void UIManager::Update(float dt) 
{
    for (UIElement* e : uiElements)
    {
        if (e->IsActive()) 
        {
            e->ResetTooltip();
            e->Update(dt);
        }
    }

    HandleClick();
}

void UIManager::Draw() const
{
    for (UIElement* e : uiElements) 
    {
        if (e->IsActive())
        {
            AEGfxSetColorToMultiply(e->GetTint().r, e->GetTint().g, e->GetTint().b, e->GetTint().a);
            e->Draw();
        }
    }

    AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);

    if (!isFontLoaded) return;

    for (UIElement* e : uiElements) 
    {
        if (e->IsActive() && e->IsShowingTooltip()) 
        {
            AEVec2 pos = e->GetTooltipPos();

            // std::cout << "drawin tooltip: " << e->GetTooltipText() << " at " << e->GetTooltipPos().x << std::endl;
            tooltipRenderer.Input_Text(e->GetTooltipText().c_str());
            tooltipRenderer.Draw_Text(pos.x + 20.f, pos.y + 20.f);
        }
    }
}

void UIManager::HandleClick()
{
    auto& input = InputHandler::GetInstance();

    if (input.isKeyTriggered(AEVK_LBUTTON))
    {
        AEVec2 mousePos = GetNewMousePos();
        for (UIElement* e : uiElements)
        {
            if (e->IsActive() && e->IsInteractable())
            {
                if (e->IsPointInside(mousePos.x, mousePos.y))
                {
                    bgm.PlayAudio("CLICK", AudioType::SFX, 1.f, 0);
                    e->OnClick();
                    //std::cout << "DEBUG: Element clicked at: " << mousePos.x << ", " << mousePos.y << std::endl;
                    break;
                }
            }
        }
    }
}

void UIManager::Exit() 
{
    uiElements.clear();
    isFontLoaded = false;
}

void UIManager::ForceResetToolTips() 
{
    for (auto* e : uiElements) 
    {
        e->ResetTooltip();
    }
}