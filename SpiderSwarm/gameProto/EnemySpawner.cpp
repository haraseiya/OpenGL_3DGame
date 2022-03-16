#include "EnemySpawner.h"
#include "EnemyBase.h"

EnemySpawner::EnemySpawner(EnemyBase* proto)
	:mEnemyProtoType(proto)
{
}

EnemySpawner::~EnemySpawner()
{
}

//EnemyBase* EnemySpawner::SpawnEnemy(const Vector3& spawnPos)
//{
//	//mSpawnPosition = spawnPos;
//	mEnemyProtoType->SetPosition(spawnPos);
//	return mEnemyProtoType->Clone();
//}
