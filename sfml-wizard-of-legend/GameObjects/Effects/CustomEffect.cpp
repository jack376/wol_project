#include "stdafx.h"
#include "CustomEffect.h"
#include "SceneMgr.h"
#include "Utils.h"
#include "ResourceMgr.h"

CustomEffect::CustomEffect(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

void CustomEffect::Init()
{
	SpriteGo::Init();

	paletteTexture.loadFromFile("shader/LancerColorIndex.png");
}

void CustomEffect::Reset()
{
	SpriteGo::Reset();
	animation.SetTarget(&sprite);
	sprite.setScale({ 4.0f, 4.0f });
	SetOrigin(origin);
	SetActive(false);

	//Debug Mode
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Yellow);
	rect.setOutlineThickness(0.f);
}

void CustomEffect::Update(float dt)
{
	if (animation.IsPlaying())
	{
		SetOrigin(origin);
		//std::cout << sprite.getOrigin().x << "\t" << sprite.getOrigin().y << std::endl;
		animation.Update(dt);
	}

	if (type == EffectTypes::Circle && animation.IsAnimEndFrame())
	{
		SetActive(false);
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
		pool->Return(this);
	}

	//Debug Mode
	SetRectBox();
}

void CustomEffect::Draw(sf::RenderWindow& window)
{
	//SpriteGo::Draw(window);
	window.draw(sprite, &shader);
}

void CustomEffect::SetAnim(const std::string& path)
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip(path));
	animation.SetTarget(&sprite);
}

void CustomEffect::SetType(const EffectTypes type)
{
	this->type = type;
}

void CustomEffect::Play(std::string name, sf::Vector2f pos, sf::Vector2f dir)
{
	this->dir = dir;
	SetPosition(pos);
	sprite.setRotation(Utils::Angle(this->dir) + 90);
	if (type == EffectTypes::Circle)
	{
		sprite.setScale(3, 3);
		sprite.setColor(sf::Color::Red);
		sf::Color spriteColor = sprite.getColor();
		spriteColor.a = 100;
		sprite.setColor(spriteColor);
	}
	SetActive(true);
	animation.Play(name);
}

void CustomEffect::SetRotation(sf::Vector2f dir)
{
	sprite.setRotation(Utils::Angle(dir) + 90);
}

void CustomEffect::AddClip(std::string path)
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip(path));
}

void CustomEffect::SetRectBox()
{
	sf::FloatRect spriteBounds = sprite.getLocalBounds();
	rect.setSize({ spriteBounds.width, spriteBounds.height });
	rect.setScale({ 4, 4 });
	rect.setOrigin(sprite.getOrigin());
	rect.setPosition(sprite.getPosition());
	rect.setRotation(sprite.getRotation());
}

void CustomEffect::SetEffectPool(ObjectPool<CustomEffect>* pool)
{
	this->pool = pool;
}
