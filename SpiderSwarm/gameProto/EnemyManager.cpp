#include "EnemyManager.h"
#include "WeakEnemy.h"
#include "StrongEnemy.h"
#include "BossEnemy.h"
#include "EnemySpawner.h"
#include "EnemySpawnerEffect.h"
#include <iostream>

// �G�����ʒu�i��ɐ�����������̂ŉ����ɔ�΂��Ă����j
const Vector3 EnemyManager::mEnemyStartPos = Vector3(5000, 5000, 5000);

// �e��G�̍ő吔
const int EnemyManager::mMaxBossEnemy = 1;			// �{�X�G�ő吔
const int EnemyManager::mMaxStrongEnemy = 8;		// ���G�ő吔
const int EnemyManager::mMaxWeakEnemy = 30;			// �G���G�ő吔

// �G�̔������̏����ʒu
const Vector3 EnemyManager::mSpawnPoint1 = Vector3(2000, 2000, 750);
const Vector3 EnemyManager::mSpawnPoint2 = Vector3(-2000, 2000, 750);
const Vector3 EnemyManager::mSpawnPoint3 = Vector3(-2000, -2000, 750);
const Vector3 EnemyManager::mSpawnPoint4 = Vector3(2000, -2000, 750);

// �X�|�[������Ԋu
const float EnemyManager::mSpawnCoolTime = 10.0f;	

// �G�}�l�[�W���[�N���X�̃R���X�g���N�^
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

	// �G�̃X�|�[���ʒu���`
	mEnemySpawnerEffect[0] = new EnemySpawnerEffect(mSpawnPoint1);
	mEnemySpawnerEffect[1] = new EnemySpawnerEffect(mSpawnPoint2);
	mEnemySpawnerEffect[2] = new EnemySpawnerEffect(mSpawnPoint3);
	mEnemySpawnerEffect[3] = new EnemySpawnerEffect(mSpawnPoint4);

	// ��ɑS�Ă̓G�𐶐�
	CreateEnemys();

	// �v���g�^�C�v�p�^�[��
	//mWeakEnemyPrototype = new WeakEnemy(mTarget);
	//mWeakEnemySpawner = new EnemySpawner(mWeakEnemyPrototype);
	//mStrongEnemyPrototype = new StrongEnemy(mTarget);
	//mStrongEnemySpawner = new EnemySpawner(mStrongEnemyPrototype);
	//mBossEnemyPrototype = new StrongEnemy(mTarget);
	//mBossEnemySpawner = new EnemySpawner(mBossEnemyPrototype);
}

// �S�ẴI�u�W�F�N�g�폜����
EnemyManager::~EnemyManager()
{
	// �G���G
	for (auto e:mWeakEnemys)
	{
		e->SetState(GameObject::State::STATE_DEAD);
	}

	// ���G
	for (auto e : mStrongEnemys)
	{
		e->SetState(GameObject::State::STATE_DEAD);
	}

	// �{�X�G
	for (auto e : mBossEnemys)
	{
		e->SetState(GameObject::State::STATE_DEAD);
	}

	// �X�|�[���p�G�t�F�N�g
	for (auto e : mEnemySpawnerEffect)
	{
		e->SetState(GameObject::State::STATE_DEAD);
	}

	//mWeakEnemys.clear();
	//mStrongEnemys.clear();
	//mBossEnemys.clear();
	//delete[] mEnemySpawnerEffect;
}

// �g�p����G�����炩���ߑS�Đ������Ă���
void EnemyManager::CreateEnemys()
{
	// �G���G
	for (int i = 0; i < mMaxWeakEnemy; i++)
	{ 
		mWeakEnemys.emplace_back(new WeakEnemy(mTarget, mEnemyStartPos));
		mWeakEnemys[i]->SetState(GameObject::State::STATE_PAUSED);
		mWeakEnemys[i]->SetEnemyStateScene(EnemyStateScene::ENEMY_SCENE_GAME);
	}
	// ���G
	for (int i = 0; i < mMaxStrongEnemy; i++)
	{
		mStrongEnemys.emplace_back(new StrongEnemy(mTarget, mEnemyStartPos));
		mStrongEnemys[i]->SetState(GameObject::State::STATE_PAUSED);
		mStrongEnemys[i]->SetEnemyStateScene(EnemyStateScene::ENEMY_SCENE_GAME);
	}
	// �{�X�G
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

// �X�V����
void EnemyManager::Update(float deltaTime)
{
	mWeakEnemyTimer += deltaTime;
	mStrongEnemyTimer += deltaTime;

	// ���Ԃ��ƂɎG���G���X�|�[��
	const float spawnWeakEnemyCoolTime = 20.0f;
	const bool isWeakEnemySpawn = mWeakEnemyTimer > spawnWeakEnemyCoolTime;
	if (isWeakEnemySpawn)
	{
		mWeakEnemyTimer = 0.0f;
		SpawnWeakEnemy();
	}

	// ���Ԃ��Ƃɋ��G���X�|�[��
	const float spawnStrongEnemyCoolTime = 90.0f;
	const bool isStrongEnemySpawn = mStrongEnemyTimer > spawnStrongEnemyCoolTime;
	if (isStrongEnemySpawn)
	{
		mStrongEnemyTimer = 0.0f;
		SpawnStrongEnemy();
	}
}

// �G���G�̐���
void EnemyManager::SpawnWeakEnemy()
{
	// �G���G�S�̂𑖍�
	for (auto e : mWeakEnemys)
	{
		// ��x��4�̂܂Ő���
		if (mWeakEnemyCount > 3) 
		{
			mWeakEnemyCount = 0;
			return; 
		}

		// �g�p����Ă��Ȃ��G�������
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

// ���G�̐���
void EnemyManager::SpawnStrongEnemy()
{
	// ���G�S�̂𑖍�
	for (auto e : mStrongEnemys)
	{
		// 4�̂܂Ő���������J�E���g��0��
		if (mStorngEnemyCount > 3)
		{
			mStorngEnemyCount = 0;
			return;
		}

		// �g���Ă��Ȃ���Ԃł����
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

// �{�X�G�̐���
void EnemyManager::SpawnBossEnemy()
{
	for (auto e : mBossEnemys)
	{
		//// �{�X�G�����񂾂�t���O�𗧂Ă�
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
	// �G�����݂��Ȃ���ΕԂ�
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

	// �G���G�z���S�đ���
	for (auto e : mWeakEnemys)
	{
		// �A�N�e�B�u�̓G�������
		const bool isActive = e->GetIsActive() == true;
		if (isActive)
		{
			count++;
		}
	}

	// ���G�z���S�đ���
	for (auto e : mStrongEnemys)
	{
		const bool isActive = e->GetIsActive() == true;
		if (isActive)
		{
			count++;
		}
	}

	// �{�X�G�z���S�đ���
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



