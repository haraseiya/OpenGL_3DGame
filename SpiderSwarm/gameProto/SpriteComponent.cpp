#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "GameObject.h"
#include "Game.h"
#include "Renderer.h"

SpriteComponent::SpriteComponent(GameObject* _owner, int _drawOrder)
	: Component(_owner)
	, m_texture(nullptr)
	, m_drawOrder(_drawOrder)
	, m_textureWidth(0)
	, m_textureHeight(0)
	, m_visible(true)
{
	// �����_���[��Sprite�|�C���^�[�𑗂�
	RENDERER->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	// �����_���[��Sprite�|�C���^�[���폜����
	RENDERER->RemoveSprite(this);
}

// �`�揈��
// �����F�g�p����V�F�[�_�[�̃|�C���^
void SpriteComponent::Draw(Shader* _shader)
{
	// �摜��񂪋�łȂ����A�e�I�u�W�F�N�g�����X�V��ԂłȂ���
	if (m_texture && mOwner->GetState() != GameObject::STATE_DEAD)
	{
		Matrix4 scaleMatrix = Matrix4::CreateScale(
			static_cast<float>(m_textureWidth),
			static_cast<float>(m_textureHeight),
			1.0f);

		Matrix4 world = scaleMatrix * mOwner->GetWorldTransform();
		_shader->SetMatrixUniform("uWorldTransform", world);

		m_texture->SetActive();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void SpriteComponent::SetTexture(Texture* _texture)
{
	m_texture = _texture;
	m_textureWidth = _texture->GetWidth();
	m_textureHeight = m_texture->GetHeight();
}




