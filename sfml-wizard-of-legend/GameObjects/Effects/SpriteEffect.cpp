#include "stdafx.h"
#include "SpriteEffect.h"
#include "SceneMgr.h"
#include "Utils.h"
#include "ResourceMgr.h"

SpriteEffect::SpriteEffect(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{

}

void SpriteEffect::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void SpriteEffect::Reset()
{
	SpriteGo::Reset();
	timer = 0.f;
	sprite.setColor({ 255, 255, 255, 255 });
	if(type == EffectTypes::SpeedUp)
		animation.Play("EffectSpeedUp");
}

void SpriteEffect::Update(float dt)
{
	timer += dt;
	//UINT8 a = Utils::Lerp(255, 0, (timer / duration));
	//sprite.setColor({ 255, 255, 255, a });
	//animation.Update(dt);

	if (timer > duration)
	{
		if (pool != nullptr)
		{
			pool->Return(this);
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
		}
		else
		{
			SetActive(false);
		}
	}
	switch (type)
	{
	case EffectTypes::SpeedUp:
		float movePos = GetPosition().x;
		movePos += -speed * dt;
		SetPosition(movePos, GetPosition().y);
		break;
	}

}

void SpriteEffect::SetAnim(const std::string& path)
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip(path));
	animation.SetTarget(&sprite);
}

void SpriteEffect::SetType(const EffectTypes type)
{
	this->type = type;
}
