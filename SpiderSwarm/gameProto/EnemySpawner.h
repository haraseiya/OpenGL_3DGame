#pragma once
#include "Math.h"

class EnemyBase;

class EnemySpawner
{
public:
	EnemySpawner(EnemyBase* proto);
	~EnemySpawner();

	EnemyBase* SpawnEnemy(const Vector3& spawnPos);

private:
	EnemyBase* mEnemyProtoType;	// �G�̃v���g�^�C�v
	Vector3 mSpawnPosition;		// �G�̃X�|�[���ʒu
};

