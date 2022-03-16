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

	// ���b�V���̃��[�h
	Mesh* mesh = RENDERER->GetMesh("assets/Mesh/SK_Priest.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// �e�N�X�`���̃��[�h
	RENDERER->GetTexture("assets/Texture/Healer.png");

	// �X�P���g���̃��[�h
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("assets/Mesh/SK_Priest.gpskel"));

	// �A�j���[�V�����̎擾 & �A�j���[�V�����z��ɃZ�b�g
	m_animTypes.resize(HEALER_ITEMNUM);
	m_animTypes[HEALER_IDLE] = RENDERER->GetAnimation("assets/Animation/Anim_staff_01_idle.gpanim", true);
	m_animTypes[HEALER_RUN] = RENDERER->GetAnimation("assets/Animation/Anim_staff_03_run.gpanim", true);
	mMeshComp->PlayAnimation(m_animTypes[HEALER_IDLE], 0.5f);

	m_mesh = new Mesh();
	m_collision = new Sphere(mPosition, 10.0f);

	// �����蔻��Z�b�g
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
	// �L�����N�^�[�̑O�����擾
	Vector3 charaForwardVec = GetForward();

	// �ǂ��̐w�c������
	switch (m_camp)
	{
	// �����w�c�Ȃ�
	case NONE:
		mMeshComp->PlayAnimation(m_animTypes[HEALER_IDLE], 0.5f);
		break;

	// �v���C���[�w�c�ɏ������Ă���ꍇ
	case PLAYER:
		// �v���C���[�ւ̌��������߂�
		m_direction = _player.GetPosition() - mPosition;
		m_direction.Normalize();

		// ������x����Ă���ꍇ
		if (m_direction.LengthSq() > 0.5f)
		{
			// �v���C���[�̕����֌�����
			mPosition += m_direction * m_speed;

			// �����L�[����
			charaForwardVec = m_direction;

			// �i�s�����Ɍ����ĉ�]
			charaForwardVec.Normalize();
			RotateToNewForward(charaForwardVec);
		}
		else
		{
			m_direction = Vector3(0, 0, 0);
		}

		mRecomputeWorldTransform = true;
		break;

		// �G�w�c�ɏ������Ă���ꍇ
	case ENEMY:
		// �G�ւ̌��������߂�
		m_direction = _enemy.GetPosition() - mPosition;
		m_direction.Normalize();

		// �G�̕����֌�����
		mPosition += m_direction * m_speed;
		SetPosition(mPosition);

		mMeshComp->PlayAnimation(m_animTypes[HEALER_RUN], 0.5f);
		break;
	}
}

