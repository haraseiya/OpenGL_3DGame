#pragma once
#include <list>
#include <iostream>

class GameObject;

class ObjectPool
{
public:
	ObjectPool(GameObject* obj, int size);
	virtual ~ObjectPool();

	// 指定したサイズでプールを埋める
	void Fill(GameObject* obj,int size);

	// リスト内のオブジェクトのインスタンスを返す
	std::unique_ptr<GameObject> GetGameObject();

	// リスト内の全てのオブジェクトをクリア
	void Clear();

	// インスタンスをオブジェクトリストに戻す
	void ReturnToPool(std::unique_ptr<GameObject> returningObject);

protected:
	GameObject* mObject;
	std::list<std::unique_ptr<GameObject>> mObjectPool;	// オブジェクトプール用のリストを生成

	int mSize;
};

