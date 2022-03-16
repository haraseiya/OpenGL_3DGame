#include "EnemyManager.h"
#include "WeakEnemy.h"
#include "StrongEnemy.h"
#include "BossEnemy.h"
#include "EnemySpawner.h"
#include "EnemySpawnerEffect.h"
#include <iostream>

// 敵初期位置（先に生成だけするので遠くに飛ばしておく）
const Vector3 EnemyManager::mEnemyStartPos = Vector3(5000, 5000, 5000);

// 各種敵の最大数
const int EnemyManager::mMaxBossEnemy = 1;			// ボス敵最大数
const int EnemyManager::mMaxStrongEnemy = 8;		// 強敵最大数
const int EnemyManager::mMaxWeakEnemy = 30;			// 雑魚敵最大数

// 敵の発生源の初期位置
const Vector3 EnemyManager::mSpawnPoint1 = Vector3(2000, 2000, 750);
const Vector3 EnemyManager::mSpawnPoint2 = Vector3(-2000, 2000, 750);
const Vector3 EnemyManager::mSpawnPoint3 = Vector3(-2000, -2000, 750);
const Vector3 EnemyManager::mSpawnPoint4 = Vector3(2000, -2000, 750);

// スポーンする間隔
const float EnemyManager::mSpawnCoolTime = 10.0f;	

// 敵マネージャークラスのコンストラクタ
EnemyManager::EnemyManager(GameObject* target)
	: mTarget(target)
	, mWeakEnemyTimer(0.0f)
	, mStrongEnemyTimer(0.0f)
	, mWaveCount(0)
	, mIsLastWave(false)
	, mIsNext(false)
	, mWeakEnemyCount(0)
	, mEnemyCount(0)
	, mIsBossEnemyDead(false)
{
	mOffset = Vector3(0, 0, 750);

	// 敵のスポーン位置を定義
	mEnemySpawnerEffect[0] = new EnemySpawnerEffect(mSpawnPoint1);
	mEnemySpawnerEffect[1] = new EnemySpawnerEffect(mSpawnPoint2);
	mEnemySpawnerEffect[2] = new EnemySpawnerEffect(mSpawnPoint3);
	mEnemySpawnerEffect[3] = new EnemySpawnerEffect(mSpawnPoint4);

	// 先に全ての敵を生成
	CreateEnemys();

	// プロトタイプパターン
	//mWeakEnemyPrototype = new WeakEnemy(mTarget);
	//mWeakEnemySpawner = new EnemySpawner(mWeakEnemyPrototype);
	//mStrongEnemyPrototype = new StrongEnemy(mTarget);
	//mStrongEnemySpawner = new EnemySpawner(mStrongEnemyPrototype);
	//mBossEnemyPrototype = new StrongEnemy(mTarget);
	//mBossEnemySpawner = new EnemySpawner(mBossEnemyPrototype);
}

// 全てのオブジェクト削除処理
EnemyManager::~EnemyManager()
{
	// 雑魚敵
	for (auto e:mWeakEnemys)
	{
		e->SetState(GameObject::State::STATE_DEAD);
	}

	// 強敵
	for (auto e : mStrongEnemys)
	{
		e->SetState(GameObject::State::STATE_DEAD);
	}

	// ボス敵
	for (auto e : mBossEnemys)
	{
		e->SetState(GameObject::State::STATE_DEAD);
	}

	// スポーン用エフェクト
	for (auto e : mEnemySpawnerEffect)
	{
		e->SetState(GameObject::State::STATE_DEAD);
	}

	//mWeakEnemys.clear();
	//mStrongEnemys.clear();
	//mBossEnemys.clear();
	//delete[] mEnemySpawnerEffect;
}

// 使用する敵をあらかじめ全て生成しておく
void EnemyManager::CreateEnemys()
{
	// 雑魚敵
	for (int i = 0; i < mMaxWeakEnemy; i++)
	{ 
		mWeakEnemys.emplace_back(new WeakEnemy(mTarget, mEnemyStartPos));
		mWeakEnemys[i]->SetState(GameObject::State::STATE_PAUSED);
		mWeakEnemys[i]->SetEnemyStateScene(EnemyStateScene::ENEMY_SCENE_GAME);
	}
	// 強敵
	for (int i = 0; i < mMaxStrongEnemy; i++)
	{
		mStrongEnemys.emplace_back(new StrongEnemy(mTarget, mEnemyStartPos));
		mStrongEnemys[i]->SetState(GameObject::State::STATE_PAUSED);
		mStrongEnemys[i]->SetEnemyStateScene(EnemyStateScene::ENEMY_SCENE_GAME);
	}
	// ボス敵
	for (int i = 0; i < mMaxBossEnemy; i++)
	{
		mBossEnemys.emplace_back(new BossEnemy(mTarget, mEnemyStartPos));
		mBossEnemys[i]->SetState(GameObject::State::STATE_PAUSED);
		mBossEnemys[i]->SetEnemyStateScene(EnemyStateScene::ENEMY_SCENE_GAME);
	}
}

void EnemyManager::RemoveDeadEnemy()
{
}

// 更新処理
void EnemyManager::Update(float deltaTime)
{
	mWeakEnemyTimer += deltaTime;
	mStrongEnemyTimer += deltaTime;

	// 時間ごとに雑魚敵をスポーン
	const float spawnWeakEnemyCoolTime = 20.0f;
	const bool isWeakEnemySpawn = mWeakEnemyTimer > spawnWeakEnemyCoolTime;
	if (isWeakEnemySpawn)
	{
		mWeakEnemyTimer = 0.0f;
		SpawnWeakEnemy();
	}

	// 時間ごとに強敵をスポーン
	const float spawnStrongEnemyCoolTime = 90.0f;
	const bool isStrongEnemySpawn = mStrongEnemyTimer > spawnStrongEnemyCoolTime;
	if (isStrongEnemySpawn)
	{
		mStrongEnemyTimer = 0.0f;
		SpawnStrongEnemy();
	}
}

// 雑魚敵の生成
void EnemyManager::SpawnWeakEnemy()
{
	// 雑魚敵全体を走査
	for (auto e : mWeakEnemys)
	{
		// 一度に4体まで生成
		if (mWeakEnemyCount > 3) 
		{
			mWeakEnemyCount = 0;
			return; 
		}

		// 使用されていない敵がいれば
		if (e->GetIsActive() == false)
		{
			Vector3 pos = Vector3(mEnemySpawnerEffect[mWeakEnemyCount]->GetPosition());
			e->SetIsActive(true);
			e->SetState(GameObject::State::STATE_ACTIVE);
			e->SetPosition(pos);
			mWeakEnemyCount++;
		}
	}
}

// 強敵の生成
void EnemyManager::SpawnStrongEnemy()
{
	// 強敵全体を走査
	for (auto e : mStrongEnemys)
	{
		// 4体まで生成したらカウントを0に
		if (mStorngEnemyCount > 3)
		{
			mStorngEnemyCount = 0;
			return;
		}

		// 使われていない状態であれば
		if (e->GetIsActive() == false)
		{
			Vector3 pos = Vector3(mEnemySpawnerEffect[mStorngEnemyCount]->GetPosition());
			e->SetIsActive(true);
			e->SetState(GameObject::State::STATE_ACTIVE);
			e->SetPosition(pos);
			mStorngEnemyCount++;
		}
	}
}

// ボス敵の生成
void EnemyManager::SpawnBossEnemy()
{
	for (auto e : mBossEnemys)
	{
		//// ボス敵が死んだらフラグを立てる
		//if (e->GetIsDeadFlag())
		//{
		//	mIsBossEnemyDead = true;
		//}
		if (e->GetIsActive() == false)
		{
			Vector3 pos = Vector3(1000, 0, 750);
			e->SetIsActive(true);
			e->SetState(GameObject::State::STATE_ACTIVE);
			e->SetPosition(pos);
		}
	}
}

bool EnemyManager::GetEnemyExtinction()
{
	return true;
}

EnemyBase* EnemyManager::GetNearestEnemy(std::vector<EnemyBase*> enemys)
{
	// 敵が存在しなければ返す
	if (enemys.empty())return nullptr;

	std::vector<float> distance;
	for (auto e : enemys)
	{
		distance.emplace_back(Math::Abs(mTarget->GetPosition().LengthSq() - e->GetPosition().LengthSq()));
	}
	return enemys[0];
}

int EnemyManager::GetActiveEnemyNum()
{
	int count = 0;

	// 雑魚敵配列を全て走査
	for (auto e : mWeakEnemys)
	{
		// アクティブの敵がいれば
		const bool isActive = e->GetIsActive() == true;
		if (isActive)
		{
			count++;
		}
	}

	// 強敵配列を全て走査
	for (auto e : mStrongEnemys)
	{
		const bool isActive = e->GetIsActive() == true;
		if (isActive)
		{
			count++;
		}
	}

	// ボス敵配列を全て走査
	for (auto e : mBossEnemys)
	{
		const bool isActive = e->GetIsActive() == true;
		if (isActive)
		{
			count++;
		}
	}

	return count;
}



