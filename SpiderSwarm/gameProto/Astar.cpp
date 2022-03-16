#include "Astar.h"
#include "Player.h"
#include "Enemy.h"

Astar::Astar()
	: m_distance(Vector3::Zero)
{
}

Astar::~Astar()
{
}

void Astar::Update(Player& _player,WeakEnemy& _enemy)
{
	m_distance = _enemy.GetPosition() - _player.GetPosition();
	m_distance.x--;
	m_distance.y--;
	m_distance.z = 0;
	_enemy.SetPosition(m_distance);
}
