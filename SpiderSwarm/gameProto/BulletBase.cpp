#include "BulletBase.h"
#include "InstanceMeshComponent.h"
#include "InstanceMeshManager.h"
#include "Renderer.h"

BulletBase::BulletBase(Tag tag, InstanceType instanceType)
	: GameObject(tag)
	, mInstanceMeshManager(RENDERER->GetInstanceMeshManager())
{
	// �C���X�^���X���b�V������
	mInstanceMeshComp = new InstanceMeshComponent(this, instanceType);

}

BulletBase::~BulletBase()
{
}

void BulletBase::Create()
{
	mIsExist = false;

}
