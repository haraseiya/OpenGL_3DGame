#include "EnemyCharge.h"
#include "ExplosionEffect.h"
#include "EnemyBullet.h"

const float EnemyCharge::mWeakEnemyShotInterval = 1.5f;
const float EnemyCharge::mStrongEnemyShotInterval = 1.5f;
const float EnemyCharge::mBossEnemyShotInterval = 0.5f;
const float EnemyCharge::mChargeSpeed = 0.005f;

EnemyCharge::EnemyCharge(EnemyBehaviorComponent* owner)
	: EnemyState(owner)
{
	mStateType = EnemyStateEnum::Attack2;

	// �ːi�^�[�Q�b�g�̎�ނ�4�����ݒu
	mTargetPositions.emplace_back(Vector3(2000,2000,750));
	mTargetPositions.emplace_back(Vector3(-2000, 2000, 750));
	mTargetPositions.emplace_back(Vector3(-2000, -2000, 750));
	mTargetPositions.emplace_back(Vector3(2000, -2000, 750));
}

EnemyCharge::~EnemyCharge()
{
}

EnemyStateEnum EnemyCharge::Update(float deltaTime)
{
	mShootTimer += deltaTime;
	
	// �̗͂�0�ȉ��Ȃ玀�S��Ԃ֑J��
	const bool isDead = mOwner->GetHitPoint() <= 0;
	if (isDead)
	{
		return EnemyStateEnum::Death;
	}

	// �A�j���[�V�������I��������ҋ@��Ԃ֑J��
	if (!mOwner->IsAnimationPlaying())
	{
		return EnemyStateEnum::Idle;
	}

	// ���g�̎�ނ𔻕ʂ��e������
	mEnemyKind = mOwner->GetEnemyKind();
	CreateBarrage(mEnemyKind);


	// ���g�̈ʒu�Ǝ��Ɍ������ʒu���擾
	Vector3 enemyPos = mOwner->GetPosition();
	enemyPos.z = 750.0f;
	Vector3 direction = mTargetPos - enemyPos;
	direction.Normalize();
	Vector3 lerpDirection = Vector3::Lerp(mOwner->GetForward(), direction, 1.0f);

	// ���Ɍ������ʒu�Ɏ������ړ�������
	mOwner->SetPosition(Vector3::Lerp(enemyPos, mTargetPos, mChargeSpeed));
	mOwner->RotateToNewForward(lerpDirection);

	// �U����Ԃ�Ԃ�
	return EnemyStateEnum::Attack2;
}

// �ːi�A�j���[�V�����J�n��
void EnemyCharge::OnEnter()
{
	// �ːi�^�[�Q�b�g�������_���Ō��߂�
	mTargetPosNum = Math::GetRandom(0, 3);
	mTargetPos = mTargetPositions[mTargetPosNum];

	// �ːi�A�j���[�V�����Đ�
	mOwner->PlayAnimation(EnemyStateEnum::Attack2);
}

// �ːi�A�j���[�V�����I����
void EnemyCharge::OnExit()
{
}

// �G��ނɉ������e������
void EnemyCharge::CreateBarrage(EnemyKind enemyKind)
{
	// �G���K�p�e�������p�^�[��
	if (enemyKind==EnemyKind::ENEMY_WEAK)
	{
		const bool isShot = mShootTimer > mWeakEnemyShotInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			// �e����
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetBack(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetRight(), 1.0f, 200.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetLeft(), 1.0f, 200.0f);
		}
	}

	// ���G�p�e�������p�^�[��
	if (enemyKind == EnemyKind::ENEMY_STRONG)
	{
		// 5�b�����Ƀv���C���[�Ɍ������Ēe�𔭎�
		const bool isShot = mShootTimer > mStrongEnemyShotInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			Vector3 upperRight = Vector3::UnitX + Vector3::UnitY;		// �E��
			Vector3 upperLeft = Vector3::UnitX + Vector3::NegUnitY;		// ����
			Vector3 lowerRight = Vector3::NegUnitX + Vector3::UnitY;	// �E��
			Vector3 lowerLeft = Vector3::NegUnitX + Vector3::NegUnitY;	// ����
			upperRight.Normalize();
			upperLeft.Normalize();
			lowerRight.Normalize();
			lowerLeft.Normalize();

			// �G�e�̃C���X�^���X����
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetBack(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetRight(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetLeft(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperRight), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperLeft), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(lowerRight), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(lowerLeft), 2.0f, 300.0f);
		}
	}

	// �{�X�G�e�������p�^�[��
	if (enemyKind == EnemyKind::ENEMY_BOSS)
	{
		// 5�b�����Ƀv���C���[�Ɍ������Ēe�𔭎�
		const bool isShot = mShootTimer > mBossEnemyShotInterval;
		if (isShot)
		{
			mShootTimer = 0.0f;

			// �e�̕�������
			Vector3 upperRight = Vector3::UnitX + Vector3::UnitY;		// �E��
			Vector3 upperLeft = Vector3::UnitX + Vector3::NegUnitY;		// ����
			Vector3 lowerRight = Vector3::NegUnitX + Vector3::UnitY;	// �E��
			Vector3 lowerLeft = Vector3::NegUnitX + Vector3::NegUnitY;	// ����
			upperRight.Normalize();
			upperLeft.Normalize();
			lowerRight.Normalize();
			lowerLeft.Normalize();

			// �G�e�̃C���X�^���X����
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetForward(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetBack(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetRight(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetLeft(), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperRight), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(upperLeft), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(lowerRight), 2.0f, 300.0f);
			mEnemyBullet = new EnemyBullet(mOwner, mOwner->GetDirectionFromForward(lowerLeft), 2.0f, 300.0f);
		}
	}
}

