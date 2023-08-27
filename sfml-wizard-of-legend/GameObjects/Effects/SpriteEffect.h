#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "AnimationController.h"


class SpriteEffect : public SpriteGo
{
protected:
	AnimationController effectAnim;

	float duration = 1.f;
	float timer = 0.f;

	bool isEffect = false;
	std::string animId;

	ObjectPool<SpriteEffect>* pool = nullptr;
public:
	SpriteEffect(const std::string& textureId = "", const std::string& n = "");
	virtual ~SpriteEffect() override { };

	void SetDuration(float duration) { this->duration = duration; }
	void SetPool(ObjectPool<SpriteEffect>* pool) { this->pool = pool; }

	virtual void Init() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;

	void FadeEffectNRemove(float dt);
	void SetAnimId(const std::string& id) { animId = id; }

};

