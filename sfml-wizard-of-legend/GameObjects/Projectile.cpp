#include "stdafx.h"
#include "Projectile.h"
#include "Player.h"
#include "SceneMgr.h"
#include "BoxCollider2D.h"
#include "Tile.h"
#include "SpriteEffect.h"

Projectile::Projectile(const std::string& textureId, const std::string& n)
{
}

Projectile::~Projectile()
{
}

void Projectile::Init()
{
	SpriteGo::Init();
	collider.Init();

	playerHitEffectPool.OnCreate = [this](SpriteEffect* effect) {
		effect->sprite.setScale(3, 3);
		effect->SetAnimId("HitEffect");
		effect->SetDuration(0.5f);
		effect->sortLayer = 20;
	};
	playerHitEffectPool.Init();
}

void Projectile::Release()
{
	SpriteGo::Release();
	collider.Release();
	for (auto obj : playerHitEffectPool.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(obj);
	}
	playerHitEffectPool.Clear();
}

void Projectile::Reset()
{
	SpriteGo::Reset();
	playerHitEffectPool.Clear();
	collider.Reset();
	collider.SetColSize();
	collider.SetSprite(sprite);

	SetPosition(0.f, 0.f);
	SetOrigin(origin);
	sprite.setRotation(0.f);
	sprite.setScale({ 4, 4 });

	isAttacked = false;
	isFire = false;
	speed = 0.f;
	damage = 0;
	direction = { 0.f, 0.f };
}

void Projectile::Update(float dt)
{
	SpriteGo::Update(dt);

	if (!isFire)
		return;

	position += direction * speed * dt;
	SetPosition(position);
	collider.SetPosition(position);

	if (collider.GetActive())
		collider.Update(dt);

	CalculatorCurrentTile();

	if (isActive && player != nullptr && player->IsAlive())
	{
		if (collider.ObbCol(player->rect))
		{
			SpriteEffect* playerHitEffect = playerHitEffectPool.Get();
			sf::Vector2f randPos(Utils::RandomRange(0.f, 10.f), Utils::RandomRange(0.f, 10.f));
			playerHitEffect->SetPosition(player->GetPosition() + randPos);
			float randAngle = Utils::RandomRange(0.f, 360.f);
			playerHitEffect->sprite.setRotation(randAngle);
			SCENE_MGR.GetCurrScene()->AddGo(playerHitEffect);

			player->SetHitDir(direction);
			player->SetHp(-damage);
			isAttacked = true;
			isFire = false;
			SetActive(false);
			collider.SetActive(false);
		}
	}

	if (currentTile->GetType() == TileType::Wall)
	{
		isFire = false;
		SetActive(false);
		collider.SetActive(false);
	}
}

void Projectile::Draw(sf::RenderWindow& window)
{
	window.draw(sprite, &shader);

	//if (collider.GetActive())
	//	collider.Draw(window);
}

void Projectile::SetRotation(const sf::Vector2f dir)
{
	float angle = Utils::Angle(dir);
	sprite.setRotation(angle);
	collider.GetObbCol().setRotation(angle);
}

void Projectile::SetRotation(const float angle)
{
	sprite.setRotation(angle);
	collider.GetObbCol().setRotation(angle);
}

void Projectile::SetPosition(const sf::Vector2f& p)
{
	SpriteGo::SetPosition(p);
	collider.SetPosition(p);
}

void Projectile::SetPosition(float x, float y)
{
	SpriteGo::SetPosition(x, y);
	collider.SetPosition(x, y);
}

void Projectile::SetOrigin(Origins origin)
{
	SpriteGo::SetOrigin(origin);
	collider.SetOrigin(origin);
}

void Projectile::SetOrigin(float x, float y)
{
	SpriteGo::SetOrigin(x, y);
	collider.SetOrigin(x, y);
}

void Projectile::SetShader(std::string filePath, float yOffset)
{
	paletteTexture.loadFromFile(filePath);
	shader.loadFromFile("shader/FragShader.frag", sf::Shader::Fragment);
	shader.setUniform("paletteTexture", paletteTexture);
	shader.setUniform("paletteYOffset", yOffset);
}

void Projectile::Fire(const sf::Vector2f pos, const sf::Vector2f direction, float speed, int damage)
{
	collider.SetActive(true);
	collider.SetColSize();
	SetActive(true);
	SetOrigin(origin);
	SetPosition(pos);
	SetRotation(Utils::Angle(direction) + 90);

	this->direction = direction;
	this->speed = speed;
	this->damage = damage;
	isFire = true;
}

void Projectile::Fire(const sf::Vector2f direction, float speed, int damage)
{
	collider.SetActive(true);
	collider.SetColSize();
	SetActive(true);
	SetOrigin(origin);

	this->direction = direction;
	this->speed = speed;
	this->damage = damage;
	isFire = true;
}

void Projectile::CalculatorCurrentTile()
{
	int rowIndex = position.x / _TileSize;
	int columnIndex = position.y / _TileSize;
	if (rowIndex < 0 || rowIndex >= worldTiles->size() || columnIndex < 0 || columnIndex >= worldTiles[0].size())
	{
		rowIndex = 0;
		columnIndex = 0;
		std::cout << "erro: Projectile::CalculatorCurrentTile() Over Range" << std::endl;
	}	
	currentTile = (*worldTiles)[rowIndex][columnIndex];
}
