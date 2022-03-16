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

	// �L�������ǂݍ��݊֘A
	void LoadModel()override;
	void LoadSkeleton()override;
	void LoadAnimation()override;

	void BehaviorResister() override;

	void SetCollider() override;

private:
	// �^�[�Q�b�g�I�u�W�F�N�g���i�[
	GameObject* mAimTarget;

	PlayerState mNowState;        // ���݂̃X�e�[�g
	PlayerState mNextState;       // ���̃X�e�[�g
	std::vector<PlayerStateBase*> mStatePools;      // �X�e�[�g�N���X�v�[��

	Vector3 mVelocityVec;

	static const float m_range;
	float mShootTimer;
	bool mAimMode;
	static const float mInterval;
};