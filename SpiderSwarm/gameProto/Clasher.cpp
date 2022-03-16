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
	// �p���[���[�^�[�ݒ�
	m_direction = Vector3::Zero;

	// ���b�V���Z�b�g
	Mesh* mesh = RENDERER->GetMesh("assets/Mesh/SK_Mannequin.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// �e�N�X�`���̃��[�h
	RENDERER->GetTexture("Assets/Texture/Clasher.png");

	// �X�P���g���̃��[�h
	mMeshComp->SetSkeleton(RENDERER->GetSkeleton("assets/Mesh/SK_Mannequin.gpskel"));

	// �A�j���[�V�����̎擾 & �A�j���[�V�����z��ɃZ�b�g
	m_animTypes.resize(CLASHER_ITEMNUM);
	m_animTypes[CLASHER_IDLE] = RENDERER->GetAnimation("assets/Animation/ThirdPersonIdle.gpanim", true);
	m_animTypes[CLASHER_RUN] = RENDERER->GetAnimation("assets/Animation/ThirdPersonRun.gpanim", true);
	mMeshComp->PlayAnimation(m_animTypes[CLASHER_RUN], 0.5f);

	// ���b�V���ǂݍ���
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

void Clasher::UpdateActor(float _deltaTime)
{
	if (m_hp <= 0) mState = GameObject::STATE_DEAD;
}

void Clasher::Follow(Player& _player, WeakEnemy& _enemy)
{
	// �L�����N�^�[�̑O�����擾
	Vector3 charaForwardVec = GetForward();

	// ���݂̃A�j���[�V�������
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

	// �ǂ��̐w�c������
	switch (m_camp)
	{
	// �����w�c�Ȃ�
	case NONE:
		m_animState = CLASHER_IDLE;
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

		mMeshComp->PlayAnimation(m_animTypes[CLASHER_RUN], animationSpeed);
		break;
	}
}


