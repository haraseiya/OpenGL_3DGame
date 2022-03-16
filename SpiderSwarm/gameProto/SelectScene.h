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
	std::vector<PlayerBase*> mPlayers;	// �S�v���C���[��
	PlayerBase* mSelectedPlayer;		// �I�΂ꂽ�v���C���[
	PlayerBase* mNoSelectedPlayer;		// �I�΂�Ȃ������v���C���[

	CameraActor* mCamera;

	PlayerNumber mNumber;
};

