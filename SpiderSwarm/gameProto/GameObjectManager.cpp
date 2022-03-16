#include "GameObjectManager.h"
#include "GameObject.h"
#include "Tag.h"

// インスタンスを初期化
GameObjectManager* GameObjectManager::mInstance = nullptr;

// インスタンスを生成
void GameObjectManager::CreateInstance()
{
	if (!mInstance)
	{
		mInstance = new GameObjectManager;
	}
}

// インスタンスを破棄
void GameObjectManager::Destroy()
{
	delete mInstance;
	mInstance = nullptr;
}

void GameObjectManager::Update(float deltaTime)
{
	//全てのアクターの更新
	for (auto tag = Tag::BEGIN; tag != Tag::END; ++tag)
	{
		for (auto actor : mGameObjects[tag])
		{
			actor->Update(deltaTime);
		}
	}

	//ペンディング中のアクターをアクティブアクターに移動
	for (auto pending : mPendingGameObjects)
	{
		pending->ComputeWorldTransform();
		Tag t = pending->GetTag();

		mGameObjects[t].emplace_back(pending);
	}
	mPendingGameObjects.clear();

	// 全ての死んでいるアクターを一時保管
	std::vector<GameObject*> deadActors;
	for (auto tag = Tag::BEGIN; tag != Tag::END; ++tag)
	{
		for (auto actor : mGameObjects[tag])
		{
			if (actor->GetState() == GameObject::State::STATE_DEAD)
			{
				deadActors.emplace_back(actor);
			}
		}
	}

	// 死んでいるアクターをdelete(mActorからも消去)
	for (auto actor : deadActors)
	{
		delete actor;
	}
	deadActors.clear();
}

void GameObjectManager::Entry(GameObject* obj)
{
	// いったんペンディングアクターに追加
	mPendingGameObjects.emplace_back(obj);
}

void GameObjectManager::Exit(GameObject* obj)
{
	//ペンディングアクター内に存在すれば消す
	auto iter = std::find(mPendingGameObjects.begin(), mPendingGameObjects.end(), obj);
	if (iter != mPendingGameObjects.end())
	{
		//ペンディングアクターの最後尾にデータを移動して、データを消す
		std::iter_swap(iter, mPendingGameObjects.end() - 1);
		mPendingGameObjects.pop_back();
		return;
	}

	// アクティブアクター内に存在すれば消す
	auto tag = obj->GetTag();
	iter = std::find(mGameObjects[tag].begin(), mGameObjects[tag].end(), obj);
	if (iter != mGameObjects[tag].end())
	{
		//アクティブアクターの最後尾にデータを移動して、データ消す
		std::iter_swap(iter, mGameObjects[tag].end() - 1);
		mGameObjects[tag].pop_back();
	}
}

void GameObjectManager::Initialize()
{

}

void GameObjectManager::Finalize()
{
	// アクターの削除　（アクターを通じてコンポーネントも削除される）
	for (auto tag = Tag::BEGIN; tag != Tag::END; ++tag)
	{
		while (!mGameObjects[tag].empty())
		{
			delete mGameObjects[tag].back();
		}
	}

	while (!mPendingGameObjects.empty())
	{
		delete mPendingGameObjects.back();
	}
}

GameObjectManager::~GameObjectManager()
{
}
