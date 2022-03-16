#include "PointLightComponent.h"
#include "GameObject.h"
#include "Game.h"
#include "Mesh.h"
#include "Shader.h"

PointLightComponent::PointLightComponent(GameObject* owner)
	: Component(owner)
{
	RENDERER->AddPointLight(this);
}

PointLightComponent::~PointLightComponent()
{
	RENDERER->RemovePointLight(this);
}

void PointLightComponent::Draw(Shader* shader, Mesh* mesh)
{
	Matrix4 scale = Matrix4::CreateScale(mOwner->GetScale() * mOuterRadius / mesh->GetRadius());
	Matrix4 trans = Matrix4::CreateTranslation(mOwner->GetPosition());
	Matrix4 worldTransform = scale * trans;
	
	shader->SetMatrixUniform("uWorldTransform", worldTransform);
	shader->SetVectorUniform("uPointLight.mWorldPos", mOwner->GetPosition());
	shader->SetVectorUniform("uPointLight.mDiffuseColor", mDiffuseColor);
	shader->SetFloatUniform("uPointLight.mInnerRadius", mInnerRadius);
	shader->SetFloatUniform("uPointLight.mOuterRadius", mOuterRadius);

	glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}
