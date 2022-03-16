#include "GameObjectManager.h"
#include "GameObject.h"
#include "Tag.h"

// �C���X�^���X��������
GameObjectManager* GameObjectManager::mInstance = nullptr;

// �C���X�^���X�𐶐�
void GameObjectManager::CreateInstance()
{
	if (!mInstance)
	{
		mInstance = new GameObjectManager;
	}
}

// �C���X�^���X��j��
void GameObjectManager::Destroy()
{
	delete mInstance;
	mInstance = nullptr;
}

void GameObjectManager::Update(float deltaTime)
{
	//�S�ẴA�N�^�[�̍X�V
	for (auto tag = Tag::BEGIN; tag != Tag::END; ++tag)
	{
		for (auto actor : mGameObjects[tag])
		{
			actor->Update(deltaTime);
		}
	}

	//�y���f�B���O���̃A�N�^�[���A�N�e�B�u�A�N�^�[�Ɉړ�
	for (auto pending : mPendingGameObjects)
	{
		pending->ComputeWorldTransform();
		Tag t = pending->GetTag();

		mGameObjects[t].emplace_back(pending);
	}
	mPendingGameObjects.clear();

	// �S�Ă̎���ł���A�N�^�[���ꎞ�ۊ�
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

	// ����ł���A�N�^�[��delete(mActor���������)
	for (auto actor : deadActors)
	{
		delete actor;
	}
	deadActors.clear();
}

void GameObjectManager::Entry(GameObject* obj)
{
	// ��������y���f�B���O�A�N�^�[�ɒǉ�
	mPendingGameObjects.emplace_back(obj);
}

void GameObjectManager::Exit(GameObject* obj)
{
	//�y���f�B���O�A�N�^�[���ɑ��݂���Ώ���
	auto iter = std::find(mPendingGameObjects.begin(), mPendingGameObjects.end(), obj);
	if (iter != mPendingGameObjects.end())
	{
		//�y���f�B���O�A�N�^�[�̍Ō���Ƀf�[�^���ړ����āA�f�[�^������
		std::iter_swap(iter, mPendingGameObjects.end() - 1);
		mPendingGameObjects.pop_back();
		return;
	}

	// �A�N�e�B�u�A�N�^�[���ɑ��݂���Ώ���
	auto tag = obj->GetTag();
	iter = std::find(mGameObjects[tag].begin(), mGameObjects[tag].end(), obj);
	if (iter != mGameObjects[tag].end())
	{
		//�A�N�e�B�u�A�N�^�[�̍Ō���Ƀf�[�^���ړ����āA�f�[�^����
		std::iter_swap(iter, mGameObjects[tag].end() - 1);
		mGameObjects[tag].pop_back();
	}
}

void GameObjectManager::Initialize()
{

}

void GameObjectManager::Finalize()
{
	// �A�N�^�[�̍폜�@�i�A�N�^�[��ʂ��ăR���|�[�l���g���폜�����j
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
