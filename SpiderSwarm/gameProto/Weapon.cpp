#include "Weapon.h"
#include "Game.h"
#include "Renderer.h"
#include "SkeletalMeshComponent.h"
#include "AttachMeshComponent.h"
#include "PlayerBase.h"

Weapon::Weapon(PlayerBase* target)
	: GameObject(Tag::NONE)
{
	//メッシュのロード
	mMesh = RENDERER->GetMesh("Assets/Weapon/AK-47/AK-47.gpmesh");
	mMeshComp = new SkeletalMeshComponent(this);
	mMeshComp->SetMesh(mMesh);
	//mMeshComp->SetSkeleton(RENDERER->GetSkeleton("Assets/Weapon/AK-47/AK-47.gpskel"));
	mAttachMeshComp = new AttachMeshComponent(this, target->GetSkeletalMeshComp(), "hand_l");
}

Weapon::~Weapon()
{
}


