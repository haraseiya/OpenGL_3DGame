#include "EnemySpawnerEffect.h"
#include "Renderer.h"
#include "EffectComponent.h"

EnemySpawnerEffect::EnemySpawnerEffect(const Vector3& pos)
	: EffectBase(Tag::EFFECT)
{
	// エフェクトコンポーネント生成
	mPosition = pos;
	mEffectComponent = new EffectComponent(this, true, false, true);
	mEffectComponent->LoadEffect(u"assets/Effect/MAGICALxSPIRAL/MagicArea.efk");
	Vector3 relativePos(0, 0, 20.0f);
	mEffectComponent->SetRelativePosition(relativePos);
	Matrix4 rot = Matrix4::CreateRotationY(Math::ToRadians(180.0f));
	mEffectComponent->SetRelativeRotate(rot);
	mEffectComponent->SetRelativeScale(300.0f);
}

EnemySpawnerEffect::~EnemySpawnerEffect()
{
}

void EnemySpawnerEffect::LoadEffect()
{
}

void EnemySpawnerEffect::CreateEffect()
{
}
