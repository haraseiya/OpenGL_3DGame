#pragma once
#include "Singleton.h"
#include <vector>
#include "Math.h"

class GameObject;
class EnemyBase;
class EnemySpawner;
class WeakEnemy;
class StrongEnemy;
class BossEnemy;
class EnemySpawnerEffect;

class EnemyManager /*: public Singleton<EnemyManager>*/
{
public:
	//friend class Singleton<EnemyManager>;	// Singleton�ł̃C���X�^���X�쐬�͋���

	EnemyManager(GameObject* target);
	virtual ~EnemyManager();

	void CreateEnemys();			// �G�𐶐�
	void CreateWave(int waveCount);	// �E�F�[�u�̍쐬
	void RemoveDeadEnemy();			// ���񂾓G��j��
	void Update(float deltaTime);	// �X�V����

	void SpawnWeakEnemy();
	void SpawnStrongEnemy();
	void SpawnBossEnemy();

	bool GetWaveFinishFlag() { return mIsLastWave; }	// �I���t���O�̎擾
	bool GetEnemyExtinction();							// �G����ł������ǂ���

	int GetWaveCount() { return mWaveCount; }
	EnemyBase* GetNearestEnemy(std::vector<EnemyBase*> enemys);	// ��ԋ߂��G

	std::vector<WeakEnemy*> GetWeakEnemys() { return mWeakEnemys; }

	int GetActiveEnemyNum();	// �A�N�e�B�u��Ԃ̓G�����̂��邩���ׂ�

	// �{�X�G�̔z����擾
	std::vector<BossEnemy*> GetBossEnemys() { return mBossEnemys; } 

private:
	GameObject* mTarget;				// �^�[�Q�b�g����I�u�W�F�N�g

	EnemyBase* mWeakEnemyPrototype;		
	EnemySpawner* mWeakEnemySpawner;
	EnemyBase* mStrongEnemyPrototype;
	EnemySpawner* mStrongEnemySpawner;
	EnemyBase* mBossEnemyPrototype;
	EnemySpawner* mBossEnemySpawner;

	EnemySpawnerEffect* mEnemySpawnerEffect[4];

	std::vector<WeakEnemy*> mWeakEnemys;		// �G���G�z��
	std::vector<StrongEnemy*> mStrongEnemys;	// ���G�z��
	std::vector<BossEnemy*> mBossEnemys;		// �{�X�G�z��
	std::vector<std::vector<EnemyBase*>> mEnemyWaveList;	// �G�E�F�[�u���X�g
	std::vector<EnemyBase*> mEnemyWave0;					// �`���[�g���A���p�E�F�[�u
	std::vector<EnemyBase*> mEnemyWave1;					// �E�F�[�u1
	std::vector<EnemyBase*> mEnemyWave2;					// �E�F�[�u2
	std::vector<EnemyBase*> mEnemyWave3;					// �E�F�[�u3
	std::vector<EnemyBase*> mEnemyWave4;					// �E�F�[�u4

	Vector3 mOffset;					// �G�ʒu�̃I�t�Z�b�g
	static const Vector3 mEnemyStartPos;	// �G�̏����ʒu

	static const int mMaxEnemyNum;		// �G�̍ő吔
	static const int mMaxWeakEnemy;		// �G���G�ő吔
	static const int mMaxStrongEnemy;	// ���G�ő吔
	static const int mMaxBossEnemy;		// �{�X�G�ő吔
	static const float mSpawnCoolTime;	// �G�X�|�[���Ԋu

	static const Vector3 mSpawnPoint1;
	static const Vector3 mSpawnPoint2;
	static const Vector3 mSpawnPoint3;
	static const Vector3 mSpawnPoint4;

	float mWeakEnemyTimer;		// �G���G�p�^�C�}�[
	float mStrongEnemyTimer;	// ���G�p�^�C�}�[
	float mBossEnemyTimer;		// �{�X�G�p�^�C�}�[

	int mWaveCount;						// �E�F�[�u�̃J�E���g
	bool mIsLastWave;					// ���X�g�E�F�[�u��
	int mEnemyCount;						// ���݂̓G�̐�
	bool mIsNext;						// ���̃E�F�[�u�ɍs���邩
	bool mIsExtinction;					// ���݂̃E�F�[�u�̓G����ł�����
	float mDistance;

	int mWeakEnemyCount;	// �G���G�J�E���g
	int mStorngEnemyCount;	// ���G�J�E���g

	int mKillCount;

	bool mIsBossEnemyDead;
};

