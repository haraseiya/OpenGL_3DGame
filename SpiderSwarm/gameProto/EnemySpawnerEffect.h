#pragma once
#include "EffectBase.h"

class Mesh;
class SkeletalMeshComponent;

class EnemySpawnerEffect : public EffectBase
{
public:
	EnemySpawnerEffect(const Vector3& pos);
	~EnemySpawnerEffect();

	const Vector3& GetPosition() const { return mPosition; }

	void LoadEffect()override;
	void CreateEffect()override;

private:
	Mesh* mMesh;
	SkeletalMeshComponent* mMeshComp;
};

