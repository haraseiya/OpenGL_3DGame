#include "ChantEffect.h"
#include "EffectComponent.h"

ChantEffect::ChantEffect(GameObject* owner,bool loop)
	: EffectBase(Tag::EFFECT)
	, m_lifeCount(0.0f)
{
	// エフェクトコンポーネント生成
	mEffectComponent = new EffectComponent(owner, true, false, loop);
	mEffectComponent->LoadEffect(u"assets/Effect/MAGICALxSPIRAL/MagicArea.efk");
	Vector3 pos(0, 0, 0.1f);
	mEffectComponent->SetRelativePosition(pos);
	Matrix4 rot = Matrix4::CreateRotationY(Math::ToRadians(180.0f));
	mEffectComponent->SetRelativeRotate(rot);
	mEffectComponent->SetRelativeScale(500.0f);
}

ChantEffect::~ChantEffect()
{
}

void ChantEffect::UpdateActor(float deltaTime)
{
}
