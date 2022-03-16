#pragma once
#include "GameObject.h"
#include "Collision.h"

class LevelActor : public GameObject
{
public:
	LevelActor();
	~LevelActor();

	void UpdateActor(float deltaTime)override;
	void LoadLevel(const char* gpmeshFileName, const char* collisionJsonFileName, Vector3& offset);
};