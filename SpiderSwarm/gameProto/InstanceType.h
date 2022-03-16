#pragma once

// �C���X�^���X�̃^�C�v
enum class InstanceType
{
	Begin,
	PlayerBullet1 = 0,	// �v���C���[1�̒e
	PlayerBullet2,		// �v���C���[2�̒e
	EnemyBullet1,		// �G���G�̒e
	EnemyBullet2,		// ���G�̒e
	EnemyBullet3,		// �{�X�G�̒e
	EnergyCapsule,		// �G�l���M�[�J�v�Z��
	HealCapsule,
	EnemySpawner,

	InstanceTypeNum,

	End
};

InstanceType& operator++(InstanceType& rhs);