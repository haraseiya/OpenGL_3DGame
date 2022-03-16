#include "Math.h"
#include <vector>

class NPCActorBase;

class Boids
{
public:
	Boids(std::vector<NPCActorBase*> m_boids);
	~Boids();

	void Update(std::vector<NPCActorBase*> m_boids);		// 更新

	void Cohesion(std::vector<NPCActorBase*> m_boids);		// 結合
	void Separation(std::vector<NPCActorBase*> m_boids);	// 分離
	void Aligment(std::vector<NPCActorBase*> m_boids);		// 整列

private:
	NPCActorBase* m_npc;

	Vector3 m_pos;			// 位置
 	Vector3 m_velocity;		// 速度
	Vector3 m_acceleration;	// 加速度
	Vector3 m_distance;		// 距離

	int m_index;
};