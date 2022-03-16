#include "EnemyChase.h"
#include "PlayerBase.h"
#include "NPCActorBase.h"
#include "GameScene.h"
#include "EnemyBullet.h"

const float EnemyChase::mRange = 10000.0f;
const float EnemyChase::mInterval = 2.0f;

EnemyChase::EnemyChase(EnemyBehaviorComponent* owner,GameObject* target)
	: EnemyState(owner)
	, mTarget(target)
	, mTime(0.0f)
{
	// �G�̃A�j���[�V������Ԃ𑖂�ɐݒ�
	mStateType = EnemyStateEnum::Run;
	mAttackInterval = Math::GetRandom(5.0f, 15.0f);
}

EnemyChase::~EnemyChase()
{
}

EnemyStateEnum EnemyChase::Update(float deltaTime)
{
	mDeltaTime = deltaTime;

	// �̗͂�0�ȉ��̏ꍇ���S�A�j���[�V�����֑J��
	if (mOwner->GetHitPoint() <= 0)
	{
		return EnemyStateEnum::Death;
	}

	// 5�b��������ːi��Ԃֈڍs
	mTime += deltaTime;
	if (mTime >= mAttackInterval)
	{
		mTime = 0.0f;
		//mAttackInterval = Math::GetRandom(5.0f, 15.0f);
		return EnemyStateEnum::Attack2;
	}

	// �^�[�Q�b�g�����݂��Ă��Ȃ����Idle��ԂɈڍs
	//const bool isIdle = !mTarget /*|| Math::Abs(mTarget->GetPosition().LengthSq() - mOwnerActor->GetPosition().LengthSq()) > mRange*/;
	//if (!mTarget)
	//{
	//	return EnemyStateEnum::Idle;
	//}

	// �L�����N�^�[�̑O�����擾
	Vector3 charaForwardVec = mOwner->GetForward();

	// �v���C���[�ւ̌��������߂�
	Vector3 enemyPos = mOwner->GetPosition();
	enemyPos.z = 750.0f;
	Vector3 playerPos = mTarget->GetPosition();
	Vector3 direction = playerPos- enemyPos;
	direction.Normalize();

	// ������x����Ă���ꍇ
	if (direction.LengthSq() > 0.5f)
	{
		// �v���C���[�̕����֌�����
		enemyPos += direction * 100.0f * deltaTime;
		mOwner->SetPosition(enemyPos);

		// �����L�[����
		charaForwardVec = Vector3::Lerp(mOwner->GetForward(), direction, 0.1f);

		// �i�s�����Ɍ����ĉ�]
		charaForwardVec.Normalize();

		mOwner->RotateToNewForward(charaForwardVec);
	}

	// �e��G���Ƃ̂ӂ�܂�
	WeakEnemyMove();
	StrongEnemyMove();
	BossEnemyMove();

	// �A�j���[�V�������s
	return EnemyStateEnum::Run;
}

void EnemyChase::OnEnter()
{
	// ����A�j���[�V�����Đ�
	mOwner->PlayAnimation(EnemyStateEnum::Run);
}

void EnemyChase::OnExit()
{
}

void EnemyChase::WeakEnemyMove()
{
	// 5�b�����Ƀv���C���[�Ɍ������Ēe����
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_WEAK)
	{
		mShootTimer += mDeltaTime;
		const bool isShot = mShootTimer > mInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			// �e����
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 1.0f, 200.0f);
		}
	}
}

void EnemyChase::StrongEnemyMove()
{
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_STRONG)
	{
		// 5�b�����Ƀv���C���[�Ɍ������Ēe�𔭎�
		mShootTimer += mDeltaTime;
		const bool isShot = mShootTimer > mInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			Vector3 upperRight = Vector3::UnitX + Vector3(0.0f, 0.6f, 0.0f);			// �E��
			Vector3 upperLeft = Vector3::UnitX + Vector3(0.0f, -0.6f, 0.0f);		// ����
			upperRight.Normalize();
			upperLeft.Normalize();

			// �G�e�̃C���X�^���X����
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperRight), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperLeft), 2.0f, 300.0f);
		}
	}
}

void EnemyChase::BossEnemyMove()
{
	if (mOwner->GetEnemyKind() == EnemyKind::ENEMY_BOSS)
	{
		// 5�b�����Ƀv���C���[�Ɍ������Ēe�𔭎�
		mShootTimer += mDeltaTime;
		const bool isShot = mShootTimer > mInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			Vector3 upperRight1 = Vector3::UnitX + Vector3(0.0f, 0.3f, 0.0f);			// �E��
			Vector3 upperLeft1 = Vector3::UnitX + Vector3(0.0f, -0.3f, 0.0f);			// ����
			Vector3 upperRight2 = Vector3::UnitX + Vector3(0.0f, 0.6f, 0.0f);			// �E��
			Vector3 upperLeft2 = Vector3::UnitX + Vector3(0.0f, -0.6f, 0.0f);			// ����
			upperRight1.Normalize();
			upperLeft1.Normalize();
			upperRight2.Normalize();
			upperLeft2.Normalize();

			// �G�e�̃C���X�^���X����
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 3.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperRight1), 3.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperLeft1), 3.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperRight2), 3.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperLeft2), 3.0f, 200.0f);
		}
	}
}
