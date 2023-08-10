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
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip(textureId));
}

void SpriteEffect::Reset()
{
	SpriteGo::Reset();
	SetOrigin(Origins::MC);
	SetActive(false);
}

void SpriteEffect::Update(float dt)
{
	if (isPlay)
		animation.Update(dt);
	if (animation.IsAnimEndFrame())
	{
		SetActive(false);
		isPlay = false;
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

void SpriteEffect::Play(float dt, sf::Vector2f pos, sf::Vector2f dir)
{
	isPlay = true;
	SetActive(true);
	SetPosition(pos);
	sprite.setRotation(Utils::Angle(dir));
}
