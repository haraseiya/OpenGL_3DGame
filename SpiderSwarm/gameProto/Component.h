// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <cstdint>

class GameObject;

class Component
{
public:
	enum class ComponentState
	{
		EActive,
		EPaused,
		EDelete
	};

	Component(GameObject* owner, int updateOrder = 100);                    // �R���X�g���N�^ updateOder���������Ƒ������s�����
	virtual       ~Component();                                             // �f�X�g���N�^
	virtual void   Update(float deltaTime);                                 // �f���^�^�C���ɂ��R���|�[�l���g�̍X�V (�I�[�o�[���C�h�j
	virtual void   ProcessInput() {}                                        // �R���|�[�l���g�̓��͏����i�I�[�o�[���C�h�j
	virtual void   OnUpdateWorldTransform() { }								// ���[���h�ϊ����K�v�Ȏ��ɌĂ΂��֐��i�I�[�o�[���C�h�j
	int            GetID() { return mID; }
	GameObject* GetOwner() { return mOwner; }                               // �I�[�i�[�A�N�^�[�̎擾
	int            GetUpdateOrder() const { return mUpdateOrder; }          // update order�̎擾
	void           SetState(ComponentState newState) { mState = newState; }
	ComponentState GetState() { return mState; }

protected:
	GameObject* mOwner;             // �I�[�i�[�A�N�^�[�ւ̃|�C���^
	int            mUpdateOrder;    // �R���|�[�l���g�̍X�V����
	int            mID;				// �R���|�[�l���g�̊Ǘ�ID
	ComponentState mState;			// 
	static int     mGlobalID;       // �R���|�[�l���g�̊Ǘ�ID�̘A�ԗp
};
