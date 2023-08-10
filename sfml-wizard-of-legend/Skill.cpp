#include "stdafx.h"
#include "Skill.h"

Skill::Skill(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

Skill::~Skill()
{
}

void Skill::Init()
{
}

void Skill::Release()
{
}

void Skill::Reset()
{
	SpriteGo::Reset();
}

void Skill::Update(float dt)
{
	SpriteGo::Update(dt);
}

void Skill::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

}
