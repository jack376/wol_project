#include "stdafx.h"
#include "AnimationProjectile.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "Tile.h"
#include "Player.h"

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
	animation.SetTarget(&sprite);
}

void AnimationProjectile::Update(float dt)
{
	SpriteGo::Update(dt);
	animation.Update(dt);

	if (!isFire)
		return;
	collider.SetColSize();
	position += direction * speed * dt;
	SetPosition(position);
	SetOrigin(origin);
	collider.SetPosition(position);
	collider.SetOrigin(origin);

	if (collider.GetActive())
	{
		collider.SetSprite(sprite);
		collider.Update(dt);
	}
		
	CalculatorCurrentTile();

	if (isActive && player != nullptr && player->IsAlive())
	{
		if (collider.ObbCol(player->rect))
		{
			player->SetHitDir(direction);
			player->SetHp(-damage);
			isAttacked = true;
			isFire = false;
			SetActive(false);
			collider.SetActive(false);

			if (animation.GetCurrentClipId() == "Fireball")
			{
				SCENE_MGR.GetCurrScene()->RemoveGo(this);
				pool->Return(this);
			}
		}
	}

	if (currentTile->GetType() == TileType::Wall)
	{
		isFire = false;
		SetActive(false);
		collider.SetActive(false);
		if (animation.GetCurrentClipId() == "Fireball")
		{
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);
		}
	}
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

void AnimationProjectile::PlayQueue(const std::string name)
{
	animation.PlayQueue(name);
}
