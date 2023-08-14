#include "stdafx.h"
#include "Archer.h"

Archer::Archer(MonsterId id, const std::string& textureId, const std::string& n)
	: Monster(id, textureId, n)
{
}

Archer::~Archer()
{
}

void Archer::Init()
{
	Monster::Init();
}

void Archer::Release()
{
	Monster::Release();
}

void Archer::Reset()
{
	Monster::Reset();
}

void Archer::Update(float dt)
{
	Monster::Update(dt);
}

void Archer::Draw(sf::RenderWindow& window)
{
	Monster::Draw(window);
}

void Archer::Attack()
{
}
