#include "InstanceMeshComponent.h"
#include "InstanceMeshManager.h"
#include "Shader.h"
#include "GameObject.h"
#include "Mesh.h"

// インスタンスオブジェクトの総数
const int InstanceMeshComponent::mMaxInstance = 100;

InstanceMeshComponent::InstanceMeshComponent(GameObject* owner,InstanceType type)
	: Component(owner)
	, mOwner(owner)
	, mMesh(nullptr)
	, mInstanceType(type)
{
	// レンダークラスにインスタンスメッシュコンポーネントを追加
	//printf("インスタンスメッシュコンポーネント追加\n");
	GAMEINSTANCE.GetRenderer()->AddInstanceMeshComponent(this);
}

InstanceMeshComponent::~InstanceMeshComponent()
{
	// インスタンスメッシュコンポーネントを破棄
	GAMEINSTANCE.GetRenderer()->RemoveInstanceMeshComponent(this);
	//printf("インスタンスメッシュコンポーネント破棄\n");
}

void InstanceMeshComponent::Update(float deltaTime)
{
}

void InstanceMeshComponent::Draw(Shader* shader)
{
}


