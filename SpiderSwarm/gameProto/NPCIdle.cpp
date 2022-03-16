#include "NPCIdle.h"
#include "Player.h"

NPCIdle::NPCIdle(NPCBehaviorComponent* owner,Player* player,EnemyBase* enemy)
	: NPCState(owner)
	, mPlayer(player)
	, mEnemy(enemy)
{
	mStateType = NPCStateEnum::Idle;
}

NPCIdle::~NPCIdle()
{
}

NPCStateEnum NPCIdle::Update(float deltaTime)
{
	// NPCとプレイヤーの距離を求める
	mNPCPos = mOwnerActor->GetPosition();
	mPlayerPos = mPlayer->GetPosition();
	mDistance = mNPCPos - mPlayerPos;

	// 離れているかどうか
	const bool isFar = Math::Abs(mDistance.x) > 150.0f && Math::Abs(mDistance.y) > 150.0f;
	// アニメーションが終了しているかどうか
	const bool isFinishAnimation = !mOwnerActor->IsAnimationPlaying();

	// 離れている又はアニメーションが終了した場合走りアニメーションに移行
	if (isFar || isFinishAnimation)
	{
		return NPCStateEnum::Run;
	}

	return NPCStateEnum::Idle;
}

void NPCIdle::OnEnter()
{
	// 待機アニメーション再生
	mOwnerActor->PlayAnimation(NPCStateEnum::Idle);
	printf("待機アニメーション\n");
}

void NPCIdle::OnExit()
{
}
