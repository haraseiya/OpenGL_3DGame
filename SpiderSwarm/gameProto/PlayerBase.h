#pragma once

#include "GameObject.h"
#include "BoxCollider.h"


// �v���C���[�N���X���enum ��ԂɕύX����������statePlayerName���ύX
enum class PlayerStateEnum : unsigned char
{
	Invalid,    // �����ȃX�e�[�g
	Spawn,      // �o��
	Idle,       // �ҋ@���
	Walk,       // ����
	Turn,       // �^�[��
	Forward,	// �O�i
	Right,		// �E�ړ�
	Left,		// ���ړ�
	Backward,	// ���
	RunForward, // �O����
	RunRight,	// �E����
	RunLeft,	// ������
	RunBackward,// ��둖��
	LookAround, // ����
	Attack1,    // �U���P
	Attack2,    // �U���Q
	Attack3,    // �U���R
	GetDamage,  // �_���[�W�󂯂�
	Roar,       // �i����
	Stun,       // �C��
	Die,        // ���S
	Hold,		// �e���\����
	Victory,	// �����|�[�Y
	StateNum	// ��Ԃ̑���
};

// �V�[�����Ƃ̃v���C���[
enum class PlayerSceneState
{
	PLAYER_TITLESCENE = 0,	// �^�C�g���V�[��
	PLAYER_SELECTSCENE,		// �Z���N�g�V�[��
	PLAYER_GAMESCENE,		// �Q�[���V�[��
	PLAYER_RESULT,			// ���U���g�V�[��
};

// �v���C���[�̏��
enum class PlayerState
{
	PLAYER_STATE_IDLE = 0,		// �ҋ@
	PLAYER_STATE_HOLD,			// �\����
	PLAYER_STATE_FORWARD,		// �O�i
	PLAYER_STATE_RIGHT,			// �E�ړ�
	PLAYER_STATE_LEFT,			// ���ړ�
	PLAYER_STATE_BACKWARD,		// ���
	PLAYER_STATE_RUN_FORWARD,	// ����
	PLAYER_STATE_RUN_RIGHT,		// �E����
	PLAYER_STATE_RUN_LEFT,		// ������
	PLAYER_STATE_RUN_BACKWARD,	// ��둖��
	PLAYER_STATE_DIE,			// ���S
	PLAYER_STATE_VICTORY,		// �����|�[�Y
	PLAYER_STATE_NUM,			// ���A�j���[�V������
};

class SkeletalMeshComponent;
class Animation;
class BoxCollider;
class EffectComponent;
class PlayerStateBase;
class Mesh;
class PlayerBehaviorComponent;
class AttachMeshComponent;
class InvincibleComponent;

class PlayerBase : public GameObject
{
public:
	PlayerBase();
	~PlayerBase();

	virtual void UpdateActor(float deltaTime) = 0;

	SkeletalMeshComponent* GetSkeletalMeshComp();
	const Animation* GetAnim(PlayerState state);

	void OnCollisionEnter(ColliderComponent* own, ColliderComponent* other);

	// �L�������ǂݍ��݊֘A
	virtual void LoadModel() = 0;
	virtual void LoadSkeleton() = 0;
	virtual void LoadAnimation() = 0;

	// �ӂ�܂���ǉ�
	virtual void BehaviorResister() = 0;

	// �����蔻���ǉ�
	virtual void SetCollider() = 0;

	// �X�e�[�g�����擾����w���p�[�֐�
	const char* GetPlayerStateEnumName(PlayerStateEnum state);

	// HP�̃Q�b�^�[�Z�b�^�[
	int GetHitPoint() { return mHitPoint; }
	void SetHitPoint(int hp) { mHitPoint = hp; }

	bool GetDeadAnimFlag() { return mIsDeadAnimFinish; }
	void SetDeadAnimFlag(bool isDeadAnimFinish) { mIsDeadAnimFinish = isDeadAnimFinish; }

	void SetPlayerSceneState(PlayerSceneState state) { mPlayerSceneState = state; }

	void SetForwardVec(Vector3& v);

	const int& GetLevel() { return mLevel; }

	void SetVisivle(bool isVisible);

protected:
	AABB mPlayerBox;
	Mesh* mMesh;

	SkeletalMeshComponent* mMeshComp;
	std::vector<const class Animation*> mAnimTypes;
	BoxCollider* mHitBox;
	BoxCollider* mHitTrigger;
	BoxCollider* mHitGroundBox;
	BoxCollider* mHitHeadBox;
	BoxCollider* mAttackBox;	

	EffectComponent* mEffectComp;
	AttachMeshComponent* mAttachMeshComp;
	InvincibleComponent* mInvincibleComp;

	PlayerState mNowState;        // ���݂̃X�e�[�g
	PlayerState mNextState;       // ���̃X�e�[�g
	std::vector<PlayerStateBase*> mStatePools;      // �X�e�[�g�N���X�v�[��
	PlayerSceneState mPlayerSceneState;				// �v���C���[�̃V�[�����̏��

	PlayerBehaviorComponent* mPlayerBehavior;	// �v���C���[�̂ӂ�܂�

	Vector3 mVelocityVec;
	Vector3 mForwardVec;  // �O�i�����x�N�g��  
			
	static const float mAnimationSpeed;
	static const float mInterval;
	static const float mSpecialShotInterval;
	static const float mInvisibleInterval;	
	static const float mInvincibleTime;		// ���G����
	static const float mBulletSpeed;		// �e��
	static const float mBulletScale;

	float mShootTimer;
	float mSpecialShotTimer;
	float mInvincibleTimer;					// ���G���Ԃ��v��

	bool mAimMode;

	int mHitPoint;
	bool mIsDeadAnimFinish;

	int mLevel;							// ���x��
	int mExperience;					// �o���l
	static const int mRequireExperience;	// �K�v�o���l

	SceneBase::SceneState mSceneState;
};

