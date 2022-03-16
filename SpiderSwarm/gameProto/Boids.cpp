#include "Boids.h"
#include "NPCActorBase.h"

Boids::Boids(std::vector<NPCActorBase*> m_boids)
{

}

Boids::~Boids()
{
}

void Boids::Update(std::vector<NPCActorBase*> m_boids)
{
	Cohesion(m_boids);
	Separation(m_boids);
	Aligment(m_boids);
}


void Boids::Cohesion(std::vector<NPCActorBase*> m_boids)
{
	// NPC位置ベクトルの合計
	for (int i = 0; i < m_boids.size(); i++)
	{
		m_pos += m_boids[i]->GetPosition();
	}

	// 群れの中心を求める
	m_pos.x = m_pos.x / m_boids.size() - 1;
	m_pos.y = m_pos.y / m_boids.size() - 1;
	m_pos.z = m_pos.z / m_boids.size() - 1;

	m_velocity += (m_pos - m_boids[m_index]->GetVelocityVec());
	m_boids[m_index]->SetVelocityVec(m_velocity);

	m_index++;
}

void Boids::Separation(std::vector<NPCActorBase*> m_boids)
{
	for (int i = 0; i < m_boids.size(); i++)
	{
		m_distance = m_boids[i]->GetPosition() - m_boids[m_index]->GetPosition();
		
		m_velocity -= m_boids[i]->GetPosition() - m_boids[m_index]->GetPosition();
	}
	m_index++;
}

void Boids::Aligment(std::vector<NPCActorBase*> m_boids)
{
}
