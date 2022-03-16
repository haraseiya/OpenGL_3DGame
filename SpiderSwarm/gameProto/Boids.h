#include "Math.h"
#include <vector>

class NPCActorBase;

class Boids
{
public:
	Boids(std::vector<NPCActorBase*> m_boids);
	~Boids();

	void Update(std::vector<NPCActorBase*> m_boids);		// �X�V

	void Cohesion(std::vector<NPCActorBase*> m_boids);		// ����
	void Separation(std::vector<NPCActorBase*> m_boids);	// ����
	void Aligment(std::vector<NPCActorBase*> m_boids);		// ����

private:
	NPCActorBase* m_npc;

	Vector3 m_pos;			// �ʒu
 	Vector3 m_velocity;		// ���x
	Vector3 m_acceleration;	// �����x
	Vector3 m_distance;		// ����

	int m_index;
};