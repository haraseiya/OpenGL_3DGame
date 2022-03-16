#include "ObjectPool.h"
#include "GameObject.h"

ObjectPool::ObjectPool(GameObject* obj,int size)
	: mObject(obj)
	, mSize(size)
{
}


ObjectPool::~ObjectPool()
{
	Clear();
}

void ObjectPool::Fill(GameObject* object, int size)
{
	// サイズ分のプールを満たす
	for (int i = 0; i < size; i++)
	{
		std::unique_ptr<GameObject> obj = std::make_unique<GameObject>(object);
		mObjectPool.push_back(std::move(obj));
	}
}

std::unique_ptr<GameObject> ObjectPool::GetGameObject()
{
	// オブジェクトプールが空ならば
	if (!mObjectPool.empty())
	{
		// 配列の先頭にゲームオブジェクトを移動する
		std::unique_ptr<GameObject> obj = std::move(mObjectPool.front());

		// 先頭に移動したオブジェクトを取り出す
		mObjectPool.pop_front();
		return obj;
	}
}

// オブジェクトを全削除
void ObjectPool::Clear()
{
	mObjectPool.clear();
}

// オブジェクトプールに返す
void ObjectPool::ReturnToPool(std::unique_ptr<GameObject> returningObject)
{
	mObjectPool.emplace_back(std::move(returningObject));
}

