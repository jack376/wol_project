#pragma once
#include "GameObject.h"
#include "ObjectPool.h"

class Player;
class Particle;
class DecoGo : public GameObject
{
protected:
	std::string textureId;
	sf::Sprite sprite;

	std::string glowId = { "graphics/effect/Glow.png" };
	sf::Sprite glow;

	sf::RectangleShape shape;
	sf::FloatRect collider;

	int durability = 1;

	float hitTimer    = 0.0f;
	float hitCooldown = 0.5f;

	bool isGlow = false;

	Player* player = nullptr;

	ObjectPool<Particle>* particlePool = nullptr;
	
public:
	DecoGo(const std::string& textureId = "", const std::string& n = "")
		: GameObject(n), textureId(textureId) {}
	virtual ~DecoGo() override {}

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

	void OnHitDecoGo(const std::string& name, int damage = 1);

	void SetPlayer(Player* player) { this-> player = player; }

	void SetDurability(int durability) { this-> durability = durability; }
	int  GetDurability() const { return durability; }

	void SetGlow(bool value) { isGlow = value; }
	bool GetGlow() const { return isGlow; }
};

