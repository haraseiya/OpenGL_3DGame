#pragma once
#include "CameraActor.h"

class GameObject;

class LookDownCamera : public CameraActor
{
public:
	LookDownCamera(GameObject *targetActor);
	~LookDownCamera();
	void UpdateActor(float deltaTime) override;

private:
	float mCameraHeight;
	float mCameraBack;
};