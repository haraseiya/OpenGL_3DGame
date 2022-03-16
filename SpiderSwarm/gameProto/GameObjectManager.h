#pragma once

#include "Singleton.h"
#include "Tag.h"
#include <unordered_map>

class GameObject;

class GameObjectManager /*:public Singleton<GameObjectManager> */
{
public:
	// �C���X�^���X����
	/*friend class Singleton<GameObjectManager>;*/
	static GameObjectManager* GetInstance()
	{
		return mInstance;
	}

	void Initialize();
	void Finalize();

	static void CreateInstance();
	static void Destroy();

	void Update(float deltaTime);

	void Entry(GameObject* obj);
	void Exit(GameObject* obj);

private:
	GameObjectManager() {}
	virtual ~GameObjectManager();

	static GameObjectManager* mInstance;

	std::unordered_map<Tag, std::vector<GameObject*>> mGameObjects;	// �Q�[���I�u�W�F�N�g�z��
	std::vector<GameObject*> mPendingGameObjects;					// �Q�[���I�u�W�F�N�g�ǉ������p�z��
};