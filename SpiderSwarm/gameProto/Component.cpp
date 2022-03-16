#include "Component.h"
#include "GameObject.h"

int Component::mGlobalID = 0;

// コンポーネントコンストラクタ
// 引数 : in owner　     コンポーネントが所属する親アクターへのポインタ
//        in updateOrder 更新順序（小さいほど早く実行される）
Component::Component(GameObject* owner, int updateOrder)
	: mOwner(owner)
	, mUpdateOrder(updateOrder)
	, mState(Component::ComponentState::EActive)
{
	// アクターにこのコンポーネントを追加
	mOwner->AddComponent(this);
	mID = mGlobalID;
	mGlobalID++;
}

// コンポーネントデストラクタ
// このコンポーネントが所属する親アクターから削除する
Component::~Component()
{
	mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{

}
