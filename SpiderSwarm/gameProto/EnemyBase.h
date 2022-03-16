#pragma once
#include "GameObject.h"
#include "SkeletalMeshComponent.h"
#include "collision.h"
#include <string>
#include <unordered_map>

// �G�̎��
enum class EnemyKind :unsigned char
{
	ENEMY_WEAK,
	ENEMY_STRONG,
	ENEMY_BOSS
};

enum class EnemyMovePattern
{
	PATTERN_TRACKING = 0,
	PATTERN_INTERCEPTION,
	PATTERN_ESCAPE,
};

enum class EnemyStateScene :unsigned char
{
	ENEMY_SCENE_TITLE = 0,
	ENEMY_SCENE_GAME
};

// �G�N���X���enum ��ԂɕύX����������stateEnumName���ύX
enum class EnemyStateEnum : unsigned char
{
	Invalid,    // �����ȃX�e�[�g
	Spawn,      // �o��
	Idle,       // �ҋ@���
	Walk,       // ����
	Turn,       // �^�[��
	Run,        // ����
	LookAround, // ����
	Attack1,    // �U���P
	Attack2,    // �U���Q
	Attack3,    // �U���R
	GetDamage,  // �_���[�W�󂯂�
	Roar,       // �i����
	Stun,       // �C��
	Death,        // ���S
	StateNum	// ��Ԃ̑���
};

// �G�g���K�[�{�b�N�X���
enum class EnemyTriggerEnum : unsigned char
{
	ForwardBox,          // ����
	LeftBox,             // ����
	RightBox,            // �E
	BackBox,             // ���
	HeadBox,             // ����
	GroundBox,           // ����
	GroundForwardBox     // �����O��
};

class BoxCollider;
class Animation;
class VertexArray;
class EnemyBullet;
class Score;

// �G�x�[�X�N���X
class EnemyBase : public GameObject
{
public:
	EnemyBase();
	virtual       ~EnemyBase() {};
	virtual void   UpdateActor(float deltaTime) = 0;
	bool           IsAnimationPlaying();
	void           PlayAnimation(EnemyStateEnum state, float rate = 0.5f);
	//void   OnCollisionEnter(ColliderComponent* other)override;
	virtual void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox) = 0;

	// �Z�b�^�[�n
	void SetVelocityVec(Vector3& v) { mVelocityVec = v; }				// ���x�x�N�g���Z�b�g
	void SetForwardVec(Vector3& v);									// �O�i�x�N�g���Z�b�g
	void SetWalkSpeed(float speed) { mWalkSpeed = speed; }			// �����X�s�[�h
	void SetTurnSpeed(float speed) { mTurnSpeed = speed; }			// ����X�s�[�h
	void SetRunSpeed(float speed) { mRunSpeed = speed; }				// ����X�s�[�h
	void SetHitPoint(int hitpoint) { mHitPoint = hitpoint; }			// �q�b�g�|�C���g�Z�b�g
	void SetIsActive(bool inUse) { mIsActive = inUse; }

	void SetTriggerBox(EnemyTriggerEnum trigerType, AABB& box);       // �g���K�[�{�b�N�X�̃Z�b�g
	void SetEnemyStateScene(EnemyStateScene enemyScene) { mEnemyStateScene = enemyScene; }

	void SetScore(int score) { mScore = score; }

	bool GetIsBossDeadFlag() { return mIsBossDead; }
	void SetIsBossDeadFlag(bool isDead) { mIsBossDead = isDead; }

	// �Q�b�^�[�n
	const Vector3&	GetVelocityVec()	const { return mVelocityVec; }	// ���x�x�N�g���̎擾
	const Vector3&	GetForwardVec()		const { return mForwardVec; }	// ���x�x�N�g���̎擾
	float			GetWalkSpeed()		const { return mWalkSpeed; }	// �����X�s�[�h�̎擾
	float			GetRunSpeed()		const { return mRunSpeed; }		// ����X�s�[�h�擾
	float			GetTurnSpeed()		const { return mTurnSpeed; }	// ����X�s�[�h�擾
	int				GetHitPoint()		const { return mHitPoint; }		// �̗͂̎擾
	EnemyKind		GetEnemyKind()		const { return mEnemyKind; }	// �G�̎�ގ擾
	EnemyStateScene GetEnemySceneState()const { return mEnemyStateScene; }
	bool GetIsActive() const { return mIsActive; }

	const EnemyStateEnum GetNowState()	const { return mNowState; }
	virtual void	SetAttackHitBox(float scale = 1.0f) = 0;

	bool IsHitTrigger(EnemyTriggerEnum type);             // TriggerEnum���q�b�g���Ă��邩�H
	bool IsExistTriggerBox(EnemyTriggerEnum type);        // TriggerBox���o�^����Ă��邩�H

	// �X�e�[�g�����擾����w���p�[�֐�
	const char* GetEnemyStateEnumName(EnemyStateEnum state);

	// �{�C�h�֘A
	Vector3 KeepDistance();					// ������ۂ�
	Vector3 MatchHeading();					// �擪�ɍ��킹��
	Vector3 SteerToCenter();				// ���S�ɂ�낤�Ƃ���
	float CanISee(EnemyBase* enemy);		// ���Ԃ������邩
	int SeeFriends(EnemyBase* firstBoid);	// ���Ԃ�T��
	void ClearVisibleList();				// �����Ă��钇�ԃ��X�g��������
	void AddToVisibleList(EnemyBase* enemy);// �����Ă��钇�Ԃ����X�g�ɒǉ�
	virtual void SetCollider() = 0;
	virtual void RemoveCollider() = 0;

	//virtual EnemyBase* Clone() = 0;		// �����֐�

protected:
	void           LoadAnimation(std::string& animationFileName, bool loop, EnemyStateEnum state);
	void           LoadSkeletalMesh(std::string& gpmeshFileName, std::string& gskelFilename);

	EnemyStateEnum mNowState;		// ���݂̃X�e�[�g
	EnemyKind		mEnemyKind;		// �G�̎��
	EnemyStateScene	mEnemyStateScene;	// �V�[�����̓G�̋���
	BoxCollider* mHitBox;			// ���g�̏Փ˔���
	EnemyBullet* mEnemyBullet;		// �G�̒e

	Vector3        mVelocityVec;	// ���x�x�N�g��
	Vector3        mForwardVec;		// �O�i�����x�N�g��  
	float          mWalkSpeed;		// �����X�s�[�h
	float          mRunSpeed;		// ����X�s�[�h
	float          mTurnSpeed;		// ����X�s�[�h
	int            mHitPoint;		// �̗�
	bool	mIsActive;					// ���̃I�u�W�F�N�g�͎g�p����
	SkeletalMeshComponent* mSkelMeshComponent; //���b�V���f�[�^
	Mesh* mMesh;

	std::unordered_map<EnemyStateEnum, const Animation*> mAnimations; // �A�j���[�V�����Z�b�g
	std::unordered_map<EnemyTriggerEnum, BoxCollider*>   mTrigerBoxs; // �g���K�[�{�b�N�X

	float mFieldOfView;
	const int mMaxFriendsVisible;
	float mShootTimer;

	int mScore;

	bool mIsBossDead;
};