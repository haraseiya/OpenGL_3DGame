// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "GameObject.h"
#include "GameObjectManager.h"
#include "Game.h"
#include "Component.h"
#include "ColliderComponent.h"

#include <algorithm>
#include <typeinfo>

// アクターのID
int GameObject::mGlobalActorNo = 0;

// 
GameObject::GameObject(Tag objectTag)
	: mTag(objectTag)
	, mState(GameObject::State::STATE_ACTIVE)
	, mPosition(Vector3::Zero)
	, mDirection(Vector3::UnitX)
	, mRotation(Quaternion::Identity)
	, mScale(1.0f)
	, mRecomputeWorldTransform(true)
	, mID(mGlobalActorNo)
	, mSpeed(0.0f)
{
	// ゲームオブジェクトマネージャーに追加
	GameObjectManager::GetInstance()->Entry(this);
	mGlobalActorNo++;
}

GameObject::~GameObject()
{
	// アクターが持っているコンポーネントをすべて削除
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}

	// ゲームシステム本体にこのアクターの削除を依頼
	GameObjectManager::GetInstance()->Exit(this);
}

void GameObject::Update(float deltaTime)
{
	// アクターが生きているときのみ処理
	if (mState == GameObject::State::STATE_ACTIVE)
	{
		// ワールド座標の再計算
		ComputeWorldTransform();

		// コンポーネント・アクターのアップデート
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		// もう1度ワールド座標の再計算
		ComputeWorldTransform();
	}
}

void GameObject::UpdateComponents(float deltaTime)
{
	// 全てのコンポーネントをアップデート
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}

	// 死亡コンポーネントを一時保管
	for (auto comp : mComponents)
	{
		if (comp->GetState() == Component::ComponentState::EDelete)
		{
			mDeleteComponents.emplace_back(comp);
		}
	}

	// 一時保管された死亡コンポーネントを削除
	for (auto comp : mDeleteComponents)
	{
		delete comp;
	}
	mDeleteComponents.clear();
}

// このアクター独自の更新処理 （必要ならオーバーライド）
void GameObject::UpdateActor(float deltaTime)
{
}

// このアクターが持っているコンポーネントの入力処理
void GameObject::ProcessInput()
{
	// アクティブ状態のコンポーネントを更新
	if (mState == GameObject::State::STATE_ACTIVE)
	{
		// 入力処理を受け取るコンポーネントを優先して実行
		for (auto comp : mComponents)
		{
			comp->ProcessInput();
		}
	}
}

// このアクターと他のアクターが持っているボックスとぶつかったとき（必要ならオーバーライド)
void GameObject::OnCollisionEnter(ColliderComponent* own,ColliderComponent* otherBox)
{
}

// forwardベクトルの向きに回転する
void GameObject::RotateToNewForward(const Vector3& forward)
{
	// X軸ベクトル(1,0,0)とforwardの間の角度を求める
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);

	// 下向きだった場合
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}

	// 上向きだった場合
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// 軸ベクトルとforwardとの外積から回転軸をもとめ、回転させる
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

// 補間をかけて回転
//void GameObject::RotateToNewForwardLerp(const Vector3& forward, float rotateSpeed)
//{
//	// X軸ベクトル(1,0,0)とforwardの間の角度を求める
//	float dot = Vector3::Dot(Vector3::UnitX, forward);
//	float angle = Math::Acos(dot);
//	// 下向きだった場合
//	if (dot > 0.9999f)
//	{
//		SetRotation(Quaternion::Identity);
//	}
//	// 上向きだった場合
//	else if (dot < -0.9999f)
//	{
//		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
//	}
//	else
//	{
//		// 軸ベクトルとforwardとの外積から回転軸をもとめ、回転させる
//		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
//		axis.Normalize();
//		SetRotation(Quaternion(axis, angle));
//	}
//}

// ワールド変換行列を計算
void GameObject::ComputeWorldTransform()
{
	//ワールド変換の再計算が必要なら実行
	if (mRecomputeWorldTransform)
	{

		mRecomputeWorldTransform = false;

		// スケーリング→回転→平行移動となるように変換行列を作成
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		// アクターが持っている全コンポーネントの変換を促す
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

// コンポーネントの追加
void GameObject::AddComponent(Component* component)
{
	// コンポーネントをソートして追加
	// 自分のオーダー番号よりも大きい挿入点を見つける
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();

	for (;iter != mComponents.end();++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	// 要素を見つけたポイントの手前に挿入する
	mComponents.insert(iter, component);
}

// コンポーネントの削除
void GameObject::RemoveComponent(Component* component)
{
	// コンポーネントを探して削除
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
