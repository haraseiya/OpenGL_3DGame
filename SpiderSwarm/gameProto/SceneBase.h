#pragma once
#include "Game.h"
#include <functional>


// シーンベースクラス
class SceneBase
{
public:
// シーン状態
	enum class SceneState
	{
		SCENE_TITLE = 0,
		SCENE_SELECT,
		SCENE_GAME,
		SCENE_RESULT,
	};

	SceneBase() {};
	virtual      ~SceneBase() {};

	virtual       SceneBase*  update() = 0;    // シーンの更新処理（オーバーライド必須）
	virtual void  draw() = 0;                  // シーンの描画処理（オーバーライド必須）

	SceneState GetSceneState() { return mSceneState; }

protected:
	SceneState mSceneState;
};