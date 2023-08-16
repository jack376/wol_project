#pragma once
#include "GameObject.h"
#include "ObjectPool.h"

class Player;
class Particle;
class BreakableObj : public GameObject
{
protected:
	std::string textureId;
	sf::Sprite sprite;
	sf::RectangleShape shape;
	sf::FloatRect collider;

	int durability = 1;
	float hitTimer = 0.0f;
	float hitCooldown = 1.0f;

	Player* player = nullptr;

	ObjectPool<Particle>* particlePool = nullptr;

public:
	BreakableObj(const std::string& textureId = "", const std::string& n = "")
		: GameObject(n), textureId(textureId) {}
	virtual ~BreakableObj() override {}

	virtual void Init() override {}
	virtual void Release() override {}
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);
	virtual void SetScale(float x, float y);
	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	void SetFlipX(bool flip);
	void SetFlipY(bool flip);

	void SetTexture(const std::string& id);
	void SetTextureRect(const sf::IntRect& rect);

	void SetParticle(sf::Vector2f position, int count);
	void SetParticlePool(ObjectPool<Particle>* pool);

	void OnHitBreakableObj(const std::string& name, int damage = 1);

	void SetPlayer(Player* player) { this->player = player; }
	void SetDurability(int durability) { this->durability = durability; }
	int GetDurability() const { return durability; }
};

