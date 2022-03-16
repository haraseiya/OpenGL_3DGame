#include "Healer.h"
#include "Player.h"
#include "Enemy.h"
#include "Texture.h"
#include "BoxCollider.h"
#include "Collision.h"
#include "Mesh.h"
#include "Renderer.h"
#include "SkeletalMeshComponent.h"
#include "Skeleton.h"
#include "Animation.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "Game.h"

Healer::Healer()
	: m_hp(30)
	, m_speed(2.0f)
	, m_camp(NONE)
	, m_state(WAIT)
{
	m_direction = Vector3::Zero;

	// メッシュのロード
	Mesh* mesh = RENDERER->GetMesh("assets/Mesh/SK_Priest.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// テクスチャのロード
	RENDERER->GetTexture("assets/Texture/Healer.png");

	// スケルトンのロード
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("assets/Mesh/SK_Priest.gpskel"));

	// アニメーションの取得 & アニメーション配列にセット
	m_animTypes.resize(HEALER_ITEMNUM);
	m_animTypes[HEALER_IDLE] = RENDERER->GetAnimation("assets/Animation/Anim_staff_01_idle.gpanim", true);
	m_animTypes[HEALER_RUN] = RENDERER->GetAnimation("assets/Animation/Anim_staff_03_run.gpanim", true);
	mMeshComp->PlayAnimation(m_animTypes[HEALER_IDLE], 0.5f);

	m_mesh = new Mesh();
	m_collision = new Sphere(mPosition, 10.0f);

	// あたり判定セット
	AABB npcBox = m_mesh->GetCollisionBox();
	m_hitBox = new BoxCollider(this, PhysicsWorld::EnumNPC);
	npcBox.mMin.x *= 0.6f;
	npcBox.mMin.y *= 0.6f;
	npcBox.mMax.x *= 0.6f;
	npcBox.mMax.y *= 0.6f;
	m_hitBox->SetObjectBox(npcBox);
}

Healer::~Healer()
{
}

void Healer::UpdateActor(float _deltaTime)
{
	if (m_hp <= 0) mState = GameObject::STATE_DEAD;
}

void Healer::Follow(Player& _player, WeakEnemy& _enemy)
{
	// キャラクターの前方を取得
	Vector3 charaForwardVec = GetForward();

	// どこの陣営所属か
	switch (m_camp)
	{
	// 所属陣営なし
	case NONE:
		mMeshComp->PlayAnimation(m_animTypes[HEALER_IDLE], 0.5f);
		break;

	// プレイヤー陣営に所属している場合
	case PLAYER:
		// プレイヤーへの向きを求める
		m_direction = _player.GetPosition() - mPosition;
		m_direction.Normalize();

		// ある程度離れている場合
		if (m_direction.LengthSq() > 0.5f)
		{
			// プレイヤーの方向へ向かう
			mPosition += m_direction * m_speed;

			// 方向キー入力
			charaForwardVec = m_direction;

			// 進行方向に向けて回転
			charaForwardVec.Normalize();
			RotateToNewForward(charaForwardVec);
		}
		else
		{
			m_direction = Vector3(0, 0, 0);
		}

		mRecomputeWorldTransform = true;
		break;

		// 敵陣営に所属している場合
	case ENEMY:
		// 敵への向きを求める
		m_direction = _enemy.GetPosition() - mPosition;
		m_direction.Normalize();

		// 敵の方向へ向かう
		mPosition += m_direction * m_speed;
		SetPosition(mPosition);

		mMeshComp->PlayAnimation(m_animTypes[HEALER_RUN], 0.5f);
		break;
	}
}

