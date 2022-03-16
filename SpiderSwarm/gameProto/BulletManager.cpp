#include "BulletManager.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "Renderer.h"
#include "Shader.h"
#include "InstanceMeshManager.h"
#include "Input.h"
#include "Game.h"

// �g���e�̑���
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

// �e�X�V����
void BulletManager::Update(float deltaTime)
{
	// �e�z�񕪂̃C�e���[�^�[����
	for (std::list<EnemyBullet*>::iterator itr = mEnemyBullet.begin(); itr != mEnemyBullet.end(); itr++)
	{
		// �z��ɑ��݂����
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
