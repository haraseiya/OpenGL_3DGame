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

	float mVelocityX;	// XŒü‚«‚Ì‘¬‚³
	float mVelocityY;	// YŒü‚«‚Ì‘¬‚³
	float mYaw;			// ‰ñ“]Šp“x

	bool mIsExist;		// ¶‘¶‚µ‚Ä‚¢‚é‚©‚Ç‚¤‚©
};