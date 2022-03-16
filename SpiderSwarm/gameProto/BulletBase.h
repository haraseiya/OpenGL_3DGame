#pragma once
#include "GameObject.h"
#include "InstanceType.h"

class InstanceMeshComponent;
class InstanceMeshManager;

class BulletBase : public GameObject
{
public:
	BulletBase(Tag tag,InstanceType instanceType);
	~BulletBase();

	virtual void SetCollider() = 0;
	void Create();
protected:
	InstanceMeshComponent* mInstanceMeshComp;
	InstanceMeshManager* mInstanceMeshManager;

	float mVelocityX;	// X�����̑���
	float mVelocityY;	// Y�����̑���
	float mYaw;			// ��]�p�x

	bool mIsExist;		// �������Ă��邩�ǂ���
};