#include "EffectComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "Renderer.h"
#include "Effekseer.h"
#include "EffekseerEffect.h"

// �G�t�F�N�g�R���|�[�l���g
EffectComponent::EffectComponent(GameObject* owner, bool moveOn, bool rotateOn,bool loop, int updateOrder)
	: Component(owner)
	, mOwner(owner)
	, mIsMove(moveOn)
	, mIsRotate(rotateOn)
	, mIsLoop(loop)
	, mHandle(0)
	, mRelativePos(0, 0, 0)
	, mRelativeScale(0.0f)
{
}


EffectComponent::~EffectComponent()
{
}

// 
void EffectComponent::LoadEffect(const char16_t* effkseerFilename)
{
	mEffect = RENDERER->GetEffect(effkseerFilename);
}

void EffectComponent::CreateEffect()
{
	Vector3 pos = Vector3(0, 0, 0);
	mHandle = mEffect->CreateInstanceHandle(pos);
}


void EffectComponent::Update(float deltaTime)
{
	// �G�t�F�N�g�����݂��Ă��Ȃ��ꍇ
	if (!(RENDERER->GetEffekseerManager()->Exists(mHandle)))
	{
		// ���[�v�t���O�������Ă���ꍇ
		if (mIsLoop)
		{
			Vector3 pos = Vector3(0, 0, 0);
			mHandle = mEffect->CreateInstanceHandle(pos);
			return;
		}
		// ���g���폜
		this->SetState(Component::ComponentState::EDelete);
		return;
	}

	// �I�[�i�[�̈ړ��ɉe�����󂯂邩
	Matrix4 trans, rot;
	if (mIsMove)
	{
		Vector3 ownerPos;
		ownerPos = mOwner->GetPosition();
		trans = Matrix4::CreateTranslation(ownerPos);
	}

	// �I�[�i�[�̉�]�ɉe�����󂯂邩
	if (mIsRotate)
	{
		Quaternion q = mOwner->GetRotation();
		rot = Matrix4::CreateFromQuaternion(q);
	}

	Effekseer::Matrix43 eMat;

	// Effecseer -> GL ��100�{ + Zup �ɍ��킹��
	Matrix4 base, convertScale, convertRot;
	convertScale = Matrix4::CreateScale(mRelativeScale);
	convertRot = Matrix4::CreateRotationX(Math::ToRadians(-90.0f));
	base = convertScale * convertRot * mRelativeRot;

	//�G�t�F�N�g�Ɉړ��E��]�i����΁j�Z�b�g
	Matrix4 relativeMoveMat = Matrix4::CreateTranslation(mRelativePos);
	Matrix4 final;
	final = base * relativeMoveMat * rot * trans;

	final.mat[0][0] *= -1;
	final.mat[1][0] *= -1;
	final.mat[2][0] *= -1;
	final.mat[3][0] *= -1;

	eMat = final;
	RENDERER->GetEffekseerManager()->SetBaseMatrix(mHandle, eMat);

}
