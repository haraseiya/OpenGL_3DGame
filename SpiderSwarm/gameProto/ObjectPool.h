#pragma once
#include <list>
#include <iostream>

class GameObject;

class ObjectPool
{
public:
	ObjectPool(GameObject* obj, int size);
	virtual ~ObjectPool();

	// �w�肵���T�C�Y�Ńv�[���𖄂߂�
	void Fill(GameObject* obj,int size);

	// ���X�g���̃I�u�W�F�N�g�̃C���X�^���X��Ԃ�
	std::unique_ptr<GameObject> GetGameObject();

	// ���X�g���̑S�ẴI�u�W�F�N�g���N���A
	void Clear();

	// �C���X�^���X���I�u�W�F�N�g���X�g�ɖ߂�
	void ReturnToPool(std::unique_ptr<GameObject> returningObject);

protected:
	GameObject* mObject;
	std::list<std::unique_ptr<GameObject>> mObjectPool;	// �I�u�W�F�N�g�v�[���p�̃��X�g�𐶐�

	int mSize;
};

