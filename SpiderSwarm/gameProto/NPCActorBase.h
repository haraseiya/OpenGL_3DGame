#pragma once
#include "GameObject.h"
#include "SkeletalMeshComponent.h"
#include "collision.h"
#include <string>
#include <unordered_map>

// �G�N���X���enum��ԂɕύX����������stateEnumName���ύX
enum class NPCStateEnum : unsigned char
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
	Die,        // ���S
	StateNum
};

// �G�g���K�[�{�b�N�X���
enum class NPCTriggerEnum : unsigned char
{
	ForwardBox,          // ����
	LeftBox,             // ����
	RightBox,            // �E
	BackBox,             // ���
	HeadBox,             // ����
	GroundBox,           // ����
	GroundForwardBox     // �����O��
};

enum class NPCCampEnum :unsigned char
{
	Player,
	Enemy
};



// �X�e�[�g�����擾����w���p�[�֐�
const char* GetNPCStateEnumName(NPCStateEnum state);

// �G�x�[�X�N���X
class NPCActorBase : public GameObject
{
public:
	NPCActorBase();
	virtual       ~NPCActorBase() {};
	virtual void   UpdateActor(float deltaTime) = 0;
	bool           IsAnimationPlaying();
	float          PlayAnimation(NPCStateEnum state, float rate = 0.5f);
	//void   OnCollisionEnter(ColliderComponent* other)override;

	// �Z�b�^�[�n
	void	SetVelocityVec(Vector3& v) { mVelocityVec = v; }			// ���x�x�N�g��
	void	SetForwardVec(Vector3& v);									// �O�i�x�N�g��
	void    SetWalkSpeed(float speed) { mWalkSpeed = speed; }			// �����X�s�[�h
	void    SetTurnSpeed(float speed) { mTurnSpeed = speed; }			// ����X�s�[�h
	void    SetRunSpeed(float speed) { mRunSpeed = speed; }				// ����X�s�[�h
	void    SetHitPoint(int hitpoint) { mHitPoint = hitpoint; }			// �q�b�g�|�C���g
	void    SetIsOnGround(bool onGround) { mIsOnGround = onGround; }	// �ڒn��Ԃ̕ύX
	//void	SetNPCState(NPCStateEnum npcState) { mNowState = npcState; }// ���݂̏��
	void    SetTriggerBox(NPCTriggerEnum trigerType, AABB& box);		// �g���K�[�{�b�N�X�̃Z�b�g

	// �Q�b�^�[�n
	const Vector3& GetVelocityVec() const { return mVelocityVec; }	// ���x�x�N�g���̎擾
	const Vector3& GetForwardVec()  const { return mForwardVec; }	// ���x�x�N�g���̎擾
	float          GetWalkSpeed()   const { return mWalkSpeed; }	// �����X�s�[�h�̎擾
	float          GetRunSpeed()    const { return mRunSpeed; }		// ����X�s�[�h�擾
	float          GetTurnSpeed()   const { return mTurnSpeed; }	// ����X�s�[�h�擾
	int            GetHitPoint()    const { return mHitPoint; }		// �̗͂̎擾
	bool           GetIsOnGround()  const { return mIsOnGround; }	// �ڒn��Ԃ̎擾
	//NPCStateEnum GetNPCBehaviorState() const { return mNowState; }	// ���݂̏��

	bool           IsHitTrigger(NPCTriggerEnum type);             // TriggerEnum���q�b�g���Ă��邩�H
	bool           IsExistTriggerBox(NPCTriggerEnum type);        // TriggerBox���o�^����Ă��邩�H

	void HitEnemyAttack(const int& power);

protected:
	void           LoadAnimation(std::string& animationFileName, bool loop, NPCStateEnum state);
	void           LoadSkeletalMesh(std::string& gpmeshFileName, std::string& gskelFilename);

	Vector3 mVelocityVec;	// ���x�x�N�g��
	Vector3 mForwardVec;	// �O�i�����x�N�g��  
	Vector3 mAcceleration;	// �����x

	float mWalkSpeed;		// �����X�s�[�h
	float mRunSpeed;		// ����X�s�[�h
	float mTurnSpeed;		// ����X�s�[�h
	int mHitPoint;			// �̗�
	bool mIsOnGround;		// �n�ʂɂ���H

	SkeletalMeshComponent* mSkelMeshComponent;								//���b�V���f�[�^
	std::unordered_map<NPCStateEnum, const class Animation*> mAnimations;	// �A�j���[�V�����Z�b�g
	std::unordered_map<NPCTriggerEnum, class BoxCollider*>   mTrigerBoxs;	// �g���K�[�{�b�N�X
};