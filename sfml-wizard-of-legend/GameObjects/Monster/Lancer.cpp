#include "stdafx.h"
#include "Lancer.h"
#include "ResourceMgr.h"
#include "Player.h"
//#include "AnimationController.h"

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
    spear.AddClip("animations/Lancer_Spear.csv");
    SpriteEffect* ptr = &spear;
    spear.PlaySup = [this, ptr]() {
        if (animation.GetCurrentClipId() == "LancerAttack")
        {
            spear.SetOrigin(Origins::BC);
            if (ptr->dir.x > 0)
                spear.GetAnimation()->SetFramePos(1, sf::Vector2f(position.x + 50, position.y - 10.f));
            else
                spear.GetAnimation()->SetFramePos(1, sf::Vector2f(position.x - 50, position.y - 10.f));
        }
        else
        {
            spear.SetOrigin(Origins::BC);
            if (ptr->dir.y > 0)
                spear.GetAnimation()->SetFramePos(1, { position.x + 30, position.y - 5 });
            else
                spear.GetAnimation()->SetFramePos(1, { position.x + 30, position.y - 15 });
        }
        spear.SetPosition(spear.sprite.getPosition());
    };
    attackEffect.AddClip("animations/Lancer_AttackEffect.csv");
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
        spear.SetActive(false);
    else
    {
        spear.Update(dt);
        spear.PlaySup();
    }

    if (!isEffect && spear.GetAnimation()->IsPlaying())
    {
        chargeTimer += dt;
        if (chargeTimer >= chargeRate)
        {
            attackEffect.Play("LancerAttackEffect", spear.sprite.getPosition(), spear.dir);
            isEffect = true;
            chargeTimer = 0.f;
        }      
    }
   
    attackEffect.Update(dt);
    attackEffect.PlaySup();
}

void Lancer::Draw(sf::RenderWindow& window)
{
    if (spear.GetActive())
        spear.Draw(window);
    Monster::Draw(window);
    if (attackEffect.GetActive())
        attackEffect.Draw(window);
}

void Lancer::Attack(float dt)
{
    SetState(MonsterState::Attacking);
    if (attackTimer >= stat.attackRate)
    {
        if (abs(look.x) >= abs(look.y))
        {
            animation.Play(stat.name + "Attack");
            spear.Play("LancerSpear", position, look);
        }
        else
        {
            spear.SetOrigin(Origins::MC);
            if (look.y > 0)
            {
                animation.Play(stat.name + "AttackDown");
                spear.Play("LancerSpear", { position.x + 20, position.y - 70 }, look);
            }
            else
            {
                animation.Play(stat.name + "AttackUp");
                spear.Play("LancerSpear", { position.x + 20, position.y + 70 }, look);
            }
            SetFlipX(false);
        }
        SetOrigin(origin);
        SetRectBox();
        spear.SetActive(true);
        attackEffect.SetActive(true);
        attackTimer = 0.f; 
        isAttacked = false;
        isShooting = true;
        isEffect = false;
    }
    if (!isAttacked && player->IsAlive())
    {
        //OBB적용 필요
        if (attackEffect.sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
        {
            player->SetHp(-stat.damage);
            isAttacked = true;
        }
    }
}
