// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include <cstdint>
#include "Math.h"
#include "Tag.h"
#include "SceneBase.h"

class GameObjectManager;
class ColliderComponent;

class GameObject
{
public:
	// �Q�[���I�u�W�F�N�g�̏��
	enum class State
	{
		STATE_ACTIVE = 0,
		STATE_FALSE,
		STATE_PAUSED,
		STATE_DEAD
	};

	GameObject(Tag objectTag);
	virtual ~GameObject();

	void              Update(float deltaTime);                                                     // Update�֐��@Game����R�[�������i�I�[�o�[���C�h�s�j
	void              UpdateComponents(float deltaTime);                                           // �A�^�b�`����Ă�S�ẴR���|�[�l���g�����̍X�V�i�I�[�o�[���C�h�s�j
	virtual void      UpdateActor(float deltaTime);                                                // �A�N�^�[��p�̃A�b�v�f�[�g�@�i�I�[�o�[���C�h�\�j
	void              ProcessInput();                                                              // �Q�[������Ă΂����͊֘A�i�I�[�o�[���C�h�s�j
	virtual void      OnCollisionEnter(ColliderComponent* ownCollider,ColliderComponent* otherBox);                         // ���̕��̂Ɠ���������

	// Getters/setters�@�Q�b�^�[�E�Z�b�^�[
	const Vector3& GetPosition() const { return mPosition; }                                                            // �ʒu�̃Q�b�g
	void              SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }                // �ʒu�̃Z�b�g
	float             GetScale() const { return mScale; }                                                                  // �X�P�[�����O�̃Q�b�g
	void              SetScale(float scale) { mScale = scale;  mRecomputeWorldTransform = true; }                          // �X�P�[�����O�̃Z�b�g
	const Quaternion& GetRotation() const { return mRotation; }                                                            // ��]�i�N�H�[�^�j�I���j�̃Q�b�g
	void              SetRotation(const Quaternion& rotation) { mRotation = rotation;  mRecomputeWorldTransform = true; }  // ��]�i�N�H�[�^�j�I���j�̃Z�b�g
	Tag               GetTag() { return mTag; }

	virtual void              ComputeWorldTransform();                                            // ���[���h�ϊ��s��̌v�Z
	void              SetComputeWorldTransform() { mRecomputeWorldTransform = true; }              // �ړ��ȂǕϊ��s��̍Čv�Z���K�v�Ȏ�
	const Matrix4&	GetWorldTransform() const { return mWorldTransform; }                        // ���[���h�ϊ��s��̃Q�b�g
	Vector3           GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); } // �O�i�x�N�g���̃Q�b�g
	Vector3           GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }   // �E�����x�N�g���̃Q�b�g
	Vector3           GetBack() const { return Vector3::Transform(Vector3::NegUnitX, mRotation); } // �O�i�x�N�g���̃Q�b�g
	Vector3           GetLeft() const { return Vector3::Transform(Vector3::NegUnitY, mRotation); }   // �E�����x�N�g���̃Q�b�g
	Vector3			  GetDirectionFromForward(Vector3& direction) const { return Vector3::Transform(direction, mRotation); }	// �O������̌���

	State             GetState() const { return mState; }                                          // �A�N�^�[�̏�ԃQ�b�g
	float             GetSpeed() const { return mSpeed; }                                          // �A�N�^�[�̌��݂̃X�s�[�h
	void              SetSpeed(float speed) { mSpeed = speed; }
	void              SetState(State state) { mState = state; }                                    // �A�N�^�[�̏�ԃZ�b�g
	int               GetID() { return mID; };                                                     // �A�N�^�[�̊Ǘ�ID�̃Q�b�g
	void              AddComponent(class Component* component);                                    // �R���|�[�l���g�̒ǉ� 
	void              RemoveComponent(class Component* component);                                 // �R���|�[�l���g�̍폜 
	void              RotateToNewForward(const Vector3& forward);                                  // �O�i�x�N�g�������]���v�Z
	//void			  RotateToNewForwardLerp(const Vector3& forward, float rotateSpeed);

protected:
	Tag               mTag;								// �A�N�^�[���
	State             mState;							// �A�N�^�[�̏��
	Matrix4           mWorldTransform;					// ���[���h�ϊ��s��
	Vector3           mPosition;						// �ʒu
	Vector3           mDirection;						// �L�����N�^�[����
	Quaternion        mRotation;						// ��]
	float             mScale;							// �X�P�[�����O
	float             mSpeed;							// ���݂̃X�s�[�h
	bool              mRecomputeWorldTransform;			// ���[���h�ϊ��̍Čv�Z���K�v���H
	int               mID;								// �Ǘ�ID
	static int        mGlobalActorNo;					// �Ǘ�ID�p�̐ÓI�J�E���^

	std::vector<class Component*> mDeleteComponents;	// �폜�R���|�[�l���g
	std::vector<class Component*> mComponents;			// �R���|�[�l���g�z��        
};
