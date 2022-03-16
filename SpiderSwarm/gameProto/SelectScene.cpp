#include "SelectScene.h"
#include "Input.h"
#include "Player1.h"
#include "Player2.h"
#include "GameScene.h"
#include "CameraActor.h"

SelectScene::SelectScene()
{
	// プレイヤー生成
	mPlayers.push_back(new Player1());
	//mPlayers.push_back(new Player2());

	// デフォルトではプレイヤー1にセット
	mSelectedPlayer = mPlayers[0];

	// プレイヤー位置セット
	for (int i = 0; i < mPlayers.size(); i++)
	{
		mPlayers[0]->SetPosition(Vector3(0, 0, 0));
		//mPlayers[1]->SetPosition(Vector3(0, 200, 0));
	}

	// セレクトシーン用のプレイヤー振る舞いをセット
	for (auto p : mPlayers)
	{
		p->SetPlayerSceneState(PlayerSceneState::PLAYER_SELECTSCENE);
	}

	// ライト
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// カメラ追加
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
		printf("プレイヤー１が選ばれました\n");
		mNumber = PlayerNumber::PLAYER_1;
		mSelectedPlayer = mPlayers[PlayerNumber::PLAYER_1];
		mCamera->SetTarget(mPlayers[PlayerNumber::PLAYER_1]->GetPosition());
	}

	//if (INPUT_INSTANCE.IsKeyPushdown(KEY_Y))
	//{
	//	printf("プレイヤー２が選ばれました\n");
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
	// 背景色を設定
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// 画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();


	GAMEINSTANCE.GetRenderer()->Draw();

	// 画面のフリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}

