#pragma once
#include "Math.h"


class Astar
{
public:
	Astar();
	~Astar();

	void Update();

private:
	Vector3 m_distance;
	class Player* m_player;
	class WeakEnemy* m_enemy;
};