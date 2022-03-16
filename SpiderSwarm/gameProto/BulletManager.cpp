#include "BulletManager.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "Renderer.h"
#include "Shader.h"
#include "InstanceMeshManager.h"
#include "Input.h"
#include "Game.h"

// 使う弾の総数
const int BulletManager::mAmount = 1000;
const float BulletManager::mInterval = 0.1f;

BulletManager::BulletManager(GameObject* owner)
	: mOwner(owner)
	, mEnemyBullet()
{
}

BulletManager::~BulletManager()
{
}

// 弾更新処理
void BulletManager::Update(float deltaTime)
{
	// 弾配列分のイテレーターを回す
	for (std::list<EnemyBullet*>::iterator itr = mEnemyBullet.begin(); itr != mEnemyBullet.end(); itr++)
	{
		// 配列に存在すれば
		if ((*itr) != nullptr)
		{
			(*itr)->Update(deltaTime);
		}
	}
}

void BulletManager::CreateBullet()
{
}

void BulletManager::DeleteBullet()
{
}
