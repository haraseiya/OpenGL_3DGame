#pragma once
#include "Game.h"
#include "SceneBase.h"

class Texture;
class PlayerBase;
class EnemyBase;
class LevelManager;
class Weapon;
class CameraActor;
class LevelActor;

class TitleScene : public SceneBase
{
public:
	TitleScene();
	virtual ~TitleScene();

	SceneBase* update();
	void       draw();

private:
	void SetPlayer();

	class DebugGrid* mGrid;
	LevelManager* mLevel;
	PlayerBase* mPlayer;
	EnemyBase* mEnemy;
	Texture* mRogo;
	Texture* mPressButton;

	Weapon* mWeapon;
	CameraActor* mCamera;

	LevelActor* mLevelActor;

	Vector3 mEnemyCameraOffset;
	Vector3 mPlayerCameraOffset;
	float mTimer;
};