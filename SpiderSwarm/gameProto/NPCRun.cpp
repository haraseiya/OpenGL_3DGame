#include "NPCRun.h"
#include "Player.h"
#include "EnemyBase.h"
#include "GameScene.h"
#include "Input.h"

NPCRun::NPCRun(NPCBehaviorComponent* owner, Player* player, EnemyBase* enemy)
	: NPCState(owner)
	, m_mode(Mode::Chase)
	, m_player(player)
	, m_enemy(enemy)
	, m_speed(250.0f)
	, mRange(Vector3(100.0f,100.0f,0.0f))
{
	mStateType = NPCStateEnum::Run;
}

NPCRun::~NPCRun()
{
}

NPCStateEnum NPCRun::Update(float deltaTime)
{
	// プレイヤーへの向きを求める
	m_npcPos = mOwnerActor->GetPosition();
	m_playerPos = m_player->GetPosition();
	mDistance = m_npcPos - m_playerPos;

	// プレイヤーとNPCの距離が近ければ
	const bool isNear = Math::Abs(mDistance.x) <= 150.0f && Math::Abs(mDistance.y) <= 150.0f;
	if (isNear)
	{
		return NPCStateEnum::Idle;
	}

	// Xボタンが押されたらアタックモードに
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_X)) m_mode = Mode::Attack;

	// Yボタンが押されたら追従モードに
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_Y)) m_mode = Mode::Chase;

	// キャラクターの前方を取得
	m_npcForwardVec = mOwnerActor->GetForward();

	// モード別のふるまい
	switch (m_mode)
	{
	// 追従モード
	case Mode::Chase:

		// プレイヤーへの位置を求める
		m_direction = m_playerPos - m_npcPos;
		m_direction.Normalize();

		// プレイヤーの方向へ向かう
		m_npcPos += m_direction * m_speed * deltaTime;
		mOwnerActor->SetPosition(m_npcPos);

		// 進行方向に向けて回転
		m_npcForwardVec = m_direction;
		m_npcForwardVec.Normalize();
		mOwnerActor->RotateToNewForward(m_npcForwardVec);

		break;

	// 攻撃モード
	case Mode::Attack:
		// 敵への向きを取得
		m_enemyPos = m_enemy->GetPosition();
		m_direction = m_enemyPos - m_npcPos;
		m_direction.Normalize();

		// 敵の方向へ向かう
		m_npcPos += m_direction * m_speed * deltaTime;
		mOwnerActor->SetPosition(m_npcPos);

		// 進行方向に向けて回転
		m_npcForwardVec = m_direction;
		m_npcForwardVec.Normalize();
		mOwnerActor->RotateToNewForward(m_npcForwardVec);
		break;
	}

	// 続行
	return NPCStateEnum::Run;
}

void NPCRun::OnEnter()
{
	printf("走りアニメーションEnter\n");

	// 走りアニメ再生
	mOwnerActor->PlayAnimation(NPCStateEnum::Run);
}

void NPCRun::OnExit()
{
}