#pragma once
#include "GameObject.h"

class GameObject;

// �V�[�����̃J��������
enum class CameraSceneState : unsigned char
{
	CAMERA_SCENE_TITLE,
	CAMERA_SCENE_GAME,
	CAMERA_SCENE_RESULT
};

class CameraActor
{
public:
	CameraActor(GameObject* target);
	CameraActor(std::vector<GameObject*> targets);

	virtual ~CameraActor();
	virtual void UpdateActor(float deltaTime);

	void     Update(float deltaTime);      //�I�[�o�[���C�h�s��
	void     SetActive();
	void     Init(const Vector3& cameraPos, const Vector3& targetPos, const Vector3& upVec);
	Matrix4& GetViewMatrix() { return mViewMatrix; }
	Vector3& GetViewTarget() { return mViewTarget; }
	Vector3& GetViewPos() { return mPosition; }

	void SetPosition(const Vector3& pos) { mPosition = pos; }
	void SetTarget(const Vector3& target) { mViewTarget = target; }

protected:
	CameraSceneState mCameraSceneState;

	GameObject* mTargetActor;  // �J��������Ƃ���A�N�^�[
	std::vector<GameObject*> mTargetActors;	// ��Ƃ���A�N�^�[����������ꍇ
	Matrix4  mViewMatrix;   // �r���[�s��
	Vector3  mPosition;		// �ʒu
	Vector3  mViewTarget;   // �J�������ړ_
	Vector3  mViewVector;   // �����x�N�g��
};