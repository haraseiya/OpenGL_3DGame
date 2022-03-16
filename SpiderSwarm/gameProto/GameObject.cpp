// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "GameObject.h"
#include "GameObjectManager.h"
#include "Game.h"
#include "Component.h"
#include "ColliderComponent.h"

#include <algorithm>
#include <typeinfo>

// �A�N�^�[��ID
int GameObject::mGlobalActorNo = 0;

// 
GameObject::GameObject(Tag objectTag)
	: mTag(objectTag)
	, mState(GameObject::State::STATE_ACTIVE)
	, mPosition(Vector3::Zero)
	, mDirection(Vector3::UnitX)
	, mRotation(Quaternion::Identity)
	, mScale(1.0f)
	, mRecomputeWorldTransform(true)
	, mID(mGlobalActorNo)
	, mSpeed(0.0f)
{
	// �Q�[���I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
	GameObjectManager::GetInstance()->Entry(this);
	mGlobalActorNo++;
}

GameObject::~GameObject()
{
	// �A�N�^�[�������Ă���R���|�[�l���g�����ׂč폜
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}

	// �Q�[���V�X�e���{�̂ɂ��̃A�N�^�[�̍폜���˗�
	GameObjectManager::GetInstance()->Exit(this);
}

void GameObject::Update(float deltaTime)
{
	// �A�N�^�[�������Ă���Ƃ��̂ݏ���
	if (mState == GameObject::State::STATE_ACTIVE)
	{
		// ���[���h���W�̍Čv�Z
		ComputeWorldTransform();

		// �R���|�[�l���g�E�A�N�^�[�̃A�b�v�f�[�g
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		// ����1�x���[���h���W�̍Čv�Z
		ComputeWorldTransform();
	}
}

void GameObject::UpdateComponents(float deltaTime)
{
	// �S�ẴR���|�[�l���g���A�b�v�f�[�g
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}

	// ���S�R���|�[�l���g���ꎞ�ۊ�
	for (auto comp : mComponents)
	{
		if (comp->GetState() == Component::ComponentState::EDelete)
		{
			mDeleteComponents.emplace_back(comp);
		}
	}

	// �ꎞ�ۊǂ��ꂽ���S�R���|�[�l���g���폜
	for (auto comp : mDeleteComponents)
	{
		delete comp;
	}
	mDeleteComponents.clear();
}

// ���̃A�N�^�[�Ǝ��̍X�V���� �i�K�v�Ȃ�I�[�o�[���C�h�j
void GameObject::UpdateActor(float deltaTime)
{
}

// ���̃A�N�^�[�������Ă���R���|�[�l���g�̓��͏���
void GameObject::ProcessInput()
{
	// �A�N�e�B�u��Ԃ̃R���|�[�l���g���X�V
	if (mState == GameObject::State::STATE_ACTIVE)
	{
		// ���͏������󂯎��R���|�[�l���g��D�悵�Ď��s
		for (auto comp : mComponents)
		{
			comp->ProcessInput();
		}
	}
}

// ���̃A�N�^�[�Ƒ��̃A�N�^�[�������Ă���{�b�N�X�ƂԂ������Ƃ��i�K�v�Ȃ�I�[�o�[���C�h)
void GameObject::OnCollisionEnter(ColliderComponent* own,ColliderComponent* otherBox)
{
}

// forward�x�N�g���̌����ɉ�]����
void GameObject::RotateToNewForward(const Vector3& forward)
{
	// X���x�N�g��(1,0,0)��forward�̊Ԃ̊p�x�����߂�
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);

	// �������������ꍇ
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}

	// ������������ꍇ
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// ���x�N�g����forward�Ƃ̊O�ς����]�������Ƃ߁A��]������
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

// ��Ԃ������ĉ�]
//void GameObject::RotateToNewForwardLerp(const Vector3& forward, float rotateSpeed)
//{
//	// X���x�N�g��(1,0,0)��forward�̊Ԃ̊p�x�����߂�
//	float dot = Vector3::Dot(Vector3::UnitX, forward);
//	float angle = Math::Acos(dot);
//	// �������������ꍇ
//	if (dot > 0.9999f)
//	{
//		SetRotation(Quaternion::Identity);
//	}
//	// ������������ꍇ
//	else if (dot < -0.9999f)
//	{
//		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
//	}
//	else
//	{
//		// ���x�N�g����forward�Ƃ̊O�ς����]�������Ƃ߁A��]������
//		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
//		axis.Normalize();
//		SetRotation(Quaternion(axis, angle));
//	}
//}

// ���[���h�ϊ��s����v�Z
void GameObject::ComputeWorldTransform()
{
	//���[���h�ϊ��̍Čv�Z���K�v�Ȃ���s
	if (mRecomputeWorldTransform)
	{

		mRecomputeWorldTransform = false;

		// �X�P�[�����O����]�����s�ړ��ƂȂ�悤�ɕϊ��s����쐬
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		// �A�N�^�[�������Ă���S�R���|�[�l���g�̕ϊ��𑣂�
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

// �R���|�[�l���g�̒ǉ�
void GameObject::AddComponent(Component* component)
{
	// �R���|�[�l���g���\�[�g���Ēǉ�
	// �����̃I�[�_�[�ԍ������傫���}���_��������
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();

	for (;iter != mComponents.end();++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	// �v�f���������|�C���g�̎�O�ɑ}������
	mComponents.insert(iter, component);
}

// �R���|�[�l���g�̍폜
void GameObject::RemoveComponent(Component* component)
{
	// �R���|�[�l���g��T���č폜
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}
