#pragma once

#include <vector>
#include <memory>

class GameObject;
class PlayerBullet;
class EnemyBullet;
class InstanceMeshManager;

class BulletManager
{
public:
	BulletManager(GameObject* owner);
	~BulletManager();

	void Update(float deltaTime);

	void CreateBullet();	// 生成
	void DeleteBullet();	// 削除

private:
	GameObject* mOwner;
	std::list<EnemyBullet*> mEnemyBullet;	// プレイヤー弾配列

	InstanceMeshManager* mInstanceMeshManager;

	float mShootTimer;
	static const int mAmount;		// オブジェクトの総数
	static const float mInterval;	// ショットの間隔
};