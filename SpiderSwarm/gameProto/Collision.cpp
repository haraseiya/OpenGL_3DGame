#include "Collision.h"
#include <algorithm>
#include <array>

Sphere::Sphere(const Vector3& center, float radius)
	: mCenter(center)
	, mRadius(radius)
{
}

bool Sphere::Contains(const Vector3& point) const
{
	//���̒��S�Ɠ_�Ƃ̋����̓����v�Z�������̂Ɣ��a��2��Ɣ�r
	float distSq = (mCenter - point).LengthSq();
	return distSq <= mRadius * mRadius;
}

// ����
Line::Line()
	: mLineStart(0.0f, 0.0f, 0.0f)
	, mLineEnd(0.0f, 0.0f, 0.0f)
	, mNormalizeDirection(0.0f, 0.0f, 0.0f)
	, mLineLength(0.0f)
{
}

// ����
Line::Line(const Vector3& start, const Vector3& end)
	: mLineStart(start)
	, mLineEnd(end)
	, mNormalizeDirection(0.0f, 0.0f, 0.0f)
	, mLineLength(0.0f)
{
	Vector3 diff;
	diff = mLineEnd - mLineStart;
	mLineLength = diff.Length();

	mNormalizeDirection = (1.0f / mLineLength) * diff;
}


AABB::AABB()
{
	mMin = Vector3(0, 0, 0);
	mMax = Vector3(0, 0, 0);
}

AABB::AABB(const Vector3& min, const Vector3& max)
	:mMin(min)
	, mMax(max)
{
	CalcVertex();
}

void AABB::CalcVertex()
{
	// ���̏㑤4�_
	mVertex[0] = Vector3(mMin.x, mMin.y, mMax.z);
	mVertex[1] = Vector3(mMax.x, mMin.y, mMax.z);
	mVertex[2] = Vector3(mMin.x, mMax.y, mMax.z);
	mVertex[3] = Vector3(mMax.x, mMax.y, mMax.z);

	// ���̉���4�_
	mVertex[4] = Vector3(mMin.x, mMin.y, mMin.z);
	mVertex[5] = Vector3(mMax.x, mMin.y, mMin.z);
	mVertex[6] = Vector3(mMin.x, mMax.y, mMin.z);
	mVertex[7] = Vector3(mMax.x, mMax.y, mMin.z);
}

void AABB::UpdateMinMax(const Vector3& point)
{
	// x,y,z���ꂼ��̍ő�ŏ������߂�
	mMin.x = Math::Min(mMin.x, point.x);
	mMin.y = Math::Min(mMin.y, point.y);
	mMin.z = Math::Min(mMin.z, point.z);

	mMax.x = Math::Max(mMax.x, point.x);
	mMax.y = Math::Max(mMax.y, point.y);
	mMax.z = Math::Max(mMax.z, point.z);

	CalcVertex();
}

bool AABB::Contains(const Vector3& point) const
{
	bool outside = point.x < mMin.x ||
		point.y < mMin.y ||
		point.z < mMin.z ||
		point.x > mMax.x ||
		point.y > mMax.y ||
		point.z > mMax.z;
	// �������true���Ȃ���΁Abox�̓����ɓ_�����݂���
	return !outside;
}

float AABB::MinDistSq(const Vector3& point) const
{
	// ���ꂼ��̎��ł̍����Ƃ�
	float dx = Math::Max(mMin.x - point.x, 0.0f);
	dx = Math::Max(dx, point.x - mMax.x);
	float dy = Math::Max(mMin.y - point.y, 0.0f);
	dy = Math::Max(dy, point.y - mMax.y);
	float dz = Math::Max(mMin.z - point.z, 0.0f);
	dz = Math::Max(dy, point.z - mMax.z);
	// 3������Ԃł̋����̓��̌������
	return dx * dx + dy * dy + dz * dz;
}

void AABB::Rotate(const Quaternion& q)
{
	// �{�b�N�X��8�̒��_�̔z����
	std::array<Vector3, 8> points;

	// �ŏ��l�͏�ɃR�[�i�[�ł���
	points[0] = mMin;
	// 2�̍ŏ��l��1�̍ő�l�̕��בւ�
	points[1] = Vector3(mMax.x, mMin.y, mMin.z);
	points[2] = Vector3(mMin.x, mMax.y, mMin.z);
	points[3] = Vector3(mMin.x, mMin.y, mMax.z);

	// 2�̍ő�l��1�̍ŏ��l�̕��בւ�
	points[4] = Vector3(mMin.x, mMax.y, mMax.z);
	points[5] = Vector3(mMax.x, mMin.y, mMax.z);
	points[6] = Vector3(mMax.x, mMax.y, mMin.z);

	// �ő�l�͏�ɃR�[�i�[�ł���
	points[7] = Vector3(mMax);

	// �ŏ��̓_����]
	Vector3 p = Vector3::Transform(points[0], q);
	// �ŏ��l�ƍő�l�����߂邽�߂ɍŏ��̓_�����ă��Z�b�g
	mMin = p;
	mMax = p;
	// ��]�ɂ���čő�l�ƍŏ��l�����߂Ȃ����B
	for (size_t i = 1; i < points.size(); i++)
	{
		p = Vector3::Transform(points[i], q);
		UpdateMinMax(p);
	}
}

void AABB::Scaling(float x, float y, float z)
{
	mMax.x *= x;
	mMax.y *= y;
	mMax.z *= z;

	mMin.x *= x;
	mMin.y *= y;
	mMin.z *= z;
}

void AABB::Scaling(float scale)
{
	Scaling(scale, scale, scale);
}

// AABB�ǂ����̏Փ�
bool Intersect(const AABB& a, const AABB& b)
{
	bool no = a.mMax.x < b.mMin.x ||
		a.mMax.y < b.mMin.y ||
		a.mMax.z < b.mMin.z ||
		b.mMax.x < a.mMin.x ||
		b.mMax.y < a.mMin.y ||
		b.mMax.z < a.mMin.z;
	// ���true���Ȃ���΁A��͏Փ˂��Ă���
	return !no;
}

//����AABB�̏Փ�
bool Intersect(const Sphere& s, const AABB& box)
{
	float distSq = box.MinDistSq(s.mCenter);
	return distSq <= (s.mRadius * s.mRadius);
}

// �ǂ̃t�`�̐��ƃ{�b�N�X���Փ˂������𔻒肷��
// 2D�I��Z�l�𖳎����āAxy���ʏ�ł̔���Ƃǂꂾ���߂荞�񂾂��𔻒肷��
// Intersect (AABB, Wall)���Ŏg�p���邱�Ƃ�z��
bool IntersectWallEdgeFix(const AABB& a, const Vector3& edgeLow, const Vector3& edgeHi, Vector3& fixVec)
{
	fixVec = Vector3(0, 0, 0);

	bool no = a.mMax.x < edgeLow.x ||
		a.mMax.y < edgeLow.y ||
		a.mMax.z < edgeLow.z ||
		edgeLow.x < a.mMin.x ||
		edgeLow.y < a.mMin.y ||
		edgeHi.z < a.mMin.z;
	if (no)
	{
		return false;
	}

	float dx1 = edgeLow.x - a.mMin.x;
	float dx2 = edgeLow.x - a.mMax.x;
	float dy1 = edgeLow.y - a.mMax.y;
	float dy2 = edgeLow.y - a.mMin.y;

	// dx, dy, dz�ɂ��ꂼ���Βl���������ق����Z�b�g����
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;

	// x,y,z�̂����ł��������������ňʒu�𒲐�
	if (Math::Abs(dx) <= Math::Abs(dy))
	{
		if (Math::Abs(dx) > 0.0001f)
		{
			fixVec.x = dx;
		}
	}
	else
	{
		if (Math::Abs(dy) > 0.0001f)
		{
			fixVec.y = dy;
		}
	}
	// �C�������܂�ɏ������Ƃ��̓q�b�g���Ă��Ȃ��Ƃ݂Ȃ�
	if (fixVec.LengthSq() < 0.0001f)
	{
		return false;
	}

	return true;
}

// AABB�ƕǖʂ̏Փˌ��m
bool Intersect(const AABB& a, const Wall& wall, CollisionInfo& info)
{
	// �߂荞�ݖ߂�x�N�g��������
	info.mFixVec = Vector3(0, 0, 0);

	// AABB���m�łԂ���Ȃ��̂ł���Α������^�[��
	if (!Intersect(a, wall.mWallAABB))
	{
		return false;
	}

	// �ǖʂ̃G�b�W�ƏՓ˂����Ƃ�
	Vector3 fix(0, 0, 0);										// �X�^�[�g�_���@�@�@�@�X�^�[�g�_��
	if (IntersectWallEdgeFix(a, wall.mWallVertex[0], wall.mWallVertex[1], fix))
	{
		info.mFixVec = fix;
		info.mCollisionPoint = wall.mWallVertex[0] + fix;
		return true;
	}
	// �I���_���@�@�@�@�@�@�@�I���_��
	if (IntersectWallEdgeFix(a, wall.mWallVertex[3], wall.mWallVertex[2], fix))
	{
		info.mFixVec = fix;
		info.mCollisionPoint = wall.mWallVertex[3] + fix;
		return true;
	}

	// AABB�̒��S�_�͕ǂ̊O���ɂ��邩�H
	Vector3 boxCenter;
	boxCenter = (a.mMin + a.mMax) * 0.5f;

	float planeLength;
	planeLength = Vector3::Dot(wall.mNormal, boxCenter) + wall.mPlaneD;

	// �ǂ̊O���ɂ���Ƃ�
	if (planeLength > 0.0f)
	{
		///////////////////////////////////////////////////////////
		// �ǖʂɑ΂��čł��߂��_�����߂�
		///////////////////////////////////////////////////////////
		Vector3 nearestPoint;

		// ���ʂ���̕����t������8�_���ׂċ��߂�
		std::vector<float> lengths;
		for (int i = 0; i < 8; i++)
		{
			lengths.push_back(Vector3::Dot(wall.mNormal, a.mVertex[i]));
		}
		// �ǖʂɑ΂���AABB�̍ŋߖT�_��������
		auto minitr = std::min_element(lengths.begin(), lengths.end());

		// �{�b�N�X���S�_��ʂ�A�ǖʂƓ����@���̕��ʂ̕������𗧂Ă�
		size_t nearestindex = std::distance(lengths.begin(), minitr);
		nearestPoint = a.mVertex[nearestindex];

		//// �ŋߖT�_��Wall��AABB���Ɋ܂܂�邩�H
		//if (!Intersect(wall.mWallAABB, nearestPoint, info))
		//{
		//	return true;
		//}

		// �ŋߖT�_�Ǝ��ۂ̕ǂ̕��ʕ������ŏՓ˔�����s��
		planeLength = Vector3::Dot(wall.mNormal, nearestPoint) + wall.mPlaneD;

		if (planeLength < 0.0f)
		{
			//�Ԃ������̂ŏՓ˓_�E�����߂��ʂ�Ԃ�
			info.mFixVec = -1.0f * planeLength * wall.mNormal;
			info.mCollisionPoint = nearestPoint + info.mFixVec;

			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////
// AABB vs �_
//////////////////////////////////////////////////////////////
bool Intersect(const AABB& a, const Vector3& movablepoint, CollisionInfo& info)
{
	info.mCollisionPoint = Vector3(0, 0, 0);
	info.mFixVec = Vector3(0, 0, 0);

	if (!a.Contains(movablepoint))
	{
		return false;
	}
	float dx1 = movablepoint.x - a.mMin.z;
	float dx2 = movablepoint.x - a.mMin.x;
	float dy1 = movablepoint.y - a.mMax.x;
	float dy2 = movablepoint.y - a.mMin.y;
	float dz1 = movablepoint.z - a.mMax.y;
	float dz2 = movablepoint.z - a.mMax.z;

	// dx, dy, dz�ɂ��ꂼ���Βl���������ق����Z�b�g����
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
	float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

	// x,y,z�̂����ł��������������ňʒu�𒲐�
	if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
	{
		info.mFixVec.x = dx;
	}
	else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
	{
		info.mFixVec.y = dy;
	}
	else
	{
		info.mFixVec.z = dz;
	}
	info.mCollisionPoint = movablepoint + info.mFixVec;

	return true;
}

////////////////////////////////////////////////////////////////////
// Box vs Box
// �Փ˂������Ƃ��m�肵���Ƃ��A�߂荞�݂�߂��֐�
// in    movableBox �ړ����� (ex �v���[���[)
// in    fixedBox   �ړ����Ȃ����́iex �u���b�N�j
// inout calcFixVec �ړ����̂̕␳�����x�N�g��
////////////////////////////////////////////////////////////////////
void calcCollisionFixVec(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec)
{
	calcFixVec = Vector3(0, 0, 0);
	float dx1 = fixedBox.mMin.x - movableBox.mMax.x;
	float dx2 = fixedBox.mMax.x - movableBox.mMin.x;
	float dy1 = fixedBox.mMin.y - movableBox.mMax.y;
	float dy2 = fixedBox.mMax.y - movableBox.mMin.y;
	float dz1 = fixedBox.mMin.z - movableBox.mMax.z;
	float dz2 = fixedBox.mMax.z - movableBox.mMin.z;

	// dx, dy, dz �ɂ� ���ꂼ��1,2�̂�����Βl�������������Z�b�g����
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
	float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

	// x, y, z�̂����ł��������������ňʒu�𒲐�
	if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
	{
		calcFixVec.x = dx;
	}
	else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
	{
		calcFixVec.y = dy;
	}
	else
	{
		calcFixVec.z = dz;
	}
}

void calcCollisionFixVecSpring(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec,float deltaTime)
{
	calcFixVec = Vector3(0, 0, 0);
	float dx1 = fixedBox.mMin.x - movableBox.mMax.x;
	float dx2 = fixedBox.mMax.x - movableBox.mMin.x;
	float dy1 = fixedBox.mMin.y - movableBox.mMax.y;
	float dy2 = fixedBox.mMax.y - movableBox.mMin.y;
	float dz1 = fixedBox.mMin.z - movableBox.mMax.z;
	float dz2 = fixedBox.mMax.z - movableBox.mMin.z;

	// dx, dy, dz �ɂ� ���ꂼ��1,2�̂�����Βl�������������Z�b�g����
	float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
	float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;
	float dz = (Math::Abs(dz1) < Math::Abs(dz2)) ? dz1 : dz2;

	// x, y, z�̂����ł��������������ňʒu�𒲐�
	if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
	{
		calcFixVec.x = dx;
	}
	else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
	{
		calcFixVec.y = dy;
	}
	else
	{
		calcFixVec.z = dz;
	}
}

/////////////////////////////////////////////////////////////
// �Ǎ\����
/////////////////////////////////////////////////////////////
Wall::Wall()
	: mNormal(0, 0, 0)
	, mStart(0, 0, 0)
	, mEnd(0, 0, 0)
	, mPlaneD(0.0f)
	, mScale(0, 0, 0)
	, mZRotate(0.0f)
	, mSlopeAngle(0.0f)
{
	for (int i = 0; i < 4; i++)
	{
		mWallVertex[i] = Vector3(0, 0, 0);
	}
}

// �_�͕ǂɊ܂܂�Ă��邩�H
// AABB�Ɋ܂܂�A���ǂ̖@���x�N�g���̔��Α��Ɋ܂܂�Ă��邩�𒲂ׂ�
bool Wall::Contains(const Vector3& point)
{
	// AABB�ɂ��܂܂�Ȃ��ꍇ�͑������^�[��
	if (!mWallAABB.Contains(point))
	{
		return false;
	}

	// ���ʂ̕����� P�EN + D = 0 ���
	if (Vector3::Dot(mNormal, point) + mPlaneD > 0.0f)
	{
		return false;
	}

	return true;
}

// �ǖʂ��쐬����
// �ǖʂ�z���ɐ����ł���K�v������Bstart-end��Z�l������ł���K�v������
// �����Z�l�ɂ��Ȃ������ꍇ�͖���`
// start - end �x�N�g���͕ǂ̒�Ӄx�N�g���AwallHeight���������������ǂƂ��Ē�`����B
Wall Wall::CreateWall(const Vector3& start, const Vector3 end, float wallHeight)
{
	Wall temp;

	temp.mStart = start;
	temp.mEnd = end;

	// �ǖʂ̕\�ʃx�N�g�������߂�
	Vector3 wallLine = end - start;
	Vector3 nomalizedWallLine = wallLine;
	nomalizedWallLine.Normalize();

	// �ǖʂ̕��ʕ�����
	temp.mNormal = Vector3::Cross(Vector3::UnitZ, nomalizedWallLine);
	temp.mPlaneD = -1.0f * Vector3::Dot(temp.mNormal, start);

	// �ǖʂ�4�_�����߂�(start�_�̉����とend�_�̏と���̏�)
	Vector3 zMax(0.0f, 0.0f, start.z + wallHeight);

	temp.mWallVertex[0] = start;
	temp.mWallVertex[1] = start + zMax;
	temp.mWallVertex[2] = end + zMax;
	temp.mWallVertex[3] = end;

	temp.mScale.x = 100.0f;
	temp.mScale.y = (end - start).Length();
	temp.mScale.z = wallHeight;

	// �\����]�p�Ɖ�]����
	Vector3 sgnVec = Vector3::Cross(Vector3::UnitX, nomalizedWallLine);
	float zAngle = acosf(Vector3::Dot(Vector3::UnitX, nomalizedWallLine)) - Math::PiOver2;

	temp.mZRotate = sgnVec.z > 0 ? zAngle : -zAngle + Math::Pi;
	temp.CalcAABB();

	return temp;
}

// �X���[�v���쐬����
Wall Wall::CreateSlope(const Vector3& pos, Wall::AngleEnum zRotateDegree, float horizonLen, float height, float slopeWidth)
{
	Wall temp;
	Vector3 slopeLine, normalizedSlopeLine;

	// �X���[�v�ʂ��ォ�猩���������A�E���ɂȂ�x�N�g��
	Vector3 slopeRightVec(0.0f, 1.0f, 0.0f);

	temp.mStart = Vector3(0, 0, height);
	temp.mEnd = Vector3(horizonLen, 0, 0);

	// �X���[�v�̕\�ʃx�N�g�������߂�
	slopeLine = temp.mEnd - temp.mStart;
	normalizedSlopeLine = slopeLine;
	normalizedSlopeLine.Normalize();

	// �ǖʖ@���x�N�g�����߂�
	temp.mNormal = Vector3::Cross(normalizedSlopeLine, slopeRightVec);
	temp.mNormal.Normalize();

	//�X�Ίp�����߂�
	temp.mSlopeAngle = acosf(Vector3::Dot(Vector3::UnitX, normalizedSlopeLine)) + Math::PiOver2;

	// �X���[�v�ʂ�4���_�����߂�
	float halfWidth = slopeWidth * 0.5f;
	temp.mWallVertex[0] = temp.mStart - slopeRightVec * halfWidth;
	temp.mWallVertex[1] = temp.mStart + slopeRightVec * halfWidth;
	temp.mWallVertex[2] = temp.mEnd + slopeRightVec * halfWidth;
	temp.mWallVertex[3] = temp.mEnd - slopeRightVec * halfWidth;

	temp.mScale.x = 100.0f;
	temp.mScale.y = slopeWidth;
	temp.mScale.z = (temp.mEnd - temp.mStart).Length();

	temp.mZRotate = Math::ToRadians(static_cast<float>(zRotateDegree));

	// �X���[�v�ʂ�4���_�̃��[�J��Z��] + ���s�ړ�
	Matrix4 rot = Matrix4::CreateRotationZ(Math::ToRadians(static_cast<float>(zRotateDegree)));
	Matrix4 trans = Matrix4::CreateTranslation(pos);
	Matrix4 mat = rot * trans;
	for (int i = 0; i < 4; i++)
	{
		temp.mWallVertex[i] = Vector3::Transform(temp.mWallVertex[i], mat);
	}
	// �@������]
	temp.mNormal = Vector3::Transform(temp.mNormal, rot);
	temp.mPlaneD = -1.0f * Vector3::Dot(temp.mNormal, temp.mWallVertex[0]);

	temp.CalcAABB();
	return temp;
}

void Wall::CalcAABB()
{
	// x,y,z�̍ő�l�@�ŏ��l�𒲂ׂ�
	mWallAABB.mMin = mWallAABB.mMax = mWallVertex[0];
	for (int i = 1; i < 4; i++)
	{
		mWallAABB.mMin.x = Math::Min(mWallAABB.mMin.x, mWallVertex[i].x);
		mWallAABB.mMin.y = Math::Min(mWallAABB.mMin.y, mWallVertex[i].y);
		mWallAABB.mMin.z = Math::Min(mWallAABB.mMin.z, mWallVertex[i].z);

		mWallAABB.mMax.x = Math::Max(mWallAABB.mMax.x, mWallVertex[i].x);
		mWallAABB.mMax.y = Math::Max(mWallAABB.mMax.y, mWallVertex[i].y);
		mWallAABB.mMax.z = Math::Max(mWallAABB.mMax.z, mWallVertex[i].z);
	}
}

