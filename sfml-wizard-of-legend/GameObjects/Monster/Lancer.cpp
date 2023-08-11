#include "stdafx.h"
#include "Lancer.h"
#include "ResourceMgr.h"
#include "Player.h"

Lancer::Lancer(MonsterId id, const std::string& textureId, const std::string& n)
	: Monster(id, textureId, n)
{
}

Lancer::~Lancer()
{
}

void Lancer::Init()
{
    Monster::Init();

    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_AttackUp.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_AttackDown.csv"));
    spear.SetName(stat.name + "Spear");
    spear.textureId = "animations/" + stat.name + "_Spear.csv";
    spear.Init();
    SpriteEffect* ptr = &spear;
    spear.PlaySup = [this, ptr]() {
        spear.GetAnimation()->SetFramePos(0, {position.x, position.y - 10.f});
        if (ptr->dir.x > 0)
            spear.GetAnimation()->SetFramePos(1, sf::Vector2f(position.x + 50, position.y - 10.f));
        else
            spear.GetAnimation()->SetFramePos(1, sf::Vector2f(position.x - 50, position.y - 10.f));   
    };
    attackEffect.SetName(stat.name + "AttackEffect");
    attackEffect.textureId = "animations/" + stat.name + "_AttackEffect.csv";
    attackEffect.Init();
    ptr = &attackEffect;
    attackEffect.PlaySup = [this, ptr]() {
        if (ptr->GetAnimation()->IsAnimEndFrame())
            attackEffect.SetActive(false);
    };
}

void Lancer::Release()
{
    Monster::Release();
}

void Lancer::Reset()
{
    Monster::Reset();
    spear.Reset();
    attackEffect.Reset();
}

void Lancer::Update(float dt)
{
    Monster::Update(dt);
    if (currentState != MonsterState::Attacking)
    {
        spear.SetActive(false);
        attackEffect.SetActive(false);
    }

    if (spear.GetActive())
    {
        spear.Update(dt);
        spear.PlaySup();
        if (spear.GetAnimation()->IsAnimEndFrame())
            attackEffect.Play(spear.GetPosition() + spear.dir * 50.f, spear.dir);
    }
        
    if (attackEffect.GetActive())
    {
        attackEffect.Update(dt);
        attackEffect.PlaySup();
    }
}

void Lancer::Draw(sf::RenderWindow& window)
{
    if (attackEffect.GetActive())
        attackEffect.Draw(window);
    if (spear.GetActive())
        spear.Draw(window);
    Monster::Draw(window);
}

void Lancer::Attack(float dt)
{
    SetState(MonsterState::Attacking);
    if (attackTimer >= stat.attackRate)
    {
        if (abs(look.x) >= abs(look.y))
        {
            animation.Play(stat.name + "AttackCharge");
            spear.Play("SpearCharge", position, look);
        }
        else if (look.y > 0)
            animation.Play(stat.name + "AttackDown");
        else
            animation.Play(stat.name + "AttackUp");

        SetOrigin(origin);
        SetRectBox();
        attackTimer = 0.f; 
        isAttacked = false;
    }
    if (!isAttacked && player->IsAlive())
    {
        if (attackEffect.sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
        {
            attackTimer = 0.f;
            player->SetHp(-stat.damage);
            isAttacked = true;
        }
    }
}
