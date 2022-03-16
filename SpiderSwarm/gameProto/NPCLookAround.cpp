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
	// �A�j���[�V�����Đ��I�������������[�h�Ɉڍs
	if (!mOwnerActor->IsAnimationPlaying())
	{
		return NPCStateEnum::Walk;
	}

	// ���s
	return NPCStateEnum::LookAround;
}

void NPCLookAround::OnEnter()
{
	// ���n���A�j���Đ�
	mOwnerActor->PlayAnimation(NPCStateEnum::LookAround);
}

void NPCLookAround::OnExit()
{
}