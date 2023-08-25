#include "stdafx.h"
#include "Projectile.h"
#include "Player.h"
#include "SceneMgr.h"
#include "BoxCollider2D.h"
#include "Tile.h"

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
}

void Projectile::Release()
{
	SpriteGo::Release();
	collider.Release();
}

void Projectile::Reset()
{
	SpriteGo::Reset();
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

	if (collider.GetActive())
		collider.Draw(window);

	std::cout << "Pos( " << position.x << ", " << position.y << " ) Origin( " << sprite.getOrigin().x << ", " << sprite.getOrigin().y << " ) Rot( " << sprite.getRotation() << " )\t" <<
		"Pos( " << collider.obbCol.getPosition().x << ", " << collider.obbCol.getPosition().y << " ) Origin( " << collider.obbCol.getOrigin().x << ", " << collider.obbCol.getOrigin().y << " ) Rot( " << collider.obbCol.getRotation() << " )\t"
		<< std::endl;
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
		currentTile = (*worldTiles)[0][0];
	currentTile = (*worldTiles)[rowIndex][columnIndex];
}
