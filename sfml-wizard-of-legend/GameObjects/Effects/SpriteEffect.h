#include "stdafx.h"
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

	AnimationController animation;

	EffectTypes type = EffectTypes::None;

	sf::RectangleShape bounds;

public:
	sf::Vector2f dir = { 0, 0 };

	SpriteEffect(const std::string& textureId = "", const std::string& n = "");
	virtual ~SpriteEffect() override { };

	void SetDuration(float duration) { this->duration = duration; }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetAnim(const std::string& path);
	void SetType(const EffectTypes type);

	void Play(std::string name, sf::Vector2f pos, sf::Vector2f dir);
	void SetRotation(sf::Vector2f dir);
	void AddClip(std::string path);
	AnimationController* GetAnimation() { return &animation; }
	const std::string& GetCurrentClipId() { return animation.GetCurrentClipId(); }
	void SetRectBox();

	std::function<void()> PlaySup;
};

