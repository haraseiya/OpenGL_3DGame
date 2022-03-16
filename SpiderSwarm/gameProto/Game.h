#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_types.h>
#include "math.h"
#include "Tag.h"

class GameObjectManager;
class GameObject;
class SceneBase;
class Renderer;
class CameraActor;
class PhysicsWorld;
class AudioManager;
class FPSCounter;
class UIScreen;
class Font;

// ゲームの状態
enum class GameState
{
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_GAMECLEAR,
	SCENE_GAMEOVER,
	SCENE_MOVIE
};

class Game
{
private:
	Game();				// シングルトン

	void Input();		// 入力処理
	void Draw();        // 描画
	int  Update();      // フレーム更新処理

	// マネージャー関連
	AudioManager* mAudioManager;			// オーディオマネージャー
	GameObjectManager* mGameObjectManager;	// ゲームオブジェクトマネージャー

	Renderer* mRenderer;			// レンダリングエンジン
	SceneBase* mNowScene;           // ベースシーンクラス
	SceneBase* mTmpScene;			// シーン入れ替え用
	CameraActor* mActiveCamera;     // システムが描画に使うカメラ 
	PhysicsWorld* mPhysicsWorld;    // あたり判定システム

	bool  mIsRunning;               // ゲームループ回すか？
	bool  mIsPauseMode;             // ポーズモード

	float    mDeltaTime;	// 1フレームの経過時間（秒単位）
	Uint32   mTicksCount;	// 経過時間（ミリ秒単位）
	Matrix4  mViewMatrix;	// ビュー行列

	GameObject* mPlayerActor;	// プレイヤーアクター
	GameObject* mEnemyActor;	// 敵アクター
	FPSCounter* mFPSCounter;

public:
	~Game();
	static Game& GetInstance()
	{
		static Game instance;
		return instance;
	}

	void Run();                                                                    // ゲーム本体実行
	void Shutdown();                                                               // 終了時
	bool Initialize(int screenWidth, int screenHeight, bool fullScreen = false);   // 画面初期化設定

	void setFirstScene(SceneBase* pScene) { mNowScene = pScene; }                  // 開始シーンのセット
	void setShutDown() { mIsRunning = false; }                                     // シャットダウン

	Renderer* GetRenderer() { return mRenderer; }									// レンダラーシステムの取得
	SDL_Renderer* GetSDLRenderer();                                                // 2DのSDLレンダラーの取得

	//void AddActor(GameObject* actor);                                             // アクターの追加
	//void RemoveActor(GameObject* actor);                                          // アクターの削除

	void SetCameraActor(CameraActor* camera);      // カメラアクターをシステムに登録
	void InActiveCamera(CameraActor* inActiveCam); // カメラの登録を解除

	void SetPlayerActor(GameObject* player);              // プレイヤーアクターの登録

	Font* mFont;

	const Matrix4& GetViewMatrix();                                                      // 現在セットされているカメラのビュー行列を返す
	const Vector3& GetViewTarget();
	const Vector3& GetViewPos();
	const float    GetDeltaTime() { return mDeltaTime; }
	PhysicsWorld* const GetPhysics() { return mPhysicsWorld; }

	AudioManager* const GetAudio() { return mAudioManager; }

	//std::vector<class GameObject*>const& GetActors(Tag type);
	//class GameObject* GetFirstActor(Tag type);

	//bool IsExistActorType(Tag type);                    // そのアクタータイプは存在するか？
	//GameObject* FindActorFromID(int searchActorID); // アクターIDからアクターへのポインタを検索する
};

#define GAMEINSTANCE	Game::GetInstance()
#define RENDERER		Game::GetInstance().GetRenderer()
#define AUDIO			Game::GetInstance().GetAudio()

