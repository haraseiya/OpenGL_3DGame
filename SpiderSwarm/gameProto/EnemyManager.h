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
	//friend class Singleton<EnemyManager>;	// Singletonでのインスタンス作成は許可

	EnemyManager(GameObject* target);
	virtual ~EnemyManager();

	void CreateEnemys();			// 敵を生成
	void CreateWave(int waveCount);	// ウェーブの作成
	void RemoveDeadEnemy();			// 死んだ敵を破棄
	void Update(float deltaTime);	// 更新処理

	void SpawnWeakEnemy();
	void SpawnStrongEnemy();
	void SpawnBossEnemy();

	bool GetWaveFinishFlag() { return mIsLastWave; }	// 終了フラグの取得
	bool GetEnemyExtinction();							// 敵が絶滅したかどうか

	int GetWaveCount() { return mWaveCount; }
	EnemyBase* GetNearestEnemy(std::vector<EnemyBase*> enemys);	// 一番近い敵

	std::vector<WeakEnemy*> GetWeakEnemys() { return mWeakEnemys; }

	int GetActiveEnemyNum();	// アクティブ状態の敵が何体いるか調べる

	// ボス敵の配列を取得
	std::vector<BossEnemy*> GetBossEnemys() { return mBossEnemys; } 

private:
	GameObject* mTarget;				// ターゲットするオブジェクト

	EnemyBase* mWeakEnemyPrototype;		
	EnemySpawner* mWeakEnemySpawner;
	EnemyBase* mStrongEnemyPrototype;
	EnemySpawner* mStrongEnemySpawner;
	EnemyBase* mBossEnemyPrototype;
	EnemySpawner* mBossEnemySpawner;

	EnemySpawnerEffect* mEnemySpawnerEffect[4];

	std::vector<WeakEnemy*> mWeakEnemys;		// 雑魚敵配列
	std::vector<StrongEnemy*> mStrongEnemys;	// 強敵配列
	std::vector<BossEnemy*> mBossEnemys;		// ボス敵配列
	std::vector<std::vector<EnemyBase*>> mEnemyWaveList;	// 敵ウェーブリスト
	std::vector<EnemyBase*> mEnemyWave0;					// チュートリアル用ウェーブ
	std::vector<EnemyBase*> mEnemyWave1;					// ウェーブ1
	std::vector<EnemyBase*> mEnemyWave2;					// ウェーブ2
	std::vector<EnemyBase*> mEnemyWave3;					// ウェーブ3
	std::vector<EnemyBase*> mEnemyWave4;					// ウェーブ4

	Vector3 mOffset;					// 敵位置のオフセット
	static const Vector3 mEnemyStartPos;	// 敵の初期位置

	static const int mMaxEnemyNum;		// 敵の最大数
	static const int mMaxWeakEnemy;		// 雑魚敵最大数
	static const int mMaxStrongEnemy;	// 強敵最大数
	static const int mMaxBossEnemy;		// ボス敵最大数
	static const float mSpawnCoolTime;	// 敵スポーン間隔

	static const Vector3 mSpawnPoint1;
	static const Vector3 mSpawnPoint2;
	static const Vector3 mSpawnPoint3;
	static const Vector3 mSpawnPoint4;

	float mWeakEnemyTimer;		// 雑魚敵用タイマー
	float mStrongEnemyTimer;	// 強敵用タイマー
	float mBossEnemyTimer;		// ボス敵用タイマー

	int mWaveCount;						// ウェーブのカウント
	bool mIsLastWave;					// ラストウェーブか
	int mEnemyCount;						// 現在の敵の数
	bool mIsNext;						// 次のウェーブに行けるか
	bool mIsExtinction;					// 現在のウェーブの敵が絶滅したか
	float mDistance;

	int mWeakEnemyCount;	// 雑魚敵カウント
	int mStorngEnemyCount;	// 強敵カウント

	int mKillCount;

	bool mIsBossEnemyDead;
};

