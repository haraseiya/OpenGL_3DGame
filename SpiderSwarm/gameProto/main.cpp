#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
#include "Game.h"
#include "Renderer.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "InstanceMeshManager.h"

int main(int argc, char** argv)
{
	if (!GAMEINSTANCE.Initialize(1280, 768, false)) return -1;


	GAMEINSTANCE.GetRenderer()->GetInstanceMeshManager()->
		SetInstanceMesh(GAMEINSTANCE.GetRenderer()->GetMesh("Assets/Weapon/Bullet/PlayerBullet.gpmesh"),InstanceType::PlayerBullet1,3000);
	GAMEINSTANCE.GetRenderer()->GetInstanceMeshManager()->
		SetInstanceMesh(GAMEINSTANCE.GetRenderer()->GetMesh("Assets/Weapon/Bullet/EnemyBullet.gpmesh"), InstanceType::EnemyBullet1, 3000);
	GAMEINSTANCE.GetRenderer()->GetInstanceMeshManager()->
		SetInstanceMesh(GAMEINSTANCE.GetRenderer()->GetMesh("Assets/Item/ExperienceItem.gpmesh"), InstanceType::EnergyCapsule, 500);
	GAMEINSTANCE.GetRenderer()->GetInstanceMeshManager()->
		SetInstanceMesh(GAMEINSTANCE.GetRenderer()->GetMesh("Assets/Item/HealItem.gpmesh"), InstanceType::HealCapsule, 500);

	GAMEINSTANCE.setFirstScene(new TitleScene);
	GAMEINSTANCE.GetRenderer()->SetWindowTitle("Spider Swarm");
	GAMEINSTANCE.Run();
	GAMEINSTANCE.Shutdown();
	return 0;
}