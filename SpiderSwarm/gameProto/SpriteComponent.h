#pragma once
#include "Component.h"

class GameObject;
class Shader;
class Texture;

class SpriteComponent : public Component
{
public:
	SpriteComponent(GameObject* _owner, int _drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(Shader* _shader);

	// �Q�b�^�[�Z�b�^�[ //
	// �e�N�X�`�����Z�b�g���c���̒������v�Z����
	virtual void SetTexture(Texture* _texture);

	// �e�N�X�`���̉���
	const int GetTexWidth() const { return m_textureWidth; }

	// �e�N�X�`���̏c��
	const int GetTexHeight() const { return m_textureHeight; }

	// �e�N�X�`���̕`�揇
	const int GetDrawOrder() const { return m_drawOrder; }

	// �`�悷�邩�ǂ�����ݒ�
	void SetVisible(bool _visible) { m_visible = _visible; }

	// �`�悷�邩�ǂ������擾
	const bool GetVisible() const { return m_visible; }

protected:
	Texture* m_texture;		// �e�N�X�`���̃|�C���^
	int m_drawOrder;		// �`�揇�i���l�����Ȃ��قǑ����X�V�����j
	int m_textureWidth;		// �e�N�X�`���̉���
	int m_textureHeight;	// �e�N�X�`���̏c��
	bool m_visible;			// �`����s����
};

