#pragma once
#include "Component.h"
#include "Math.h"

class GameObject;
class Shader;
class Mesh;

class PointLightComponent : public Component
{
public:
	PointLightComponent(GameObject* owner);
	~PointLightComponent();

	// ���̃|�C���g���C�g���W�I���g���Ƃ��ĕ`��
	void Draw(Shader* shader, Mesh* mesh);

	// �f�B�t���[�Y�J���[
	Vector3 mDiffuseColor;

	// ���C�g�̔��a
	float mInnerRadius;
	float mOuterRadius;
};

