#include "stdafx.h"
#include "AnimationProjectile.h"
#include "ResourceMgr.h"

AnimationProjectile::AnimationProjectile(const std::string& textureId, const std::string& n)
{
}

AnimationProjectile::~AnimationProjectile()
{
}

void AnimationProjectile::Init()
{
	Projectile::Init();
}

void AnimationProjectile::Release()
{
	Projectile::Release();
}

void AnimationProjectile::Reset()
{
	Projectile::Reset();	
}

void AnimationProjectile::Update(float dt)
{
	Projectile::Update(dt);
	animation.Update(dt);
}

void AnimationProjectile::Draw(sf::RenderWindow& window)
{
	Projectile::Draw(window);
}

void AnimationProjectile::AddClip(const std::string path)
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip(path));
}

void AnimationProjectile::Play(const std::string name)
{
	animation.Play(name);
}
