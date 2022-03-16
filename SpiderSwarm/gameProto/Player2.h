#pragma once
#include "PlayerBase.h"

class SkeletalMeshComponent;
class Animation;
class BoxCollider;
class EffectComponent;
class PlayerStateBase;

class Player2 : public PlayerBase
{
public:
	Player2();
	~Player2();

	virtual void UpdateActor(float deltaTime) override;
	void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox);

	SkeletalMeshComponent* GetSkeletalMeshComp();
	const Animation* GetAnim(PlayerState state);

	void OnCollisionEnter(ColliderComponent* own, ColliderComponent* other) override;

	// キャラ情報読み込み関連
	void LoadModel()override;
	void LoadSkeleton()override;
	void LoadAnimation()override;

	void BehaviorResister() override;

	void SetCollider() override;

private:
	// ターゲットオブジェクトを格納
	GameObject* mAimTarget;

	PlayerState mNowState;        // 現在のステート
	PlayerState mNextState;       // 次のステート
	std::vector<PlayerStateBase*> mStatePools;      // ステートクラスプール

	Vector3 mVelocityVec;

	static const float m_range;
	float mShootTimer;
	bool mAimMode;
	static const float mInterval;
};