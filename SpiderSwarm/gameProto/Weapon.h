#pragma once

#include "GameObject.h"

class Mesh;
class SkeletalMeshComponent;
class AttachMeshComponent;
class PlayerBase;

class Weapon : public GameObject
{
public:
	Weapon(PlayerBase* target);
	~Weapon();

private:
	Mesh* mMesh;
	SkeletalMeshComponent* mMeshComp;
	AttachMeshComponent* mAttachMeshComp;
};

