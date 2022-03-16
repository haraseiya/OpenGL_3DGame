#include "BGBlock.h"
#include "MeshComponent.h"
#include "BoxCollider.h"
#include "Mesh.h"

BGBlock::BGBlock()
	: GameObject(Tag::BACK_GROUND)
	, mMeshComp(nullptr)
{
	mPos = Vector3(0, 0, 0);
}

BGBlock::~BGBlock()
{
}

void BGBlock::SetMesh(class Mesh* mesh)
{
	// ���b�V�����Z�b�g
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(mesh);

	// �����蔻���ݒ�
	mBox = new BoxCollider(this);
	mBox->SetObjectBox(mesh->GetCollisionBox());
}

