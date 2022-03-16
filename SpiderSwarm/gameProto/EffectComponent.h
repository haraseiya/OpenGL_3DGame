#pragma once
#include "Component.h"
#include "math.h"
#include <cstddef>
#include <Effekseer.h>

class GameObject;
class EffekseerEffect;

class EffectComponent : public Component
{
public:
	EffectComponent(GameObject* owner, bool moveOn = false, bool rotateOn = false, bool loop = false, int updateOrder = 100);
	~EffectComponent();
	void LoadEffect(const char16_t* effkseerFilename);
	void CreateEffect();
	void Update(float deltaTime) override;

	void SetRelativeScale(const float& relativeScale) { mRelativeScale=relativeScale; }
	void SetRelativePosition(const Vector3& relativePos) { mRelativePos = relativePos; }
	void SetRelativeRotate(const Matrix4& relativeRotMat) { mRelativeRot = relativeRotMat; }
	void SetMove(bool move) { mIsMove = move; }
	void SetRotate(bool rotate) { mIsRotate = rotate; }

private:
	GameObject* mOwner;					// �I�[�i�[�A�N�^�[
	Effekseer::Handle mHandle;          // �G�t�F�N�g�p�����[�^���Ǘ����邽�߂̃n���h��
	EffekseerEffect* mEffect;
	bool              mIsMove;          // �I�[�i�[�A�N�^�[�̈ړ��ɍ��킹��
	bool              mIsRotate;        // �I�[�i�[�A�N�^�[�̉�]�ړ��ɍ��킹��
	bool mIsLoop;						// ���[�v���邩�ǂ���
	float mRelativeScale;
	Vector3           mRelativePos;     // �I�[�i�[�A�N�^�[�̑��΍��W
	Matrix4           mRelativeRot;     // ���Ή�]�s��
};