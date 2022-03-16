#include "LaserEffect.h"
#include "EffectComponent.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "Collision.h"

LaserEffect::LaserEffect(GameObject* owner)
	: EffectBase(Tag::PLAYER_SPECIAL_SHOT)
	, mOwner(owner)
{
	// エフェクトコンポーネント生成
	mEffectComponent = new EffectComponent(owner, true, true, false);
	mEffectComponent->LoadEffect(u"assets/Effect/01_AndrewFM01/Raser.efk");
	Vector3 pos(0, 0, 0.1f);
	Matrix4 rot = Matrix4::CreateRotationZ(Math::ToRadians(-90.0f));
	mEffectComponent->SetRelativeRotate(rot);
	mEffectComponent->SetRelativeScale(100.0f);

	//// 当たり判定セット
	//mHitBox = new BoxCollider(this);
	//mLaserBox.InitMinMax(mOwner->GetPosition());
	//mLaserBox.Scaling(0.1);
	//mLaserBox.UpdateMinMax(Vector3(100.0f,50.0f,10.0f));
	//mLaserBox.SetArrowRotate(true);
	//mLaserBox.Rotate(mOwner->GetRotation());
	//mHitBox->SetObjectBox(mLaserBox);
}

LaserEffect::~LaserEffect()
{
}

void LaserEffect::UpdateActor(float deltaTime)
{
}

void LaserEffect::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherBox)
{
}

void LaserEffect::LoadEffect()
{
}

void LaserEffect::CreateEffect()
{
}
