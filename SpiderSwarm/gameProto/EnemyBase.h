#pragma once
#include "GameObject.h"
#include "SkeletalMeshComponent.h"
#include "collision.h"
#include <string>
#include <unordered_map>

// 敵の種類
enum class EnemyKind :unsigned char
{
	ENEMY_WEAK,
	ENEMY_STRONG,
	ENEMY_BOSS
};

enum class EnemyMovePattern
{
	PATTERN_TRACKING = 0,
	PATTERN_INTERCEPTION,
	PATTERN_ESCAPE,
};

enum class EnemyStateScene :unsigned char
{
	ENEMY_SCENE_TITLE = 0,
	ENEMY_SCENE_GAME
};

// 敵クラス状態enum 状態に変更があったらstateEnumNameも変更
enum class EnemyStateEnum : unsigned char
{
	Invalid,    // 無効なステート
	Spawn,      // 出現
	Idle,       // 待機状態
	Walk,       // 歩く
	Turn,       // ターン
	Run,        // 走る
	LookAround, // 見回す
	Attack1,    // 攻撃１
	Attack2,    // 攻撃２
	Attack3,    // 攻撃３
	GetDamage,  // ダメージ受けた
	Roar,       // 吠える
	Stun,       // 気絶
	Death,        // 死亡
	StateNum	// 状態の総数
};

// 敵トリガーボックス種類
enum class EnemyTriggerEnum : unsigned char
{
	ForwardBox,          // 正面
	LeftBox,             // 左側
	RightBox,            // 右
	BackBox,             // 後ろ
	HeadBox,             // 頭上
	GroundBox,           // 足元
	GroundForwardBox     // 足元前方
};

class BoxCollider;
class Animation;
class VertexArray;
class EnemyBullet;
class Score;

// 敵ベースクラス
class EnemyBase : public GameObject
{
public:
	EnemyBase();
	virtual       ~EnemyBase() {};
	virtual void   UpdateActor(float deltaTime) = 0;
	bool           IsAnimationPlaying();
	void           PlayAnimation(EnemyStateEnum state, float rate = 0.5f);
	//void   OnCollisionEnter(ColliderComponent* other)override;
	virtual void FixCollision(BoxCollider* hitPlayerBox, BoxCollider* hitBox) = 0;

	// セッター系
	void SetVelocityVec(Vector3& v) { mVelocityVec = v; }				// 速度ベクトルセット
	void SetForwardVec(Vector3& v);									// 前進ベクトルセット
	void SetWalkSpeed(float speed) { mWalkSpeed = speed; }			// 歩きスピード
	void SetTurnSpeed(float speed) { mTurnSpeed = speed; }			// 旋回スピード
	void SetRunSpeed(float speed) { mRunSpeed = speed; }				// 走りスピード
	void SetHitPoint(int hitpoint) { mHitPoint = hitpoint; }			// ヒットポイントセット
	void SetIsActive(bool inUse) { mIsActive = inUse; }

	void SetTriggerBox(EnemyTriggerEnum trigerType, AABB& box);       // トリガーボックスのセット
	void SetEnemyStateScene(EnemyStateScene enemyScene) { mEnemyStateScene = enemyScene; }

	void SetScore(int score) { mScore = score; }

	bool GetIsBossDeadFlag() { return mIsBossDead; }
	void SetIsBossDeadFlag(bool isDead) { mIsBossDead = isDead; }

	// ゲッター系
	const Vector3&	GetVelocityVec()	const { return mVelocityVec; }	// 速度ベクトルの取得
	const Vector3&	GetForwardVec()		const { return mForwardVec; }	// 速度ベクトルの取得
	float			GetWalkSpeed()		const { return mWalkSpeed; }	// 歩きスピードの取得
	float			GetRunSpeed()		const { return mRunSpeed; }		// 走りスピード取得
	float			GetTurnSpeed()		const { return mTurnSpeed; }	// 旋回スピード取得
	int				GetHitPoint()		const { return mHitPoint; }		// 体力の取得
	EnemyKind		GetEnemyKind()		const { return mEnemyKind; }	// 敵の種類取得
	EnemyStateScene GetEnemySceneState()const { return mEnemyStateScene; }
	bool GetIsActive() const { return mIsActive; }

	const EnemyStateEnum GetNowState()	const { return mNowState; }
	virtual void	SetAttackHitBox(float scale = 1.0f) = 0;

	bool IsHitTrigger(EnemyTriggerEnum type);             // TriggerEnumがヒットしているか？
	bool IsExistTriggerBox(EnemyTriggerEnum type);        // TriggerBoxが登録されているか？

	// ステート名を取得するヘルパー関数
	const char* GetEnemyStateEnumName(EnemyStateEnum state);

	// ボイド関連
	Vector3 KeepDistance();					// 距離を保つ
	Vector3 MatchHeading();					// 先頭に合わせる
	Vector3 SteerToCenter();				// 中心によろうとする
	float CanISee(EnemyBase* enemy);		// 仲間が見えるか
	int SeeFriends(EnemyBase* firstBoid);	// 仲間を探す
	void ClearVisibleList();				// 見えている仲間リストを初期化
	void AddToVisibleList(EnemyBase* enemy);// 見えている仲間をリストに追加
	virtual void SetCollider() = 0;
	virtual void RemoveCollider() = 0;

	//virtual EnemyBase* Clone() = 0;		// 複製関数

protected:
	void           LoadAnimation(std::string& animationFileName, bool loop, EnemyStateEnum state);
	void           LoadSkeletalMesh(std::string& gpmeshFileName, std::string& gskelFilename);

	EnemyStateEnum mNowState;		// 現在のステート
	EnemyKind		mEnemyKind;		// 敵の種類
	EnemyStateScene	mEnemyStateScene;	// シーン毎の敵の挙動
	BoxCollider* mHitBox;			// 自身の衝突判定
	EnemyBullet* mEnemyBullet;		// 敵の弾

	Vector3        mVelocityVec;	// 速度ベクトル
	Vector3        mForwardVec;		// 前進方向ベクトル  
	float          mWalkSpeed;		// 歩きスピード
	float          mRunSpeed;		// 走りスピード
	float          mTurnSpeed;		// 旋回スピード
	int            mHitPoint;		// 体力
	bool	mIsActive;					// このオブジェクトは使用中か
	SkeletalMeshComponent* mSkelMeshComponent; //メッシュデータ
	Mesh* mMesh;

	std::unordered_map<EnemyStateEnum, const Animation*> mAnimations; // アニメーションセット
	std::unordered_map<EnemyTriggerEnum, BoxCollider*>   mTrigerBoxs; // トリガーボックス

	float mFieldOfView;
	const int mMaxFriendsVisible;
	float mShootTimer;

	int mScore;

	bool mIsBossDead;
};