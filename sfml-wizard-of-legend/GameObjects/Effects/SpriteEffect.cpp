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
	animation.SetTarget(&sprite);
	sprite.setScale({ 4.0f, 4.0f });
	SetOrigin(Origins::BC);
	SetActive(false);
}

void SpriteEffect::Update(float dt)
{
	if (animation.IsPlaying())
	{
		animation.Update(dt);
		SetOrigin(Origins::BC);
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

void SpriteEffect::Play(sf::Vector2f pos, sf::Vector2f dir)
{
	this->dir = dir;
	sprite.setRotation(Utils::Angle(this->dir) + 90);
	SetActive(true);
	SetPosition(pos);
	animation.Play(name);
	isPlay = true;
}

void SpriteEffect::SetRotation(sf::Vector2f dir)
{
	sprite.setRotation(Utils::Angle(dir) + 90);
}
