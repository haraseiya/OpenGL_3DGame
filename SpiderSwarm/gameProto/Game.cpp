#include "game.h"
#include "Input.h"
#include "renderer.h"
#include "SceneBase.h"
#include "GameObject.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "Animation.h"
#include "Skeleton.h"
#include "CameraActor.h"
#include "PhysicsWorld.h"
#include "InstanceMeshManager.h"
#include "FPSCounter.h"
#include "UIManager.h"
#include "ScoreManager.h"
#include "Font.h"
#include "GameObjectManager.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <typeinfo>
#include <iostream>

Game::Game()
	: mRenderer(nullptr)
	, mNowScene(nullptr)
	, mIsRunning(true)
	, mIsPauseMode(false)
	, mActiveCamera(nullptr)
	, mPhysicsWorld(nullptr)
	, mFont(nullptr)
	, mGameObjectManager(nullptr)
	, mAudioManager(nullptr)
	, mPlayerActor(nullptr)
	, mEnemyActor(nullptr)
	, mFPSCounter(nullptr)
	, mTmpScene(nullptr)
	, mTicksCount(0)
	, mDeltaTime(0.0f)
{
}

Game::~Game()
{
}

void Game::Input()
{
	// 終了イベントのキャッチ
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			// パッドイベント取得
			INPUT_INSTANCE.GetControllerInstance()->ReceiveEvent(event);
		}
	}

	// キーボード入力更新
	INPUT_INSTANCE.Update();

	// ESCが押されたら終了
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_ESCAPE))
	{
		mIsRunning = false;
	}

	// アクターデバッグ
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_F12))
	{
	/*	ShowActor();*/
	}

	// レンダリングリソース表示
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_F11))
	{
		GetRenderer()->ShowResource();
	}

	// ポーズモード移行／解除
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_BACKSPACE))
	{
		////ポーズモード切替
		//mIsPauseMode = !mIsPauseMode;
		//GameObject::State changeState;
		//if (mIsPauseMode)
		//{
		//	changeState = GameObject::STATE_PAUSED; //ポーズ
		//}
		//else
		//{
		//	changeState = GameObject::STATE_ACTIVE; //アクティブ
		//}

		////全てのステートを変更する
		//for (auto tag = Tag::BEGIN; tag != Tag::END; ++tag)
		//{
		//	for (auto itr : mActors[tag])
		//	{
		//		if (itr->GetState() != GameObject::STATE_DEAD)
		//		{
		//			itr->SetState(changeState);
		//		}
		//	}
		//}
	}
}

void Game::Draw()
{
	if (!mNowScene)
	{
		return;
	}
	mNowScene->draw();
}

int Game::Update()
{
	// 16ミリ秒(= 60フレーム/秒)になるように、前のフレームからの16ミリ秒以上経過するまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// 前のフレームから現在時刻までの経過時間算出(秒単位）
	mDeltaTime = (SDL_GetTicks() - mTicksCount + 16) / 1000.0f;

	//このフレームの開始時間保存（次のフレームまでの経過時間をはかるため）
	mTicksCount = SDL_GetTicks();

	// フレーム時間があまりにも経過している場合は0.05 → 20fps固定
	if (mDeltaTime > 0.10f)
	{
		mDeltaTime = 0.10f;
	}

	if (!mNowScene)
	{
		return 0;
	}

	// アクターの処理
	GameObjectManager::GetInstance()->Update(mDeltaTime);
	//ActorUpdate();

	RENDERER->GetInstanceMeshManager()->PreparationBufferMatrices();

	//あたり判定
	mPhysicsWorld->Collision();

	SceneBase* tmpScene;

	if (mActiveCamera != nullptr)
	{
		// カメラ行列計算
		mActiveCamera->Update(mDeltaTime);

		// アクティブカメラのビュー行列を持ってくる
		mViewMatrix = mActiveCamera->GetViewMatrix();
		GetRenderer()->SetViewMatrix(mViewMatrix);
	}
	//シーンを実行
	tmpScene = mNowScene->update();

	if (tmpScene != mNowScene)
	{
		delete mNowScene;

		//新規シーンを今のシーンに差し替え
		mNowScene = tmpScene;
		return 1;
	}
	return 0;
}

void Game::Run()
{
	// レンダラーが初期化されていなかったら初期化
	if (!mRenderer)
	{
		Initialize(1920, 1080);
	}
	// 開始シーンが定義されていなかったら終了
	if (!mNowScene)
	{
		printf("開始シーンが設定されていません");
		return;
	}

	// ゲームループ
	while (mIsRunning)
	{
		Input();
		if (Update())
		{
			continue; // シーン切り替え時のみ描画処理をさせないため
		}
		Draw();
	}
}

void Game::Shutdown()
{
	GameObjectManager::GetInstance()->Finalize();
	GameObjectManager::Destroy();

	// マネージャーの破棄
	UIManager::DeleteInstance();
	ScoreManager::RemoveInstance();

	// フォントの終了処理
	mFont->Finalize();
}

bool Game::Initialize(int screenWidth, int screenHeight, bool fullScreen)
{
	// SDL初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("SDL初期化失敗: %s\n", SDL_GetError());
		return false;
	}

	// レンダラーの作成
	mRenderer = new Renderer;
	if (!mRenderer->Initialize(screenWidth, screenHeight, fullScreen))
	{
		printf("Rendererの初期化に失敗");
		delete mRenderer;
		return false;
	}

	// サウンドの初期化
	if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		delete mRenderer;
		return false;
	}
	printf("SDLMixer初期化完了\n");

	// SDLMixer API初期化　44100:音源の周波数 2:ステレオ 4096:内部バッファサイズ
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		delete mRenderer;
		Mix_CloseAudio();
		Mix_Quit();
		return false;
	}

	int decordNum = Mix_GetNumMusicDecoders();
	for (int i = 0; i < decordNum; ++i)
	{
		printf("MusicDecorder %d : %s\n", i, Mix_GetMusicDecoder(i));
	}

	int chunkNum = Mix_GetNumChunkDecoders();
	for (int i = 0; i < chunkNum; ++i)
	{
		printf("SoundDecorder %d : %s\n", i, Mix_GetChunkDecoder(i));
	}
	printf("SDLMixerAPI初期化完了\n");

	mFPSCounter = new FPSCounter(60);
	// あたり判定システム作成
	mPhysicsWorld = new PhysicsWorld;

	// コントローラ初期化
	INPUT_INSTANCE.ControllerInit();

	mTicksCount = SDL_GetTicks();

	// マネージャー生成
	UIManager::CreateInstance();
	ScoreManager::CreateInstance();
	GameObjectManager::CreateInstance();

	// フォントの初期化
	mFont->Initialize();

	return true;
}

SDL_Renderer* Game::GetSDLRenderer()
{
	return mRenderer->GetSDLRenderer();
}

//void Game::AddActor(GameObject* actor)
//{
//	// いったんペンディングアクターに追加
//	mPendingGameObjects.emplace_back(actor);
//}
//
//void Game::RemoveActor(GameObject* actor)
//{
//	//ペンディングアクター内にいる？
//	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
//	if (iter != mPendingActors.end())
//	{
//		//ペンディングアクターの最後尾にデータを移動して、データを消す
//		std::iter_swap(iter, mPendingActors.end() - 1);
//		mPendingActors.pop_back();
//		return;
//	}
//
//	// アクティブアクター内にいる？
//	auto tag = actor->GetTag();
//	iter = std::find(mActors[tag].begin(), mActors[tag].end(), actor);
//	if (iter != mActors[tag].end())
//	{
//		//アクティブアクターの最後尾にデータを移動して、データ消す
//		std::iter_swap(iter, mActors[tag].end() - 1);
//		mActors[tag].pop_back();
//	}
//}

void Game::SetCameraActor(CameraActor* camera)
{
	printf("SetCamera [%p]\n", camera);
	mActiveCamera = camera;
}

void Game::InActiveCamera(CameraActor* inActiveCam)
{
	if (inActiveCam == mActiveCamera)
	{
		printf("Camera is inActive. change to default view.\n");
		mActiveCamera = nullptr;
	}

	mViewMatrix =
		Matrix4::CreateLookAt(Vector3(0, 0, 100),
			Vector3(0, 0, 0),
			Vector3(0, 0, 1));
}

void Game::SetPlayerActor(GameObject* player)
{
	mPlayerActor = player;
}

//GameObject* Game::GetPlayerActor()
//{
//	return GetFirstActor(Tag::PLAYER);
//}
//
//GameObject* Game::GetEnemyActor()
//{
//	return GetFirstActor(Tag::ENEMY);
//}

const Matrix4& Game::GetViewMatrix()
{
	return mViewMatrix;
}

const Vector3& Game::GetViewTarget()
{
	if (mActiveCamera == nullptr)
	{
		printf("Camera is inActive. return IllegalVec\n");
	}
	return mActiveCamera->GetViewTarget();
}

const Vector3& Game::GetViewPos()
{
	if (mActiveCamera == nullptr)
	{
		printf("Camera is inActive. return IllegalVec\n");
	}
	return mActiveCamera->GetViewPos();
}

//std::vector<class GameObject*> const& Game::GetActors(Tag type)
//{
//	return mActors[type];
//}

//GameObject* Game::GetFirstActor(Tag type)
//{
//	// アクティブリストをチェック
//	if (mActors[type].size() != 0)
//	{
//		return mActors[type][0];
//	}
//
//	// ペンディングにいないかを検索
//	for (auto iter = mPendingActors.begin(); iter != mPendingActors.end(); ++iter)
//	{
//		if ((*iter)->GetTag() == type)
//		{
//			return *iter;
//		}
//	}
//
//	return nullptr;
//}

//bool Game::IsExistActorType(Tag type)
//{
//	return mActors[type].size() != 0;
//}

//GameObject* Game::FindActorFromID(int searchActorID)
//{
//	// アクティブリスト内から検索
//	for (Tag t = Tag::BEGIN; t != Tag::END; ++t)
//	{
//		for (auto item : mActors[t])
//		{
//			if (item->GetID() == searchActorID)
//			{
//				return item;
//			}
//		}
//	}
//	// ペンディングにいないかを検索
//	for (auto iter = mPendingActors.begin(); iter != mPendingActors.end(); ++iter)
//	{
//		if ((*iter)->GetID() == searchActorID)
//		{
//			return *iter;
//		}
//	}
//	return nullptr;
//}

