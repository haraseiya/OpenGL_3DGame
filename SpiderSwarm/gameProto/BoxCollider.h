#pragma once
#include "Component.h"
#include "ColliderComponent.h"
#include "Collision.h"
#include "PhysicsWorld.h"
#include "Tag.h"

class WallCollider;

class BoxCollider : public ColliderComponent
{
public:
	BoxCollider(class GameObject* owner, int updateOrder = 100);
	~BoxCollider();

	void  OnUpdateWorldTransform();								  // ���[���h�ϊ���
	void  SetObjectBox(const AABB& box);                          // �����蔻��p���E�{�b�N�X���Z�b�g
	const AABB& GetWorldBox() const { return mWorldBox; }         // ���[���h��ԏ�ł̋��E�{�b�N�X���擾
	void  SetArrowRotate(bool value) { mRotatable = value; }      // ��]�������邩�H

	bool CollisionDetection(ColliderComponent* other) ;

protected:
	bool Check(BoxCollider* other);
	bool Check(WallCollider* other);

	AABB mObjectBox;                                              // �I�u�W�F�N�g��ԁi�ϊ��O�j�̃{�b�N�X
	AABB mWorldBox;                                               // ���[���h��Ԃɒu�������̃{�b�N�X
	bool mRotatable;                                              // ��]�������邩�H

	friend class PhysicsWorld;
};