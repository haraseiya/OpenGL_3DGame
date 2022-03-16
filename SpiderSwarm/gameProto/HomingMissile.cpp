#include "HomingMissile.h"
#include "EffectComponent.h"
#include "GameObject.h"

HomingMissile::HomingMissile(GameObject* owner, bool loop)
	: EffectBase(Tag::EFFECT)
{
	// エフェクトコンポーネント生成
	mEffectComponent = new EffectComponent(owner, true, true, loop);
	mEffectComponent->LoadEffect(u"assets/Effect/01_AndrewFM01/Raser.efk");
	Vector3 pos(0, 0, 0.1f);
	Matrix4 rot = Matrix4::CreateRotationZ(Math::ToRadians(-90.0f));
	mEffectComponent->SetRelativeRotate(rot);
	mEffectComponent->SetRelativeScale(100.0f);
}

HomingMissile::~HomingMissile()
{
}
