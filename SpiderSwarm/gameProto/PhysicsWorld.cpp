#include <typeinfo>
#include <algorithm>

#include "PhysicsWorld.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Shader.h"
#include "PlayerBase.h"
#include "EnemyBase.h"
#include "NPCActorBase.h"
#include "NPCState.h"
#include "Collision.h"
#include "BoxCollider.h"
#include "WallCollider.h"

PhysicsWorld::PhysicsWorld()
	: mBoolDebugMode(false)
{
	printf("PysicsWorld �쐬\n");

	// �����R���|�[�l���g�z��̊m��
	mBGBoxs.reserve(256);

	// �{�b�N�X���_�̏�����
	InitBoxVertices();

	// �f�o�b�O�p���C���̃V�F�[�_�[�ǂݍ���
	mLineShader = new Shader();
	mLineShader->Load("shaders/LineWorld.vert", "shaders/Line.frag");

	// �����蔻��{�b�N�X�̃��C���z��
	mLineColors.emplace_back(Color::White);
	mLineColors.emplace_back(Color::Red);
	mLineColors.emplace_back(Color::Green);
	mLineColors.emplace_back(Color::Blue);
	mLineColors.emplace_back(Color::Yellow);
	mLineColors.emplace_back(Color::LightYellow);
	mLineColors.emplace_back(Color::LightBlue);
	mLineColors.emplace_back(Color::LightPink);
	mLineColors.emplace_back(Color::LightGreen);
}

PhysicsWorld::~PhysicsWorld()
{
	printf("PysicsWorld �j��\n");
	mBGBoxs.clear();
	delete mLineShader;
}

// �f�o�b�O�p�@�{�b�N�X���X�g�\��
void PhysicsWorld::DebugShowBoxLists()
{
	if (mBGBoxs.size())
	{
		printf("\n-----------------PhysicsList--BGLists---------------\n");
		for (auto m : mBGBoxs)
		{
			Vector3 pos = m->GetOwner()->GetPosition();
			printf("%6d ", m->mGlobalID);
			printf("(% 7.2f,% 7.2f % 7.2f)-", pos.x, pos.y, pos.z);
			printf("[%p]\n", m->GetOwner());
		}
	}
}

// �����蔻��
void PhysicsWorld::Collision()
{
	// �Е��������A�N�V������Ԃ������蔻��e�X�g
	for (auto reactionPair : mOneSideReactions)
	{
		OneReactionMatch(reactionPair);
	}

	// �������A�N�V������Ԃ������蔻��Z�b�g
	for (auto reactionPair : mDualReactions)
	{
		DualReactionMatch(reactionPair);
	}

	// �����Ɠ������X�g�̓����蔻��Z�b�g
	for (auto t : mSelfReactions)
	{
		SelfReactionMatch(t);
	}
}

// �R���C�_�[�̒ǉ�
void PhysicsWorld::AddCollider(ColliderComponent* collider)
{
	Tag t = collider->GetTag();
	mColliders[t].emplace_back(collider);
}

// �R���C�_�[�̔j��
void PhysicsWorld::RemoveCollider(ColliderComponent* collider)
{
	Tag t = collider->GetOwner()->GetTag();
	// �^�O���猟�����č폜
	std::vector<ColliderComponent*>::iterator iter = std::find(mColliders[t].begin(), mColliders[t].end(), collider);
	if (iter != mColliders[t].end())
	{
		mColliders[t].erase(iter);
		return;
	}
}

// �����蔻��̉����i�f�o�b�O�p�j
void PhysicsWorld::DebugShowBox()
{
	// �f�o�b�O���[�h���H
	if (!mBoolDebugMode)
	{
		return;
	}

	// AABB�`�揀��
	Matrix4 scale, trans, world, view, proj, viewProj;
	view = RENDERER->GetViewMatrix();
	proj = RENDERER->GetProjectionMatrix();
	viewProj = view * proj;
	mLineShader->SetActive();
	mLineShader->SetMatrixUniform("uViewProj", viewProj);

	// �����蔻��{�b�N�X�`�� tag���ɐF��ς��Ă��ׂẴ��X�g�\��
	int colorCount = 0;
	size_t colorNum = mLineColors.size();
	for (auto t = Tag::BEGIN; t != Tag::END; ++t)
	{
		DrawCollisions(mColliders[t], mLineColors[colorCount % colorNum]);
		colorCount++;
	}
}

// �����蔻��̕`��
void PhysicsWorld::DrawBoxs(std::vector<BoxCollider*>& boxs, const Vector3& color)
{
	// �s��Q
	Matrix4 scaleMat, posMat, worldMat;
	Vector3 scale, pos;

	mLineShader->SetVectorUniform("uColor", color);
	for (auto item : boxs)
	{
		AABB box;
		Vector3 min, max;
		box = item->GetWorldBox();

		// �{�b�N�X�̃X�P�[���ƈʒu���擾
		min = box.mMin;
		max = box.mMax;
		scale = max - min;
		pos = min;

		scaleMat = Matrix4::CreateScale(scale);
		posMat = Matrix4::CreateTranslation(pos);

		worldMat = scaleMat * posMat;
		mLineShader->SetMatrixUniform("uWorld", worldMat);

		glBindVertexArray(m_boxVAO);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	}
}

// �Փ˃{�b�N�X�̕`��
void PhysicsWorld::DrawCollisions(std::vector<class ColliderComponent*>& collisions, const Vector3& color)
{
	Matrix4 scaleMat, posMat, rotMat, worldMat, slopeRot;
	Vector3 scale, pos;

	mLineShader->SetVectorUniform("uColor", color);
	for (auto item : collisions)
	{
		// Box�������ꍇ�̕`��
		if (item->GetColliderType() == ColliderTypeEnum::Box)
		{
			AABB box;
			Vector3 min, max;
			box = dynamic_cast<BoxCollider*>(item)->GetWorldBox();

			// �{�b�N�X�̃X�P�[���ƈʒu���擾
			min = box.mMin;
			max = box.mMax;
			scale = max - min;
			pos = min;

			scaleMat = Matrix4::CreateScale(scale);
			posMat = Matrix4::CreateTranslation(pos);

			worldMat = scaleMat * posMat;
			mLineShader->SetMatrixUniform("uWorld", worldMat);

			glBindVertexArray(mBoxVAO);
			glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
		}
		// Wall�������ꍇ�̕`�� 
		if (item->GetColliderType() == ColliderTypeEnum::Wall)
		{
			WallCollider* wallcol;
			Wall walldata;

			Vector3 scale; // �`��X�P�[�����O�W��
			Vector3 pos; // �`��ʒu
			Vector3 normal; // �ǖ@��

			// WallCollider�ƕǃf�[�^�擾
			wallcol = dynamic_cast<WallCollider*>(item);
			walldata = wallcol->GetWall();
			normal = walldata.mNormal;
			// 4�_�̒��_�����߂�
			for (int i = 0; i < 4; i++)
			{
				pos += walldata.mWallVertex[i];
			}
			pos = pos * 0.25f;

			// �s��
			scaleMat = Matrix4::CreateScale(walldata.mScale);
			rotMat = Matrix4::CreateRotationZ(walldata.mZRotate);
			posMat = Matrix4::CreateTranslation(pos);
			slopeRot = Matrix4::CreateRotationY(walldata.mSlopeAngle);

			worldMat = scaleMat * slopeRot * rotMat * posMat;
			mLineShader->SetMatrixUniform("uWorld", worldMat);

			glBindVertexArray(mSquareVAO);
			glDrawElements(GL_LINES, 10, GL_UNSIGNED_INT, 0);

		}
	}
}

// �����蔻��̏�����
void PhysicsWorld::InitBoxVertices()
{
	// �{�b�N�X���_���X�g
	float vertices[] = {
		0.0f, 0.0f, 0.0f,  // min
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,  // max
		0.0f, 1.0f, 1.0f,
	};
	// �{�b�N�X�̃��C�����X�g
	unsigned int lineList[] = {
		0,1,
		1,2,
		2,3,
		3,0,
		4,5,
		5,6,
		6,7,
		7,4,
		0,4,
		1,5,
		2,6,
		3,7,
	};

	unsigned int vbo, ebo;
	glGenVertexArrays(1, &mBoxVAO);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(mBoxVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineList), lineList, GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void PhysicsWorld::InitSquareVertices()
{
	// �{�b�N�X���_���X�g
	float vertices[] = {  // ���������と��O�と��O��
		0.0f, 0.5f,-0.5f,
		0.0f, 0.5f, 0.5f,
		0.0f,-0.5f, 0.5f,
		0.0f,-0.5f,-0.5f,
		0.0f, 0.0f, 0.0f, // �@���\��
	   -1.0f, 0.0f, 0.0f,
	};
	// �{�b�N�X�̃��C�����X�g
	unsigned int lineList[] = {
		0,1,
		1,2,
		2,3,
		3,0,
		4,5
	};

	// ���_�z�� mSquareVAO�̐ݒ�
	unsigned int vbo, ebo;
	glGenVertexArrays(1, &mSquareVAO);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(mSquareVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineList), lineList, GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

// �Е�����������ꍇ�̓����蔻��
void PhysicsWorld::SetOneSideReactionCollisionPair(Tag noReactionType, Tag reactionType)
{
	collidePairs cp;
	cp.pair1 = noReactionType;
	cp.pair2 = reactionType;

	mOneSideReactions.emplace_back(cp);
}

// ��������������ꍇ�̓����蔻��
void PhysicsWorld::SetDualReactionCollisionPair(Tag reaction1, Tag reaction2)
{
	collidePairs cp;
	cp.pair1 = reaction1;
	cp.pair2 = reaction2;
	mDualReactions.emplace_back(cp);
}


void PhysicsWorld::SetSelfReaction(Tag selfreaction)
{
	mSelfReactions.emplace_back(selfreaction);
}

void PhysicsWorld::ClearOneSidePair()
{
	mOneSideReactions.clear();
}

void PhysicsWorld::ClearDualPair()
{
	mDualReactions.clear();
}

void PhysicsWorld::ClearSelfPair()
{
	mSelfReactions.clear();
}

void PhysicsWorld::ClearAllPair()
{
	ClearOneSidePair();
	ClearDualPair();
	ClearSelfPair();
}

// �Е��������A�N�V�������s�������蔻��g�ݍ��킹�𑍓�����`�F�b�N
void PhysicsWorld::OneReactionMatch(collidePairs cp)
{
	for (auto obj : mColliders[cp.pair1])
	{
		for (auto reactionObj : mColliders[cp.pair2])
		{
			if (obj->CollisionDetection(reactionObj))
			{
				reactionObj->GetOwner()->OnCollisionEnter(reactionObj, obj);
			}
		}
	}
}

// �����Ƃ����A�N�V�������s�������蔻��g�ݍ��킹�𑍃`�F�b�N
void PhysicsWorld::DualReactionMatch(collidePairs cp)
{
	for (auto reaction1 : mColliders[cp.pair1])
	{
		for (auto reaction2 : mColliders[cp.pair2])
		{
			if (reaction1->CollisionDetection(reaction2))
			{
				// ���ꂼ�ꃊ�A�N�V�������N�����A��������̏Փˑ����n��
				reaction1->GetOwner()->OnCollisionEnter(reaction1, reaction2);
			}
		}
	}
}

// �����Ɠ��������蔻��O���[�v�̑��g�ݍ��킹���s��
void PhysicsWorld::SelfReactionMatch(Tag type)
{
	size_t size = mColliders[type].size();

	for (int i = 0; i < size; i++)
	{
		GameObject* obj1 = mColliders[type][i]->GetOwner();

		//�������g�̓����蔻��͍s�킸�A��������`�F�b�N
		for (int j = i + 1; j < size; j++)
		{
			GameObject* obj2 = mColliders[type][j]->GetOwner();

			if (obj1 != obj2)
			{
				if (mColliders[type][i]->CollisionDetection(mColliders[type][j]))
				{
					obj1->OnCollisionEnter(mColliders[type][i], mColliders[type][j]);
					obj2->OnCollisionEnter(mColliders[type][j], mColliders[type][i]);
				}
			}
		}
	}
}
