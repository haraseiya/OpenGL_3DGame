#pragma once
#include "Game.h"

// �Փˏ��
struct CollisionInfo
{
	Vector3 mCollisionPoint; // �Փ˓_
	Vector3 mFixVec;         // �����߂��x�N�g��
	Vector3 mHitNormal;      // �Փ˓_�ł̖@��
};

// ����
struct Sphere
{
	Sphere(const Vector3& center, float radius);
	bool Contains(const Vector3& point) const;                        // ���̓��ɓ_���܂܂�邩�H

	Vector3 mCenter;                                                  // ���S�ʒu
	float mRadius;                                                    // ���a
};

// ����
struct Line
{
	Line();
	Line(const Vector3& start, const Vector3& end);

	Vector3 mLineStart;
	Vector3 mLineEnd;
	Vector3 mNormalizeDirection;
	float   mLineLength;
};

// �����s�{�b�N�X(Axis-Aligned Bounding Box)
struct AABB
{
	AABB();
	AABB(const Vector3& min, const Vector3& max);                     // AABB�������p

	// ���f���ǂݍ��ݎ��̍ŏ��ő�̓_�����߂�̂Ɏg�p             
	void InitMinMax(const Vector3& point) { mMin = mMax = point; } 

	// �{�b�N�X��8���_���Čv�Z����֐�
	void CalcVertex();
	void UpdateMinMax(const Vector3& point);                          // �X�V����
	bool Contains(const Vector3& point) const;                        // �_���{�b�N�X���Ɋ܂܂�邩
	float MinDistSq(const Vector3& point) const;                      // �_�Ƃ̍ŏ����������߂�
	void Rotate(const Quaternion& q);                                 // ��]
	void SetArrowRotate(bool value) { mIsRotatable = value; }         // ��]�������邩�H
	void Scaling(float x, float y, float z);
	void Scaling(float scale);

	Vector3 mMin;                                                     // �{�b�N�X�ŏ����W
	Vector3 mMax;                                                     // �{�b�N�X�ő���W 
	Vector3 mVertex[8];

	bool    mIsRotatable = false;                                     // ��]���邩���Ȃ����H
};

// �ǖ�
struct Wall
{
	enum AngleEnum
	{
		Angle0 = 0,
		Angle90 = 90,
		Angle180 = 180,
		Angle270 = 270
	};

	Wall();
	bool Contains(const Vector3& point);

	static Wall CreateWall(const Vector3& start, const Vector3 end, float wallHeight);  // z���ɐ����ǂ��쐬�ȕǂ��쐬
	static Wall CreateSlope(const Vector3& pos, Wall::AngleEnum zRotateDegree, float horizonLen, float height, float slopeWidth); // �⓹�쐬

	void    CalcAABB();

	AABB    mWallAABB; // �ǑS�̂��܂�AABB 
	Vector3 mNormal; // �ǖ@��
	Vector3 mStart; // �ǂ̊J�n�_
	Vector3 mEnd; // �ǂ̏I���_�i�J�n�_�[�I���_�͕ǂ̒��S���ɂȂ�j
	Vector3 mWallVertex[4]; // �ǂ̒��_
	Vector3 mScale; // �\���px,y,z�X�P�[�����O�W�� 

	float  mPlaneD;          // �ǖʂ̕��ʕ�������d�l   
	float  mZRotate;         // �\���pz����]
	float  mSlopeAngle;      // �X�Ίp
};


bool Intersect(const AABB& a, const AABB& b);                         // AABB ���m�̏Փ�
bool Intersect(const Sphere& s, const AABB& box);                     // ���̂�AABB�Ƃ̏Փ�
bool Intersect(const AABB& a, const Wall& wall, CollisionInfo& info);     // AABB�ƕǖʂ̏Փ˂𒲂ׁA�߂荞�ݖ߂�x�N�g����Ԃ�
bool Intersect(const AABB& a, const Vector3& point, CollisionInfo& info);

void calcCollisionFixVec(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec); // AABB�̂߂荞�݂����ǂ��ʂ��v�Z
void calcCollisionFixVecSpring(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec,float deltaTime);
