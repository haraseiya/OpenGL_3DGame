#include "NPCActorBase.h"
#include "NPCPatrol.h"

#include <iostream>

NPCPatrol::NPCPatrol(NPCBehaviorComponent* owner)
	: NPCState(owner)
	, mTurnMode(false)
	, mAimDir(0.0f, 0.0f, 0.0f)
	, mAngleSpeed(0.0f)
	, mTurnNum(0)
{
	mStateType = NPCStateEnum::Walk;
	mWalkSpeed = mOwnerActor->GetWalkSpeed();
	mAngleSpeed = mOwnerActor->GetTurnSpeed();
	mWalkRemainTime = mWalkTime;

	// �p�g���[���p�̌��݌����Ă�������Ƌt�����������擾
	mAimDir = mOwnerActor->GetForward();
	mPrevAimDir = -1.0f * mAimDir;
}

NPCPatrol::~NPCPatrol()
{

}

NPCStateEnum NPCPatrol::Update(float deltaTime)
{
	// Actor�̌��݈ʒu�E�������擾
	Vector3 pos, forward;
	pos = mOwnerActor->GetPosition();
	forward = mOwnerActor->GetForward();

	// �^�[�����s��
	if (mTurnMode)
	{
		// �O�i������mAimDir�ɏ��X�Ɍ�����
		Matrix4 rot = Matrix4::CreateRotationZ(mAngleSpeed * deltaTime);
		forward = Vector3::Transform(forward, rot);
		forward.Normalize();

		// ���e�ڕW�p�x�ɂȂ�����mAimDir�ɂ�Turnmode������
		const float epsiron = 0.03f;
		float cos = Vector3::Dot(forward, mAimDir);
		if (fabs(1.0f - cos) < epsiron)
		{
			forward = mAimDir;
			mTurnMode = false;
			mWalkRemainTime = mWalkTime;
			mTurnNum++;

			// �K��񐔐U��������猩�n���s���Ɉڍs
			if (mTurnNum >= mTurnMaxNum)
			{
				// ���n���s���Ɉڍs
				mOwnerActor->SetForwardVec(forward);
				mOwnerActor->SetComputeWorldTransform();
				return NPCStateEnum::LookAround;
			}
		}
	}
	else
	{
		// �O�i�s���̎c�莞�ԎZ�o���A�o�߂����������ύX
		mWalkRemainTime -= deltaTime;
		if (mWalkRemainTime < 0.0f)
		{
			//�i�s������O��̖ڕW�����ɕύX
			Vector3 tmpDir = mAimDir;
			mAimDir = mPrevAimDir;
			mPrevAimDir = tmpDir;
			mTurnMode = true;

			return mStateType;
		}
		// �ʒu�E�O�i�������A���̈ʒu���Z�o
		pos += forward * mWalkSpeed * deltaTime;
	}

	// Actor�̈ʒu�E�����̍X�V
	mOwnerActor->SetForwardVec(forward);
	mOwnerActor->SetPosition(pos);
	mOwnerActor->SetComputeWorldTransform();

	// �����s���p��
	return NPCStateEnum::Walk;
}

void NPCPatrol::OnEnter()
{
	// �����A�j���[�V�����X�^�[�g
	mOwnerActor->PlayAnimation(NPCStateEnum::Walk, 0.5f);
	mTurnNum = 0;
}

void NPCPatrol::OnExit()
{
}
