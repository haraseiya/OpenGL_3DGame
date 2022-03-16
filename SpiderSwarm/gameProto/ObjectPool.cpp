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
	// �T�C�Y���̃v�[���𖞂���
	for (int i = 0; i < size; i++)
	{
		std::unique_ptr<GameObject> obj = std::make_unique<GameObject>(object);
		mObjectPool.push_back(std::move(obj));
	}
}

std::unique_ptr<GameObject> ObjectPool::GetGameObject()
{
	// �I�u�W�F�N�g�v�[������Ȃ��
	if (!mObjectPool.empty())
	{
		// �z��̐擪�ɃQ�[���I�u�W�F�N�g���ړ�����
		std::unique_ptr<GameObject> obj = std::move(mObjectPool.front());

		// �擪�Ɉړ������I�u�W�F�N�g�����o��
		mObjectPool.pop_front();
		return obj;
	}
}

// �I�u�W�F�N�g��S�폜
void ObjectPool::Clear()
{
	mObjectPool.clear();
}

// �I�u�W�F�N�g�v�[���ɕԂ�
void ObjectPool::ReturnToPool(std::unique_ptr<GameObject> returningObject)
{
	mObjectPool.emplace_back(std::move(returningObject));
}

