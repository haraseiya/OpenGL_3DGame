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

	// パトロール用の現在向いている向きと逆向き方向を取得
	mAimDir = mOwnerActor->GetForward();
	mPrevAimDir = -1.0f * mAimDir;
}

NPCPatrol::~NPCPatrol()
{

}

NPCStateEnum NPCPatrol::Update(float deltaTime)
{
	// Actorの現在位置・向きを取得
	Vector3 pos, forward;
	pos = mOwnerActor->GetPosition();
	forward = mOwnerActor->GetForward();

	// ターン時行動
	if (mTurnMode)
	{
		// 前進方向をmAimDirに徐々に向ける
		Matrix4 rot = Matrix4::CreateRotationZ(mAngleSpeed * deltaTime);
		forward = Vector3::Transform(forward, rot);
		forward.Normalize();

		// 許容目標角度になったらmAimDirにしTurnmodeを解除
		const float epsiron = 0.03f;
		float cos = Vector3::Dot(forward, mAimDir);
		if (fabs(1.0f - cos) < epsiron)
		{
			forward = mAimDir;
			mTurnMode = false;
			mWalkRemainTime = mWalkTime;
			mTurnNum++;

			// 規定回数振り向いたら見渡し行動に移行
			if (mTurnNum >= mTurnMaxNum)
			{
				// 見渡し行動に移行
				mOwnerActor->SetForwardVec(forward);
				mOwnerActor->SetComputeWorldTransform();
				return NPCStateEnum::LookAround;
			}
		}
	}
	else
	{
		// 前進行動の残り時間算出し、経過したら向きを変更
		mWalkRemainTime -= deltaTime;
		if (mWalkRemainTime < 0.0f)
		{
			//進行方向を前回の目標方向に変更
			Vector3 tmpDir = mAimDir;
			mAimDir = mPrevAimDir;
			mPrevAimDir = tmpDir;
			mTurnMode = true;

			return mStateType;
		}
		// 位置・前進方向より、次の位置を算出
		pos += forward * mWalkSpeed * deltaTime;
	}

	// Actorの位置・向きの更新
	mOwnerActor->SetForwardVec(forward);
	mOwnerActor->SetPosition(pos);
	mOwnerActor->SetComputeWorldTransform();

	// 歩き行動継続
	return NPCStateEnum::Walk;
}

void NPCPatrol::OnEnter()
{
	// 歩きアニメーションスタート
	mOwnerActor->PlayAnimation(NPCStateEnum::Walk, 0.5f);
	mTurnNum = 0;
}

void NPCPatrol::OnExit()
{
}
