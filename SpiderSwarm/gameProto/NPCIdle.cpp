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
	// NPC�ƃv���C���[�̋��������߂�
	mNPCPos = mOwnerActor->GetPosition();
	mPlayerPos = mPlayer->GetPosition();
	mDistance = mNPCPos - mPlayerPos;

	// ����Ă��邩�ǂ���
	const bool isFar = Math::Abs(mDistance.x) > 150.0f && Math::Abs(mDistance.y) > 150.0f;
	// �A�j���[�V�������I�����Ă��邩�ǂ���
	const bool isFinishAnimation = !mOwnerActor->IsAnimationPlaying();

	// ����Ă��閔�̓A�j���[�V�������I�������ꍇ����A�j���[�V�����Ɉڍs
	if (isFar || isFinishAnimation)
	{
		return NPCStateEnum::Run;
	}

	return NPCStateEnum::Idle;
}

void NPCIdle::OnEnter()
{
	// �ҋ@�A�j���[�V�����Đ�
	mOwnerActor->PlayAnimation(NPCStateEnum::Idle);
	printf("�ҋ@�A�j���[�V����\n");
}

void NPCIdle::OnExit()
{
}
