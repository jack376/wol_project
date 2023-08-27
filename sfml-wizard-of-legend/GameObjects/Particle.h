#pragma once
#include "GameObject.h"
#include "ObjectPool.h"

class Particle : public GameObject
{
protected:
	std::string textureId;
	sf::Sprite sprite;

	sf::Vector2f direction;
	sf::Vector2f setAddPosition;
	sf::Vector2f setScale;
	sf::Color setColor = { 255, 255, 255, 255 };

	float setFlowTime = 0.0f;
	float setVelocity = 0.0f;
	float setDuration = 0.75f;
	float setSpeed = 1000.0f;
	float setRotation = 0.0f;


	float flowTime = setFlowTime;
	float velocity = setVelocity;
	float duration = setDuration;
	float speed    = setSpeed;

	float slowdown = 100.0f;
	float gravity  = 15.0f;
	float rotation = setRotation;

	bool isLighting = false;
	bool isAnimation = false;

	float frameTime = 0.1f;
	float currentTime = 0.0f;
	int currentFrame = 0;

	sf::IntRect animationRect = sf::IntRect(currentFrame * 16, 0, 16, 16);

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
	void SetTexture(const std::string& id);
	void SetTextureRect(const sf::IntRect& rect);
	void SetScale(float x, float y);
	void SetColor(sf::Color color);

	void SetPool(ObjectPool<Particle>* pool) { this->pool = pool; }

	void SetDirection(const sf::Vector2f& dir)  { direction = dir; } 
	void SetFlowTime(float newFlowTime)         { setFlowTime = newFlowTime; }
	void SetVelocity(float newVelocity)         { setVelocity = newVelocity; }
	void SetDuration(float newDuration)         { setDuration = newDuration; }
	void SetSpeed(float newSpeed)               { setSpeed = newSpeed; }
	void SetSlowdown(float newSlowdown)         { slowdown = newSlowdown; }
	void SetGravity(float newGravity)           { gravity = newGravity; }
	void SetRotation(float newRotation)         { setRotation = newRotation; }
	void SetAddPosition(sf::Vector2f newAddPos) { setAddPosition = newAddPos; }

	void SetLighting(bool newIsLighting)        { isLighting = newIsLighting; }
	void SetAnimation(bool newIsAnimation)      { isAnimation = newIsAnimation; }

	sf::Vector2f ScaleAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed);
};