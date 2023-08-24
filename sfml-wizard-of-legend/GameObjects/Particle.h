#pragma once
#include "GameObject.h"
#include "ObjectPool.h"

class Particle : public GameObject
{
protected:
	std::string textureId;
	sf::Sprite sprite;
	sf::RectangleShape shape;
	sf::Vector2f direction;

	float flowTime = 0.0f;
	float velocity = 0.0f;
	float duration = 0.75f;
	float speed    = 750.0f;

	float slowdown = 50.0f;
	float gravity  = 15.0f;
	float rotation = 1.0f;

	bool isLighting = false;

	ObjectPool<Particle>* pool = nullptr;

public:
	Particle(const std::string& textureId = "", const std::string& n = "", const float& flowTime = 0.0f)
		: GameObject(n), textureId(textureId) {}
	virtual ~Particle() override {}

	virtual void Init() override {}
	virtual void Release() override {}

	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetPosition(const sf::Vector2f& p);
	void SetPosition(float x, float y);
	void SetOrigin(Origins origin);
	void SetOrigin(float x, float y);
	void SetRotation(float angle);
	void SetTexture(const std::string& id);
	void SetTextureRect(const sf::IntRect& rect);

	void SetDuration(float duration) { this->duration = duration; }
	void SetPool(ObjectPool<Particle>* pool) { this->pool = pool; }
};