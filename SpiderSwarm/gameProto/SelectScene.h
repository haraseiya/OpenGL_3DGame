#pragma once
#include "SceneBase.h"

class PlayerBase;

class SelectScene : public SceneBase
{
public:
	enum PlayerNumber
	{
		PLAYER_1 = 0,
		PLAYER_2,
	};

	SelectScene();
	~SelectScene();

	SceneBase* update();
	void draw();

	PlayerBase* SelectedPlayer();

private:
	std::vector<PlayerBase*> mPlayers;	// 全プレイヤーの
	PlayerBase* mSelectedPlayer;		// 選ばれたプレイヤー
	PlayerBase* mNoSelectedPlayer;		// 選ばれなかったプレイヤー

	CameraActor* mCamera;

	PlayerNumber mNumber;
};

