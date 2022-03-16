#include "FPSCounter.h"
#include "SDL/SDL.h"
#include <stdio.h>

FPSCounter::FPSCounter(const float maxFps)
	: mSetMaxFps(maxFps)
	, mOneFrameTickCount(1000 / mSetMaxFps)
	, mFpsCount(0)
	, mFrameStartTickTime(0)
	, mFps(0)
	, mBeforetickCount(0)
	, mDeltaTime(0)
{
}

FPSCounter::~FPSCounter()
{
}

/*
@brief	フレーム毎の処理
*/
void FPSCounter::Update()
{
	Wait();
	mDeltaTime = (SDL_GetTicks() - mBeforetickCount) / 1000.0f;

	if (mDeltaTime >= 0.10f)
	{
		mDeltaTime = 0.10f;
	}
	mBeforetickCount = static_cast<float>(SDL_GetTicks());

	//1フレーム目の時刻を保存
	if (mFpsCount == 0)
	{
		mFrameStartTickTime = static_cast<float>(SDL_GetTicks());
	}
	//設定したフレーム数が経過したら
	if (mFpsCount == mSetMaxFps)
	{
		int nowTickTime = SDL_GetTicks();
		mFps = 1000 / ((nowTickTime - mFrameStartTickTime) / mSetMaxFps);
		mFpsCount = 0;
	}
	else
	{
		mFpsCount++;
	}
}

/*
@brief	FPSが設定値より大きくならないための制御
*/
void FPSCounter::Wait()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mBeforetickCount + mOneFrameTickCount));
}
