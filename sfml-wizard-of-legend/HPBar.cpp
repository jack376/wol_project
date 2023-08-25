#include "stdafx.h"
#include "HPBar.h"
#include "ResourceMgr.h"
#include "Framework.h"

HPBar::HPBar(const std::string& n)
	: GameObject(n)
{
	if (n == "PlayerHP")
	{
		hpBarBGTextureId = "Resources/assets/sprites/ui/HPBarBG.png";
		hpBarFillTextureId = "Resources/assets/sprites/ui/HPBarFill.png";
		hurtBarFillTextureId = "Resources/assets/sprites/ui/HPBarHurtFill.png";
	}
	else if (n == "BossHP")
	{
		hpBarBGTextureId = "Resources/assets/sprites/ui/EnemyHealthBarBG.png";
		hpBarFillTextureId = "Resources/assets/sprites/ui/EnemyHealthBarFill.png";
		hurtBarFillTextureId = "Resources/assets/sprites/ui/EnemyHurtBarFill.png";
	}
}

HPBar::~HPBar()
{
}

void HPBar::SetPosition(const sf::Vector2f& p)
{
	GameObject::SetPosition(p);
	float gap = (hpBarBG.getGlobalBounds().width - hpBarFill.getGlobalBounds().width) * 0.5;
	hpBarBG.setPosition(p);
	hpBarFill.setPosition(p.x + gap, p.y);
	hurtBarFill.setPosition(p.x + gap, p.y);
}

void HPBar::SetPosition(float x, float y)
{
	GameObject::SetPosition(x, y);
	float gap = (hpBarBG.getGlobalBounds().width - hpBarFill.getGlobalBounds().width) * 0.5;
	hpBarBG.setPosition(x, y);
	hpBarFill.setPosition(x + gap, y);
	hurtBarFill.setPosition(x + gap, y);
}

void HPBar::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(hpBarBG, origin);
		Utils::SetOrigin(hpBarFill, origin);
		Utils::SetOrigin(hurtBarFill, origin);
	}
}

void HPBar::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	hpBarBG.setOrigin(x, y);
	hpBarFill.setOrigin(x, y);
	hurtBarFill.setOrigin(x, y);
}

void HPBar::Init()
{
}

void HPBar::Release()
{
}

void HPBar::Reset()
{
	sf::Texture* tex = RESOURCE_MGR.GetTexture(hpBarBGTextureId);
	if (tex != nullptr)
		hpBarBG.setTexture(*tex);
	tex = RESOURCE_MGR.GetTexture(hpBarFillTextureId);
	if (tex != nullptr)
		hpBarFill.setTexture(*tex);
	tex = RESOURCE_MGR.GetTexture(hurtBarFillTextureId);
	if (tex != nullptr)
		hurtBarFill.setTexture(*tex);

	hpBarBG.setScale(4.f, 4.f);
	hpBarFill.setScale(4.f, 4.f);
	hurtBarFill.setScale(4.f, 4.f);
	SetOrigin(origin);
	SetPosition(position);
	hurtHp = *maxHp;
}

void HPBar::Update(float dt)
{
	if (maxHp == nullptr && hp == nullptr)
		return;

	float percent = (float)*hp / (float)*maxHp;

	if (percent >= 0)
	{
		hpBarFill.setScale(4.f * percent, 4.f);
	}
	if (*hp < hurtHp)
	{
		hurtHp -= (hurtHp - *hp) * dt;
		float hurtPercent = (float)hurtHp / (float)*maxHp;
		hurtBarFill.setScale(4.f * hurtPercent, 4.f);
	}	
}

void HPBar::Draw(sf::RenderWindow& window)
{
	window.draw(hpBarBG);
	window.draw(hurtBarFill);
	window.draw(hpBarFill);
}

void HPBar::SetTarget(int* maxHp, int* hp)
{
	this->maxHp = maxHp;
	this->hp = hp;
}
