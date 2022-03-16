#include "SelectScene.h"
#include "Input.h"
#include "Player1.h"
#include "Player2.h"
#include "GameScene.h"
#include "CameraActor.h"

SelectScene::SelectScene()
{
	// �v���C���[����
	mPlayers.push_back(new Player1());
	//mPlayers.push_back(new Player2());

	// �f�t�H���g�ł̓v���C���[1�ɃZ�b�g
	mSelectedPlayer = mPlayers[0];

	// �v���C���[�ʒu�Z�b�g
	for (int i = 0; i < mPlayers.size(); i++)
	{
		mPlayers[0]->SetPosition(Vector3(0, 0, 0));
		//mPlayers[1]->SetPosition(Vector3(0, 200, 0));
	}

	// �Z���N�g�V�[���p�̃v���C���[�U�镑�����Z�b�g
	for (auto p : mPlayers)
	{
		p->SetPlayerSceneState(PlayerSceneState::PLAYER_SELECTSCENE);
	}

	// ���C�g
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// �J�����ǉ�
	mCamera= new CameraActor(mPlayers[0]);
	mCamera->Init(Vector3(150, 0, 150), Vector3(0, 0, 100), Vector3(0, 0, 0));
}

SelectScene::~SelectScene()
{
	//delete mPlayers[PlayerNumber::PLAYER_2];
}

SceneBase* SelectScene::update()
{
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_X))
	{
		printf("�v���C���[�P���I�΂�܂���\n");
		mNumber = PlayerNumber::PLAYER_1;
		mSelectedPlayer = mPlayers[PlayerNumber::PLAYER_1];
		mCamera->SetTarget(mPlayers[PlayerNumber::PLAYER_1]->GetPosition());
	}

	//if (INPUT_INSTANCE.IsKeyPushdown(KEY_Y))
	//{
	//	printf("�v���C���[�Q���I�΂�܂���\n");
	//	mNumber = PlayerNumber::PLAYER_2;
	//	mSelectedPlayer = mPlayers[PlayerNumber::PLAYER_2];
	//	mCamera -> SetTarget(mPlayers[PlayerNumber::PLAYER_2]->GetPosition());
	//}

	if (INPUT_INSTANCE.IsKeyPushdown(KEY_A))
	{
		return new GameScene(mSelectedPlayer);
	}

	return this;
}

void SelectScene::draw()
{
	// �w�i�F��ݒ�
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// ��ʃN���A
	GAMEINSTANCE.GetRenderer()->WindowClear();


	GAMEINSTANCE.GetRenderer()->Draw();

	// ��ʂ̃t���b�v
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}

