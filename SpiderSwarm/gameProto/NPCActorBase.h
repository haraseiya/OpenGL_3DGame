#pragma once
#include "GameObject.h"
#include "SkeletalMeshComponent.h"
#include "collision.h"
#include <string>
#include <unordered_map>

// 敵クラス状態enum状態に変更があったらstateEnumNameも変更
enum class NPCStateEnum : unsigned char
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
	Die,        // 死亡
	StateNum
};

// 敵トリガーボックス種類
enum class NPCTriggerEnum : unsigned char
{
	ForwardBox,          // 正面
	LeftBox,             // 左側
	RightBox,            // 右
	BackBox,             // 後ろ
	HeadBox,             // 頭上
	GroundBox,           // 足元
	GroundForwardBox     // 足元前方
};

enum class NPCCampEnum :unsigned char
{
	Player,
	Enemy
};



// ステート名を取得するヘルパー関数
const char* GetNPCStateEnumName(NPCStateEnum state);

// 敵ベースクラス
class NPCActorBase : public GameObject
{
public:
	NPCActorBase();
	virtual       ~NPCActorBase() {};
	virtual void   UpdateActor(float deltaTime) = 0;
	bool           IsAnimationPlaying();
	float          PlayAnimation(NPCStateEnum state, float rate = 0.5f);
	//void   OnCollisionEnter(ColliderComponent* other)override;

	// セッター系
	void	SetVelocityVec(Vector3& v) { mVelocityVec = v; }			// 速度ベクトル
	void	SetForwardVec(Vector3& v);									// 前進ベクトル
	void    SetWalkSpeed(float speed) { mWalkSpeed = speed; }			// 歩きスピード
	void    SetTurnSpeed(float speed) { mTurnSpeed = speed; }			// 旋回スピード
	void    SetRunSpeed(float speed) { mRunSpeed = speed; }				// 走りスピード
	void    SetHitPoint(int hitpoint) { mHitPoint = hitpoint; }			// ヒットポイント
	void    SetIsOnGround(bool onGround) { mIsOnGround = onGround; }	// 接地状態の変更
	//void	SetNPCState(NPCStateEnum npcState) { mNowState = npcState; }// 現在の状態
	void    SetTriggerBox(NPCTriggerEnum trigerType, AABB& box);		// トリガーボックスのセット

	// ゲッター系
	const Vector3& GetVelocityVec() const { return mVelocityVec; }	// 速度ベクトルの取得
	const Vector3& GetForwardVec()  const { return mForwardVec; }	// 速度ベクトルの取得
	float          GetWalkSpeed()   const { return mWalkSpeed; }	// 歩きスピードの取得
	float          GetRunSpeed()    const { return mRunSpeed; }		// 走りスピード取得
	float          GetTurnSpeed()   const { return mTurnSpeed; }	// 旋回スピード取得
	int            GetHitPoint()    const { return mHitPoint; }		// 体力の取得
	bool           GetIsOnGround()  const { return mIsOnGround; }	// 接地状態の取得
	//NPCStateEnum GetNPCBehaviorState() const { return mNowState; }	// 現在の状態

	bool           IsHitTrigger(NPCTriggerEnum type);             // TriggerEnumがヒットしているか？
	bool           IsExistTriggerBox(NPCTriggerEnum type);        // TriggerBoxが登録されているか？

	void HitEnemyAttack(const int& power);

protected:
	void           LoadAnimation(std::string& animationFileName, bool loop, NPCStateEnum state);
	void           LoadSkeletalMesh(std::string& gpmeshFileName, std::string& gskelFilename);

	Vector3 mVelocityVec;	// 速度ベクトル
	Vector3 mForwardVec;	// 前進方向ベクトル  
	Vector3 mAcceleration;	// 加速度

	float mWalkSpeed;		// 歩きスピード
	float mRunSpeed;		// 走りスピード
	float mTurnSpeed;		// 旋回スピード
	int mHitPoint;			// 体力
	bool mIsOnGround;		// 地面にいる？

	SkeletalMeshComponent* mSkelMeshComponent;								//メッシュデータ
	std::unordered_map<NPCStateEnum, const class Animation*> mAnimations;	// アニメーションセット
	std::unordered_map<NPCTriggerEnum, class BoxCollider*>   mTrigerBoxs;	// トリガーボックス
};