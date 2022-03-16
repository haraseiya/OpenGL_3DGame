#pragma once
#include "GameObject.h"

class Player;
class WeakEnemy;
class SkeletalMeshComponent;
class SpriteComponent;
class BoxCollider;
class Animation;
class Mesh;
struct Sphere;

class Healer :public GameObject
{
public:
	// �A�j���[�V�����̎��
	enum AnimKind
	{
		HEALER_IDLE,		// �ҋ@�A�j���[�V����
		HEALER_RUN,			// ����A�j���[�V����
		HEALER_HEAL,		// �񕜃A�j���[�V����
		HEALER_ITEMNUM		// ���A�j���[�V������
	};
	// �ǂ̐w�c������
	enum Camp
	{
		NONE,		// �ǂ��瑤�ł��Ȃ�
		PLAYER,		// �v���C���[�w�c
		ENEMY,		// �G�l�~�[�w�c
	};

	// NPC�̏��
	enum State
	{
		WAIT,		// �҂�
		FOLLOW,		// �Ǐ]
		ATTACK,		// �U��
	};

	Healer();
	~Healer();

	void ChangeState(Player& _player, WeakEnemy& _enemy);

	// �Ǐ]����
	void Follow(Player& _player, WeakEnemy& _enemy);

	// ��ԃQ�b�^�[�Z�b�^�[
	const State GetNPCState() { return m_state; }
	const void SetNPCState(State _state) { m_state = _state; }

	// �w�c�̃Q�b�^�[�Z�b�^�[
	const Camp GetCamp() { return m_camp; }
	void SetCamp(Camp _camp) { m_camp = _camp; }

	const int GetHP() { return m_hp; }
	const void SetHP(int _hp) { m_hp = _hp; }

	void UpdateActor(float _deltaTime)override;

private:
	SkeletalMeshComponent* mMeshComp;
	SpriteComponent* m_spriteComp;
	Mesh* m_mesh;
	BoxCollider* m_hitBox;
	BoxCollider* mHitGroundBox;
	BoxCollider* mHitHeadBox;
	std::vector<BoxCollider*>   mPlayerBoxs;                 // �v���[���[������f�[�^
	std::vector<BoxCollider*>   mNPCBoxs;
	Sphere* m_collision;

	Camp m_camp;
	State m_state;
	std::vector<const Animation*> m_animTypes;

	float m_speed;
	int m_hp;
};