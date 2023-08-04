#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "AnimationController.h"

enum class EffectTypes
{
	None = -1,
	SpeedUp,
	Invincibility,	// 무적
	Magnet,			// 자석
};

class SpriteEffect : public SpriteGo
{
protected:
	float duration = 0.f;
	float timer = 0.f;

	ObjectPool<SpriteEffect>* pool = nullptr;
	AnimationController animation;

	float speed = 1300.f;
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
};

