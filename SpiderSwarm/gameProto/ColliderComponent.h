#pragma once
#include "Component.h"
#include "Collision.h"
#include "PhysicsWorld.h"
#include "Tag.h"

class GameObject;
class BoxCollider;
class WallCollider;

// �����蔻��̎��
enum class ColliderTypeEnum
{
	Box,
	Wall,
	Sphere,
	Line,
};

// �����蔻���t�^����R���|�[�l���g�N���X
class ColliderComponent : public Component
{
public:
	ColliderComponent(GameObject* owner, ColliderTypeEnum type, int updateOrder = 100);
	virtual ~ColliderComponent() {};

	Tag GetTag();
	void SetInfo(const CollisionInfo& info) { mInfo = info; }
	void ClearInfo();

	ColliderTypeEnum GetColliderType() { return mColliderType; }
	CollisionInfo& GetCollisionInfo() { return mInfo; }

	virtual void OnUpdateWorldTransform() {};

	// Double-Dispatch �p�^�[�����g�p���ďՓ˔���g�ݍ��킹���s��
	virtual bool CollisionDetection(ColliderComponent* other) = 0;

	// Double-Dispatch �p�^�[���FCollider���m�̂ӂ�܂����`
	virtual bool Check(BoxCollider* other);
	virtual bool Check(WallCollider* other);

protected:
	Tag              mTag;                // �����蔻��O���[�v�^�O
	ColliderTypeEnum mColliderType;       // �Փ˃^�C�v
	CollisionInfo    mInfo;               // �Փˎ����

	friend class PhysicsWorld;
};