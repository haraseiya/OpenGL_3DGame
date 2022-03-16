#include "StageWall.h"
#include "BoxCollider.h"
#include "WallCollider.h"

StageWall::StageWall()
	: GameObject(Tag::BACK_GROUND)
{
	std::vector<Vector3> v;
	const int split = 16;
	const float radius = 1000.0f;

	const float angle = Math::TwoPi / split;

	// �~�`�̕ǂ̂��߂̍��W�ۑ��p�z����쐬
	//Vector3 pos;
	//for (int i = 0; i < split; i++)
	//{
	//	pos.x = radius * cosf(angle * i);
	//	pos.y = radius * sinf(angle * i);
	//	pos.z = 0.0f;

	//	v.push_back(pos);
	//}

	WallCollider* wc;
	Wall wall;

	// �z�񂩂�~�`�̕ǂ𗧂Ă�
	//int i;
	//for (i = 0; i < split - 1; ++i)
	//{
	//	wc = new WallCollider(this);
	//	wall = Wall::CreateWall(v[i], v[i + 1], 1000);
	//	wc->SetWall(wall);
	//}

	// �Ђ��`�̕ǂ𗧂Ă�
	wc = new WallCollider(this);
	wall = Wall::CreateWall(Vector3(2300, -2600, 750), Vector3(2300, 2600, 750), 1000);
	wc->SetWall(wall);

	wc = new WallCollider(this);
	wall = Wall::CreateWall(Vector3(2600, 2300, 750), Vector3(-2600, 2300, 750), 1000);
	wc->SetWall(wall);

	wc = new WallCollider(this);
	wall = Wall::CreateWall(Vector3(-2300, 2600, 750), Vector3(-2300, -2600, 750), 1000);
	wc->SetWall(wall);

	wc = new WallCollider(this);
	wall = Wall::CreateWall(Vector3(-2600, -2300, 750), Vector3(2600, -2300, 750), 1000);
	wc->SetWall(wall);

	//// ���ݒ�
	//wc = new WallCollider(this);
	//wall = Wall::CreateSlope(Vector3(1300, 800, 0), Wall::Angle270, 600, 300, 600.0f);
	//wc->SetWall(wall);

	//// �X�e�[�W�S�̂̏���ݒ�
	//AABB groundBox;
	//groundBox.mMax = Vector3(2000, 2000, 0);
	//groundBox.mMin = Vector3(-2000, -2000, -100);

	//BoxCollider* bc = new BoxCollider(this);
	//bc->SetObjectBox(groundBox);
}

StageWall::~StageWall()
{
}
