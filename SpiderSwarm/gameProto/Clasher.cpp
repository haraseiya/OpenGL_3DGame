#include "Clasher.h"
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

const float animationSpeed = 0.5f;

Clasher::Clasher()
	: m_hp(50)
	, m_speed(5.0f)
	, m_camp(NONE)
	, m_state(WAIT)
	, m_animState(CLASHER_IDLE)
{
	// パラーメーター設定
	m_direction = Vector3::Zero;

	// メッシュセット
	Mesh* mesh = RENDERER->GetMesh("assets/Mesh/SK_Mannequin.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// テクスチャのロード
	RENDERER->GetTexture("Assets/Texture/Clasher.png");

	// スケルトンのロード
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("assets/Mesh/SK_Mannequin.gpskel"));

	// アニメーションの取得 & アニメーション配列にセット
	m_animTypes.resize(CLASHER_ITEMNUM);
	m_animTypes[CLASHER_IDLE] = RENDERER->GetAnimation("assets/Animation/ThirdPersonIdle.gpanim", true);
	m_animTypes[CLASHER_RUN] = RENDERER->GetAnimation("assets/Animation/ThirdPersonRun.gpanim", true);
	mMeshComp->PlayAnimation(m_animTypes[CLASHER_RUN], 0.5f);

	// メッシュ読み込み
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

void Clasher::UpdateActor(float _deltaTime)
{
	if (m_hp <= 0) mState = GameObject::STATE_DEAD;
}

void Clasher::Follow(Player& _player, WeakEnemy& _enemy)
{
	// キャラクターの前方を取得
	Vector3 charaForwardVec = GetForward();

	// 現在のアニメーション状態
	switch (m_animState)
	{

	case CLASHER_IDLE:
		if (m_direction.LengthSq() > 0.5f)
		{
			if (m_animState != CLASHER_IDLE)
			{
				m_animState = CLASHER_RUN;
				mMeshComp->PlayAnimation(m_animTypes[CLASHER_IDLE], animationSpeed);
			}
		}
		break;

	case CLASHER_RUN:
		if (m_direction.LengthSq() <= 0.5f)
		{
			if (m_animState != CLASHER_RUN)
			{
				m_animState = CLASHER_IDLE;
				mMeshComp->PlayAnimation(m_animTypes[CLASHER_RUN], animationSpeed);
			}
		}
		break;
	}

	// どこの陣営所属か
	switch (m_camp)
	{
	// 所属陣営なし
	case NONE:
		m_animState = CLASHER_IDLE;
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

		mMeshComp->PlayAnimation(m_animTypes[CLASHER_RUN], animationSpeed);
		break;
	}
}


