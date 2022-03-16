#pragma once
#include "SceneBase.h"

class Texture;
class Player1;
class BitMapText;
class CameraActor;
class LevelActor;

class ResultScene:public SceneBase
{
public:
	ResultScene();
	~ResultScene();

	SceneBase* update();
	void draw();

private:
	void DrawUI();

	Texture* m_texture;
	Player1* mPlayer;

	BitMapText* mFont;
	CameraActor* mCamera;

	LevelActor* mLevelActor;

	int mScore;
	std::vector<int> mHighScores;
	int mLerpScore;
};

