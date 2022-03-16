#include "UIScreen.h"
#include "UIManager.h"

UIScreen::UIScreen(UIManager* manager)
	: mManager(manager)
{
	mManager->AddUIScreen(this);
}

UIScreen::~UIScreen()
{
}

void UIScreen::Update(float deltaTime)
{
}

void UIScreen::Draw(Shader* shader)
{
}

void UIScreen::ProcessInput(const uint8_t* keys)
{
}

void UIScreen::HandleKeyPress(int key)
{
}

void UIScreen::Close()
{
}

void UIScreen::DrawTexture(Shader* shader, Texture* texture, const Vector2& offset, float scale)
{
}
