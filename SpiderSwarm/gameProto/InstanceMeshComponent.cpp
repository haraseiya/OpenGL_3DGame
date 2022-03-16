#include "InstanceMeshComponent.h"
#include "InstanceMeshManager.h"
#include "Shader.h"
#include "GameObject.h"
#include "Mesh.h"

// �C���X�^���X�I�u�W�F�N�g�̑���
const int InstanceMeshComponent::mMaxInstance = 100;

InstanceMeshComponent::InstanceMeshComponent(GameObject* owner,InstanceType type)
	: Component(owner)
	, mOwner(owner)
	, mMesh(nullptr)
	, mInstanceType(type)
{
	// �����_�[�N���X�ɃC���X�^���X���b�V���R���|�[�l���g��ǉ�
	//printf("�C���X�^���X���b�V���R���|�[�l���g�ǉ�\n");
	GAMEINSTANCE.GetRenderer()->AddInstanceMeshComponent(this);
}

InstanceMeshComponent::~InstanceMeshComponent()
{
	// �C���X�^���X���b�V���R���|�[�l���g��j��
	GAMEINSTANCE.GetRenderer()->RemoveInstanceMeshComponent(this);
	//printf("�C���X�^���X���b�V���R���|�[�l���g�j��\n");
}

void InstanceMeshComponent::Update(float deltaTime)
{
}

void InstanceMeshComponent::Draw(Shader* shader)
{
}


