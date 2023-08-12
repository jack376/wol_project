#include "stdafx.h"
#include "SpriteEffect.h"
#include "SceneMgr.h"
#include "Utils.h"
#include "ResourceMgr.h"

SpriteEffect::SpriteEffect(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

void SpriteEffect::Init()
{
	SpriteGo::Init();
}

void SpriteEffect::Reset()
{
	SpriteGo::Reset();
	animation.SetTarget(&sprite);
	sprite.setScale({ 4.0f, 4.0f });
	SetOrigin(Origins::BC);
	SetActive(false);

	//Debug Mode
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color::Yellow);
	rect.setOutlineThickness(1.f);
}

void SpriteEffect::Update(float dt)
{
	if (animation.IsPlaying())
	{
		animation.Update(dt);
		SetOrigin(Origins::BC);
	}

	//Debug Mode
	SetRectBox();
}

void SpriteEffect::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	window.draw(rect);
}

void SpriteEffect::SetAnim(const std::string& path)
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip(path));
	animation.SetTarget(&sprite);
}

void SpriteEffect::SetType(const EffectTypes type)
{
	this->type = type;
}

void SpriteEffect::Play(std::string name, sf::Vector2f pos, sf::Vector2f dir)
{
	this->dir = dir;
	SetPosition(pos);
	sprite.setRotation(Utils::Angle(this->dir) + 90);
	SetActive(true);
	animation.Play(name);
}

void SpriteEffect::SetRotation(sf::Vector2f dir)
{
	sprite.setRotation(Utils::Angle(dir) + 90);
}

void SpriteEffect::AddClip(std::string path)
{
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip(path));
}

void SpriteEffect::SetRectBox()
{
	sf::FloatRect spriteBounds = sprite.getLocalBounds();
	rect.setSize({ spriteBounds.width, spriteBounds.height });
	rect.setScale({ 4, 4 });
	rect.setOrigin(sprite.getOrigin());
	rect.setPosition(sprite.getPosition());
	rect.setRotation(sprite.getRotation());
}
