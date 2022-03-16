#pragma once
#include "GameObject.h"

class BGBlock : public GameObject
{
public:
	BGBlock();
	~BGBlock();
	void SetMesh(class Mesh* mesh);

private:
	Vector3 mPos;
	class MeshComponent* mMeshComp;
	class BoxCollider* mBox;
};