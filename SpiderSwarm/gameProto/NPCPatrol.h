#pragma once
#include "NPCState.h"
#include "NPCBehaviorComponent.h"

class Player;

class NPCPatrol : public NPCState
{
public:
	NPCPatrol(class NPCBehaviorComponent* owner);
	virtual ~NPCPatrol();
	NPCStateEnum Update(float deltaTime) override;

	void           OnEnter() override;
	void           OnExit() override;

private:
	Vector3 mAimDir;				// 目標向きベクトル
	Vector3 mPrevAimDir;			// 以前の目標向きベクトル
	bool    mTurnMode;				// ターン中か？
	float   mAngleSpeed;			// ターンスピード
	float   mWalkSpeed;				// 歩き速度
	float   mWalkRemainTime;		// 歩き残り時間
	int     mTurnNum;				// 旋回回数カウンタ

	const int   mTurnMaxNum = 5;	// 最大ターン回数→見わたし行動に移行
	const float mWalkTime = 3.0f;	// 歩き時間
};