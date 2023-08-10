#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "AnimationController.h"

enum class EffectTypes
{
	None = -1,
	Attack,
};

class SpriteEffect : public SpriteGo
{
protected:
	float duration = 0.f;
	float timer = 0.f;

	bool isPlay = false;

	ObjectPool<SpriteEffect>* pool = nullptr;
	AnimationController animation;

	EffectTypes type = EffectTypes::None;

public:
	SpriteEffect(const std::string& textureId = "", const std::string& n = "");
	virtual ~SpriteEffect() override { };

	void SetDuration(float duration) { this->duration = duration; }
	void SetPool(ObjectPool<SpriteEffect>* pool) { this->pool = pool; }

	virtual void Init() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	void SetAnim(const std::string& path);
	void SetType(const EffectTypes type);

	void Play(float dt, sf::Vector2f pos, sf::Vector2f dir);
};

