#include "NPCAttack.h"
#include "EnemyBase.h"

NPCAttack::NPCAttack(NPCBehaviorComponent* owner, EnemyBase* enemy)
	: NPCState(owner)
{
	mStateType = NPCStateEnum::Attack1;
}

NPCAttack::~NPCAttack()
{
}

NPCStateEnum NPCAttack::Update(float deltaTime)
{
	// 攻撃モーションが終わったら追従モードに移行
	if (!mOwnerActor->IsAnimationPlaying()) 
	{
		return NPCStateEnum::Run; 
	}

	return NPCStateEnum::Attack1;
}

void NPCAttack::OnEnter()
{
	// 走りアニメ再生
	mOwnerActor->PlayAnimation(NPCStateEnum::Attack1);
}

void NPCAttack::OnExit()
{
}
