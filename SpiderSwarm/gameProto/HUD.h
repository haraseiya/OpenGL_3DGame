#pragma once

#include "UIScreen.h"

class Shader;

class HUD : public UIScreen
{
public:
	HUD(UIManager* manager);
	~HUD();

	void Update(float deltaTime)override;
	void Draw(Shader* shader)override;

private:

};

