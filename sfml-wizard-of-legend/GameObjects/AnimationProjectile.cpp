#include "stdafx.h"
#include "AnimationProjectile.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "Tile.h"
#include "Player.h"
#include "Particle.h"

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
	sortLayer = 24;
}

void AnimationProjectile::Update(float dt)
{
	SpriteGo::Update(dt);
	animation.Update(dt);

	if (type == Type::Meteor)
	{
		collisionTimer += dt;
		sf::Vector2f currentPosition = Utils::Lerp(startPos, endPos, collisionTimer / collisionDuration);
		SetPosition(currentPosition);
		if (currentPosition == endPos)
		{
			if (sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
			{
				player->SetHp(-10);
			}

			type = Type::None;
			SetActive(false);
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);
			SetParticle(position + sf::Vector2f(150, 100), 100);
		}
		return;
	}

	if (!isFire)
		return;

	collider.SetColSize();
	position += direction * speed * dt;
	SetOrigin(origin);
	SetPosition(position);
	SetRotation(direction);

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

void AnimationProjectile::MeteorFire(const sf::Vector2f start, const sf::Vector2f end, int damage)
{
	SetActive(true);
	SetOrigin(origin);

	startPos = start;
	endPos = end;

	type = Type::Meteor;
	this->damage = damage;
}


void AnimationProjectile::SetParticle(sf::Vector2f position, int count)
{
	for (int i = 0; i < count; i++)
	{
		Particle* particle = particlePool->Get();
		particle->SetPosition(position);
		SCENE_MGR.GetCurrScene()->AddGo(particle);
	}
}

void AnimationProjectile::SetParticlePool(ObjectPool<Particle>* pool)
{
	particlePool = pool;
};