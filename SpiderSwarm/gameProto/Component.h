// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <cstdint>

class GameObject;

class Component
{
public:
	enum class ComponentState
	{
		EActive,
		EPaused,
		EDelete
	};

	Component(GameObject* owner, int updateOrder = 100);                    // コンストラクタ updateOderが小さいと早く実行される
	virtual       ~Component();                                             // デストラクタ
	virtual void   Update(float deltaTime);                                 // デルタタイムによるコンポーネントの更新 (オーバーライド可）
	virtual void   ProcessInput() {}                                        // コンポーネントの入力処理（オーバーライド可）
	virtual void   OnUpdateWorldTransform() { }								// ワールド変換が必要な時に呼ばれる関数（オーバーライド可）
	int            GetID() { return mID; }
	GameObject* GetOwner() { return mOwner; }                               // オーナーアクターの取得
	int            GetUpdateOrder() const { return mUpdateOrder; }          // update orderの取得
	void           SetState(ComponentState newState) { mState = newState; }
	ComponentState GetState() { return mState; }

protected:
	GameObject* mOwner;             // オーナーアクターへのポインタ
	int            mUpdateOrder;    // コンポーネントの更新順序
	int            mID;				// コンポーネントの管理ID
	ComponentState mState;			// 
	static int     mGlobalID;       // コンポーネントの管理IDの連番用
};
