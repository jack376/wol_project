#pragma once
#include "GameObject.h"
#include "ObjectPool.h"

class Player;
class Particle;
class GlowGo : public GameObject
{
protected:
	std::string textureId;
	sf::Sprite sprite;

	float timer = 0.0f;
	float cooldown = 0.1f;

	bool isFireLight = false;
	bool isPortal = false;


	Player* player = nullptr;

	ObjectPool<Particle>* particlePool = nullptr;

public:
	GlowGo(const std::string& textureId = "", const std::string& n = "")
		: GameObject(n), textureId(textureId) {}
	virtual ~GlowGo() override {}

	virtual void Init() override {}
	virtual void Release() override {}

	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetPosition(const sf::Vector2f& p);
	void SetPosition(float x, float y);
	void SetOrigin(Origins origin);
	void SetOrigin(float x, float y);
	void SetTexture(const std::string& id);
	void SetScale(float x, float y);
	void SetColor(sf::Color color);

	void SetPlayer(Player* player) { this->player = player; }

	void SetParticle(sf::Vector2f position, int count);
	void SetParticlePool(ObjectPool<Particle>* pool);

	void OnFireParticle()   { isFireLight = true; }
	void OnPortalParticle() { isPortal = true; }

	void FireParticleAnimation();
	void PortalParticleAnimation();
};