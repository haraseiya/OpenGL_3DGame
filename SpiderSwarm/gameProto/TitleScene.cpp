#include "TitleScene.h"
#include "Renderer.h"
#include "Texture.h"
#include "DebugGrid.h"
#include "CameraActor.h"
#include "PlayerBase.h"
#include "Player1.h"
#include "SelectScene.h"
#include "Input.h"
#include "GameScene.h"
#include "WeakEnemy.h"
#include "Weapon.h"
#include "ThirdPersonCamera.h"
#include "LevelActor.h"
#include "ScoreManager.h"

TitleScene::TitleScene()
	: mPlayer(nullptr)
	, mWeapon(nullptr)
	, mCamera(nullptr)
{
	printf("//////////////////\n");
	printf("//タイトルシーン//\n");
	printf("//////////////////\n");

	// プレイヤー生成
	mPlayer = new Player1();
	mPlayer->SetPosition(Vector3(0.0, -100.0f, 800.0f));
	Vector3 playerDir = Vector3(-0.5f, 0.5f, 0.0f);
	playerDir.Normalize();
	mPlayer->RotateToNewForward(playerDir);
	mPlayer->SetPlayerSceneState(PlayerSceneState::PLAYER_TITLESCENE);

	// 雑魚敵生成
	mEnemy = new WeakEnemy(mPlayer, Vector3(0.0f, 100.0f, 900.0f));
	Vector3 enemyDir = Vector3(-0.5f, -0.5f, 0.0f);
	enemyDir.Normalize();
	mEnemy->RotateToNewForward(enemyDir);
	mEnemy->SetEnemyStateScene(EnemyStateScene::ENEMY_SCENE_TITLE);

	// マップ読み込み
	mLevelActor = new LevelActor();
	Vector3 offset(0, 0, 0);
	mLevelActor->LoadLevel("Assets/Map/Stage.gpmesh", "", offset);
	mLevelActor->SetScale(4.5f);

	// 武器生成
	//mWeapon = new Weapon(mPlayer);
	//mWeapon->SetPosition(Vector3(0.0f, 0.0f, 300.0f));
	//mWeapon->SetScale(1.0f);

	// ライティング設定
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection = Vector3(0.7f, -0.7f, -0.7f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// プレイヤーの初期位置
	mPlayerCameraOffset = Vector3(100.0f, 0.0f, 200.0f);
	mEnemyCameraOffset = Vector3(100.0f, 0.0f, 100.0f);

	// カメラの追加
	mCamera = new CameraActor(mPlayer);
	mCamera->Init(Vector3(-150, 0, 1000), Vector3(0,0,950), Vector3(0,0,0));

	// テクスチャ追加
	mRogo = RENDERER->GetTexture("Assets/Image/Rogo.png");
	mPressButton = RENDERER->GetTexture("Assets/Image/PressAButton.png");

	// スコアの初期化
	ScoreManager::GetInstance()->Initialize();
}

TitleScene::~TitleScene()
{
	delete mPlayer;
	mPlayer = nullptr;

	delete mEnemy;
	mEnemy = nullptr;

	delete mLevelActor;
	mLevelActor = nullptr;

	//delete mRogo;
	//delete mPressButton;
	delete mCamera;

	printf("タイトルシーン終了");
}

SceneBase* TitleScene::update()
{
	// Aキーが押されたらゲーム画面へ遷移
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_A))
	{
		return new GameScene();
	}
	// デルタタイムを取得
	mTimer += GAMEINSTANCE.GetDeltaTime();

	// タイトル画面カメラワーク
	//Vector3 cameraPos;
	//if (mTimer <= 5.0f)
	//{
	//	cameraPos = Vector3::Lerp(mCamera->GetViewPos(), mPlayer->GetPosition() + mEnemyCameraOffset, 0.01f);
	//	mCamera->SetPosition(cameraPos);
	//}
	//else if (mTimer > 5.0f)
	//{
	//	mCamera->SetTarget(mEnemy->GetPosition());
	//	cameraPos = Vector3::Lerp(mCamera->GetViewPos(), mEnemy->GetPosition() + mPlayerCameraOffset, 0.01f);
	//	mCamera->SetPosition(cameraPos);
	//}

	float radius = 160.0f;
	static float anim = 0.0f;
	anim += 0.01f;
	Matrix4 view = GAMEINSTANCE.GetViewMatrix();
	GAMEINSTANCE.GetRenderer()->SetViewMatrix(view);

	return this;
}

void TitleScene::draw()
{
	// 背景色を設定
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// 画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();

	// レンダー描画
	GAMEINSTANCE.GetRenderer()->Draw();

	// スプライト描画
	RENDERER->SpriteDrawBegin();
	RENDERER->DrawTexture(mRogo, Vector2(RENDERER->GetScreenWidth()/2, 100));
	RENDERER->DrawTexture(mPressButton, Vector2(RENDERER->GetScreenWidth() / 2, RENDERER->GetScreenHeight()-100));
	RENDERER->SpriteDrawEnd();


	// 画面のフリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}
