#include "NPCHit.h"

NPCHit::NPCHit(NPCBehaviorComponent* owner)
	: NPCState(owner)
{
	mStateType = NPCStateEnum::GetDamage;
}

NPCHit::~NPCHit()
{
}

NPCStateEnum NPCHit::Update(float deltaTime)
{
	Vector3 pos, backward;
	pos = mOwnerActor->GetPosition();
	backward = -1.0f * mOwnerActor->GetForwardVec();

	if (!mOwnerActor->IsAnimationPlaying())
	{
		return NPCStateEnum::Walk;
	}

	//ノックバック移動
	const float speed = 10.0f;
	float percentage = mElapseTime / mTotalAnimTime;

	//経過割合をもとに移動処理
	pos += Math::EaseOut(percentage, speed, 0.0f) * backward;

	mOwnerActor->SetPosition(pos);

	return NPCStateEnum::GetDamage;
}

void NPCHit::OnEnter()
{
}

void NPCHit::OnExit()
{

}