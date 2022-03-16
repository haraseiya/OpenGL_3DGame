#pragma once
#include "Component.h"
#include "Math.h"
#include "InstanceType.h"

#include <vector>

class GameObject;
class Shader;
class Mesh;
class VertexArray;
class InstanceMeshManager;

class InstanceMeshComponent : public Component
{
public:
	InstanceMeshComponent(GameObject* owner,InstanceType type);
	~InstanceMeshComponent();

	void Update(float deltaTime) override;
	void Draw(Shader* shader);

	InstanceType GetType() { return mInstanceType; }

private:
	//unsigned int mBuffer;
	float* mBufferMatrices;
	static const int mMaxInstance;
	std::vector<GameObject*> mModelMatrices;

	Mesh* mMesh;
	VertexArray* mVertexArray;
	GameObject* mOwner;

	Shader* mShader;
	InstanceMeshManager* mInstanceManager;
	InstanceType mInstanceType;
};