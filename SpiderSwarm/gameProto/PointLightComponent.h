#pragma once
#include "Component.h"
#include "Math.h"

class GameObject;
class Shader;
class Mesh;

class PointLightComponent : public Component
{
public:
	PointLightComponent(GameObject* owner);
	~PointLightComponent();

	// このポイントライトをジオメトリとして描画
	void Draw(Shader* shader, Mesh* mesh);

	// ディフューズカラー
	Vector3 mDiffuseColor;

	// ライトの半径
	float mInnerRadius;
	float mOuterRadius;
};

