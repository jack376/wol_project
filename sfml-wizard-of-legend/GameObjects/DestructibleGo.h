#pragma once
#include "GameObject.h"
#include "ObjectPool.h"

class Particle;
class DestructibleGo : public GameObject
{
protected:
	std::string textureId;
	sf::Sprite sprite;
	sf::RectangleShape shape;

	int durability = 1;
	ObjectPool<Particle>* particlePool;

public:
	DestructibleGo(const std::string& textureId = "", const std::string& n = "")
		: GameObject(n), textureId(textureId) {}
	virtual ~DestructibleGo() override {}

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

	void OnHitDestructibleGo(int hit);
	void SetParticle(sf::Vector2f position);
	void SetParticlePool(ObjectPool<Particle>* pool);
};

