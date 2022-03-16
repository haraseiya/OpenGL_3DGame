#pragma once

// インスタンスのタイプ
enum class InstanceType
{
	Begin,
	PlayerBullet1 = 0,	// プレイヤー1の弾
	PlayerBullet2,		// プレイヤー2の弾
	EnemyBullet1,		// 雑魚敵の弾
	EnemyBullet2,		// 強敵の弾
	EnemyBullet3,		// ボス敵の弾
	EnergyCapsule,		// エネルギーカプセル
	HealCapsule,
	EnemySpawner,

	InstanceTypeNum,

	End
};

InstanceType& operator++(InstanceType& rhs);