#include "PlayerStateIdle.h"
#include "PlayerBase.h"
#include "Input.h"
#include "InputController.h"
#include "SkeletalMeshComponent.h"
#include "PlayerBullet.h"

const float PlayerStateIdle::mInterval = 0.1f;

PlayerStateIdle::PlayerStateIdle(PlayerBehaviorComponent* owner)
	: PlayerStateBase(owner)
{
	mStateType = PlayerStateEnum::Idle;
}

PlayerStateIdle::~PlayerStateIdle()
{
}

// �A�C�h����Ԃ��瑼�̏�Ԃւ̈ڍs
PlayerStateEnum PlayerStateIdle::Update(float deltaTime)
{
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return PlayerStateEnum::Die;
	}

	// �X�e�B�b�N���͂���������
	mIsControllerInputOff = !(INPUT_INSTANCE.IsLStickMove());
	INPUT_INSTANCE.GetLStick();

	// �����L�[�����͂��ꂽ��
	mIsIdle = INPUT_INSTANCE.IsKeyOff(KEY_UP) &
		INPUT_INSTANCE.IsKeyOff(KEY_RIGHT) &
		INPUT_INSTANCE.IsKeyOff(KEY_DOWN) &
		INPUT_INSTANCE.IsKeyOff(KEY_LEFT) &
		mIsControllerInputOff;

	// �J��������݂��O�i�������擾
	Vector3 TargetPos = GAMEINSTANCE.GetViewTarget();
	Vector3 ViewPos = GAMEINSTANCE.GetViewPos();
	Vector3 forwardVec = TargetPos - ViewPos;
	forwardVec.z = 0.0; // ���������𖳎�

	// �J�����O���x�N�g���ƉE�����x�N�g���Z�o
	forwardVec = Vector3::Normalize(forwardVec);
	Vector3 rightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	float forwardAngle = 0.0f;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, rightVec));
	Vector2 stickR = INPUT_INSTANCE.GetRStick();
	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	stickR = Vector2::Transform(stickR, rot);

	Vector3 DirVecR(0.0f, 0.0f, 0.0f);
	DirVecR.x += stickR.x;
	DirVecR.y += stickR.y;

	Vector3 charaForwardVec = mOwner->GetForward(); // �L�����̑O�i�x�N�g��

	if (DirVecR.LengthSq() > 0.5f)
	{
		// �i�s�����Ɍ����ĉ�]
		charaForwardVec = Vector3::Lerp(mOwner->GetForward(), DirVecR, 0.2f);
		charaForwardVec.Normalize();
		mOwner->RotateToNewForward(charaForwardVec);
	}

	// �A�C�h����Ԃł͂Ȃ��ꍇ
	if (!mIsIdle)
	{
		return PlayerStateEnum::Forward;
	}
	Shot(deltaTime);
	return PlayerStateEnum::Idle;
}

// �A�C�h����Ԃւ̈ڍs����
void PlayerStateIdle::OnEnter()
{
	// �A�C�h����Ԃ̃A�j���[�V�����Đ�
	mMeshComp = mOwner->GetSkeletalMeshComp();
	mMeshComp->PlayAnimation(mOwner->GetAnim(PlayerState::PLAYER_STATE_IDLE));
}

void PlayerStateIdle::OnExit()
{
}

void PlayerStateIdle::Shot(float deltaTime)
{
	// �e�����Ă�Ȃ�
	mShootTimer += deltaTime;
	const bool isShoot = INPUT_INSTANCE.IsKeyPressed(KEY_R) && mShootTimer > mInterval;
	if (isShoot)
	{
		mShootTimer = 0.0f;

		if (mOwner->GetLevel() == 1)
		{
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetForward(), 1.0f, 1000.0f);
		}
		else if (mOwner->GetLevel() == 2)
		{
			Vector3 upperRight = Vector3(1.0f, 0.3f, 0.0f);
			Vector3 upperLeft = Vector3(1.0f, -0.3f, 0.0f);
			upperRight.Normalize();
			upperLeft.Normalize();
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetForward(), 1.0f, 1000.0f);
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetDirectionFromForward(upperRight), 1.0f, 1000.0f);
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetDirectionFromForward(upperLeft), 1.0f, 1000.0f);
		}
		else if (mOwner->GetLevel() >= 3)
		{
			Vector3 upperRight1 = Vector3(1.0f, 0.3f, 0.0f);
			Vector3 upperLeft1 = Vector3(1.0f, -0.3f, 0.0f);
			Vector3 upperRight2 = Vector3(1.0f, 0.5f, 0.0f);
			Vector3 upperLeft2 = Vector3(1.0f, -0.5f, 0.0f);
			upperRight1.Normalize();
			upperLeft1.Normalize();
			upperRight2.Normalize();
			upperLeft2.Normalize();

			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetForward(), 1.0f, 1000.0f);
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetDirectionFromForward(upperRight1), 1.0f, 1000.0f);
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetDirectionFromForward(upperRight2), 1.0f, 1000.0f);
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetDirectionFromForward(upperLeft1), 1.0f, 1000.0f);
			mBullet = new PlayerBullet(mOwner, mOwner->GetPosition(), mOwner->GetDirectionFromForward(upperLeft2), 1.0f, 1000.0f);
		}
	}
}
