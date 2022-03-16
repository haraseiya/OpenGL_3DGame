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
	// �U�����[�V�������I�������Ǐ]���[�h�Ɉڍs
	if (!mOwnerActor->IsAnimationPlaying()) 
	{
		return NPCStateEnum::Run; 
	}

	return NPCStateEnum::Attack1;
}

void NPCAttack::OnEnter()
{
	// ����A�j���Đ�
	mOwnerActor->PlayAnimation(NPCStateEnum::Attack1);
}

void NPCAttack::OnExit()
{
}
