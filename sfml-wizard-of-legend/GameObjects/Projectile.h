#pragma once
#include "SpriteGo.h"
#include "BoxCollider2D.h"
#include "Beam.h"

class Player;
class Tile;

class Projectile :
    public SpriteGo
{
protected:
	sf::Vector2f direction;
	float speed = 0.f;
	int damage = 0;
	bool isAttacked = false;
	bool isFire = false;

	Player* player = nullptr;
	Tile* currentTile = nullptr;
	std::vector<std::vector<Tile*>>* worldTiles = nullptr;

	BoxCollider2D collider;
	Beam raycaster;	
	//충돌한 순간 Bullet에서 쏜 레이에 맞은 충돌체의 위치에 이팩트 발생 
public:
	Projectile(const std::string& textureId = "", const std::string& n = "");
	virtual ~Projectile() override;

	virtual void Init() override; 
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetRotation(const sf::Vector2f dir);
	void SetRotation(const float angle);
	virtual void SetPosition(const sf::Vector2f& p) override;
	virtual void SetPosition(float x, float y) override;
	virtual void SetOrigin(Origins origin) override;
	virtual void SetOrigin(float x, float y) override;

	void SetPlayer(Player* player) { this->player = player; }
	void SetDamage(int num) { damage = num; }
	void SetSpeed(float num) { speed = num; }
	void SetTiles(std::vector<std::vector<Tile*>>* tiles) { this->worldTiles = tiles; }

	int GetDamage() { return damage; }
	float GetSpeed() { return speed; }

	void Fire(const sf::Vector2f pos, const sf::Vector2f direction, float speed, int damage);
	void Fire(const sf::Vector2f direction, float speed, int damage);

	void CalculatorCurrentTile();

	std::function<void()> action;
};

