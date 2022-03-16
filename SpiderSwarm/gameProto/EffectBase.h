#pragma once
#include "GameObject.h"

class EffectComponent;

class EffectBase : public GameObject
{
public:
	EffectBase(Tag tag);
	virtual ~EffectBase();

	// �G�t�F�N�g�̃A�b�v�f�[�g
	void UpdateActor(float deltaTime) override;

	// �G�t�F�N�g�̃��[�h
	virtual void LoadEffect() = 0;

	// �G�t�F�N�g�̐���
	virtual void CreateEffect() = 0;

protected:
	EffectComponent* mEffectComponent;
	float m_lifeCount;
};

