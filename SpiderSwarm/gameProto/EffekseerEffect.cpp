#include "EffekseerEffect.h"
#include "Game.h"
#include "Renderer.h"
#include "Effekseer.h"
#include "EffekseerRendererGL.h"
#include <string>
#include <iostream>

EffekseerEffect::EffekseerEffect()
	: mEffectHandle(nullptr)
	, mIsReady(false)
{
}

EffekseerEffect::~EffekseerEffect()
{
}

bool EffekseerEffect::LoadEffect(const char16_t* filename)
{
	std::cout << filename;

	mEffectHandle = Effekseer::Effect::Create(RENDERER->GetEffekseerManager(), filename);
	if (mEffectHandle == nullptr)
	{
		std::cout << " : Effect File Load Failed." << std::endl;
		mIsReady = false;
	}
	else
	{
		std::cout << " : File Load Success." << std::endl;
		mIsReady = true;
	}
	return mIsReady;
}

Effekseer::Handle EffekseerEffect::CreateInstanceHandle(Vector3& pos)
{
	if (!mIsReady)
	{
		return 0;
	}
	return RENDERER->GetEffekseerManager()->Play(mEffectHandle, Effekseer::Vector3D(pos.x, pos.y, pos.z));
}

void EffekseerEffect::SetPosition(Vector3& pos, Effekseer::Handle handle)
{
	Effekseer::Vector3D v;
	v = pos;
	RENDERER->GetEffekseerManager()->SetLocation(handle, v);
}

void EffekseerEffect::SetRotation(Vector3& axis, float angle, Effekseer::Handle handle)
{
	Effekseer::Vector3D v;
	v = axis;
	RENDERER->GetEffekseerManager()->SetRotation(handle, v, angle);
}

void EffekseerEffect::SetScale(Vector3& scale, Effekseer::Handle handle)
{
	Effekseer::Vector3D v;
	v = scale;
	RENDERER->GetEffekseerManager()->SetScale(handle,v.X,v.Y,v.Z);
}

void EffekseerEffect::SetBaseMatrix(Matrix4& baseMat, Effekseer::Handle handle)
{
	Effekseer::Matrix43 m;
	m = baseMat;
	RENDERER->GetEffekseerManager()->SetBaseMatrix(handle, m);
}
