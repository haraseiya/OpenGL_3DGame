#include "NPCLookAround.h"

NPCLookAround::NPCLookAround(NPCBehaviorComponent* owner)
	: NPCState(owner)
{
	mStateType = NPCStateEnum::LookAround;
}

NPCLookAround::~NPCLookAround()
{

}

NPCStateEnum NPCLookAround::Update(float deltaTime)
{
	// アニメーション再生終わったら歩きモードに移行
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return NPCStateEnum::Walk;
	}

	// 続行
	return NPCStateEnum::LookAround;
}

void NPCLookAround::OnEnter()
{
	// 見渡しアニメ再生
	mOwnerActor->PlayAnimation(NPCStateEnum::LookAround);
}

void NPCLookAround::OnExit()
{
}