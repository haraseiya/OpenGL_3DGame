#pragma once
#include "Game.h"

// 衝突情報
struct CollisionInfo
{
	Vector3 mCollisionPoint; // 衝突点
	Vector3 mFixVec;         // 押し戻しベクトル
	Vector3 mHitNormal;      // 衝突点での法線
};

// 球体
struct Sphere
{
	Sphere(const Vector3& center, float radius);
	bool Contains(const Vector3& point) const;                        // 球体内に点が含まれるか？

	Vector3 mCenter;                                                  // 中心位置
	float mRadius;                                                    // 半径
};

// 線分
struct Line
{
	Line();
	Line(const Vector3& start, const Vector3& end);

	Vector3 mLineStart;
	Vector3 mLineEnd;
	Vector3 mNormalizeDirection;
	float   mLineLength;
};

// 軸並行ボックス(Axis-Aligned Bounding Box)
struct AABB
{
	AABB();
	AABB(const Vector3& min, const Vector3& max);                     // AABB初期化用

	// モデル読み込み時の最小最大の点を求めるのに使用             
	void InitMinMax(const Vector3& point) { mMin = mMax = point; } 

	// ボックスの8頂点を再計算する関数
	void CalcVertex();
	void UpdateMinMax(const Vector3& point);                          // 更新処理
	bool Contains(const Vector3& point) const;                        // 点がボックス内に含まれるか
	float MinDistSq(const Vector3& point) const;                      // 点との最小距離を求める
	void Rotate(const Quaternion& q);                                 // 回転
	void SetArrowRotate(bool value) { mIsRotatable = value; }         // 回転を許可するか？
	void Scaling(float x, float y, float z);
	void Scaling(float scale);

	Vector3 mMin;                                                     // ボックス最小座標
	Vector3 mMax;                                                     // ボックス最大座標 
	Vector3 mVertex[8];

	bool    mIsRotatable = false;                                     // 回転するかしないか？
};

// 壁面
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

	static Wall CreateWall(const Vector3& start, const Vector3 end, float wallHeight);  // z軸に水平壁を作成な壁を作成
	static Wall CreateSlope(const Vector3& pos, Wall::AngleEnum zRotateDegree, float horizonLen, float height, float slopeWidth); // 坂道作成

	void    CalcAABB();

	AABB    mWallAABB; // 壁全体を含むAABB 
	Vector3 mNormal; // 壁法線
	Vector3 mStart; // 壁の開始点
	Vector3 mEnd; // 壁の終了点（開始点ー終了点は壁の中心線になる）
	Vector3 mWallVertex[4]; // 壁の頂点
	Vector3 mScale; // 表示用x,y,zスケーリング係数 

	float  mPlaneD;          // 壁面の平面方程式のd値   
	float  mZRotate;         // 表示用z軸回転
	float  mSlopeAngle;      // 傾斜角
};


bool Intersect(const AABB& a, const AABB& b);                         // AABB 同士の衝突
bool Intersect(const Sphere& s, const AABB& box);                     // 球体とAABBとの衝突
bool Intersect(const AABB& a, const Wall& wall, CollisionInfo& info);     // AABBと壁面の衝突を調べ、めり込み戻りベクトルを返す
bool Intersect(const AABB& a, const Vector3& point, CollisionInfo& info);

void calcCollisionFixVec(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec); // AABBのめり込みをもどす量を計算
void calcCollisionFixVecSpring(const AABB& movableBox, const AABB& fixedBox, Vector3& calcFixVec,float deltaTime);
