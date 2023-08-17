#include "stdafx.h"
#include "Lancer.h"
#include "ResourceMgr.h"
#include "Player.h"
#include "DataTableMgr.h"
#include "MonsterTable.h"

#define _AttackUpAimLocalPos sf::Vector2f(76, 92)
#define _AttackDownAimLocalPos sf::Vector2f(72, 36)

Lancer::Lancer(MonsterId id, const std::string& textureId, const std::string& n)
	: Monster(id, textureId, n)
{
}

Lancer::~Lancer()
{
}

void Lancer::Init()
{
    stat = DATATABLE_MGR.Get<MonsterTable>(DataTable::Ids::Monster)->Get((int)monsterId);

    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_Run.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_AttackAim.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_AttackShoot.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_Death.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_Idle.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_Hurt.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_AttackUpAim.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_AttackUpShoot.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_AttackDownAim.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_AttackDownShoot.csv"));
    
    spearAni.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Lancer_SpearAim.csv"));
    spearAni.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Lancer_SpearAttack.csv"));
    spearAni.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Lancer_SpearUpDownAim.csv"));
    spearAni.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Lancer_SpearUpDownAttack.csv"));

    attackEffect.AddClip("animations/Lancer_AttackEffect.csv");
    CustomEffect* ptr = &attackEffect;
    attackEffect.PlaySup = [this, ptr]() {
        if (ptr->GetAnimation()->IsAnimEndFrame())
            attackEffect.SetActive(false);
    };

    collider.Init();

    //rect.setSize({ 100, 150 });
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(1.f);
    rect.setOutlineColor(sf::Color::Green);
}

void Lancer::Release()
{
    Monster::Release();
    collider.Release();
}

void Lancer::Reset()
{
    Monster::Reset();
    collider.Reset();

    spearAni.SetTarget(&spear);
    spear.setScale({ 4.f, 4.f });
    Utils::SetOrigin(spear, Origins::BC);
}

void Lancer::Update(float dt)
{
    Monster::Update(dt);     
    attackEffect.Update(dt);
    attackEffect.PlaySup();

    collider.SetSprite(spear);
    collider.SetColSize();
    //collider.SetPosition(spear.getPosition());
}

void Lancer::Draw(sf::RenderWindow& window)
{
    if (currentAttackState == AttackState::Aim ||
        currentAttackState == AttackState::Shoot)
        window.draw(spear);
    Monster::Draw(window);
    if (attackEffect.GetActive())
        attackEffect.Draw(window);

    collider.Draw(window);
        
}

void Lancer::HandleAttackState(float dt)
{
    switch (currentAttackState)
    {
    case AttackState::Aim:
        Aim(dt);
        break;
    case AttackState::Shoot:
        Shoot(dt);
        break;
    default:
        std::cout << "HandleAttackState default" << std::endl;
        break;
    }
}

void Lancer::Attack(float dt)
{
    spearAni.Update(dt);


    if (attackTimer >= stat.attackRate)
    {
        currentAttackState = AttackState::Aim;
        attackTimer = 0.f;
        
        SetLook(player->GetPosition());
        attackDir = look;
    }
    HandleAttackState(dt);
}

void Lancer::Aim(float dt)
{
    if (abs(attackDir.x) >= abs(attackDir.y))
    {
        if (animation.GetCurrentClipId() != "LancerAttackAim")
        {
            animation.Play(stat.name + "AttackAim");
            //sf::Vector2f playerPos = player->GetPosition();
            //SetLook(playerPos);
        }
            
        if (attackDir.x > 0)
            spear.setPosition({ position.x, position.y });
        else
            spear.setPosition({ position.x, position.y });
    }
    else
    {
        //look = direction = Utils::Normalize(player->GetPosition() - position);
        sf::Vector2f pos = { sprite.getGlobalBounds().left, sprite.getGlobalBounds().top };
        if (attackDir.y > 0)
        {
            if (animation.GetCurrentClipId() != "LancerAttackDownAim")
                animation.Play(stat.name + "AttackDownAim");
            Utils::SetOrigin(spear, Origins::MC);
            spear.setPosition(pos + _AttackDownAimLocalPos);
        }
        else
        {
            if (animation.GetCurrentClipId() != "LancerAttackUpAim")
                animation.Play(stat.name + "AttackUpAim");
            Utils::SetOrigin(spear, Origins::MC);
            spear.setPosition(pos + _AttackUpAimLocalPos);
        }  
        SetFlipX(false);
    }
    spearAni.Play("LancerSpearAim");

    SetOrigin(origin);
    SetRectBox();

    spear.setRotation(Utils::Angle(look) + 90);

    aimTimer += dt;
    if (aimTimer > aimRate)
    {
        currentAttackState = AttackState::Shoot;
        aimTimer = 0;
        collider.SetActive(true);
        collider.SetOrigin(Origins::BC);
    }

}

void Lancer::Shoot(float dt)
{

    if (abs(attackDir.x) >= abs(attackDir.y))
    {
        if (animation.GetCurrentClipId() != "LancerAttackShoot")
        {
            animation.Play(stat.name + "AttackShoot");
            sf::Vector2f playerPos = player->GetPosition();
            SetLook(playerPos);
        }
            
        if (attackDir.x > 0)
            spear.setPosition({ position.x + 50, position.y - 10.f });
        else
            spear.setPosition({ position.x - 50, position.y - 10.f });
    }
    else
    {
        //look = direction = Utils::Normalize(player->GetPosition() - position);
        sf::Vector2f pos = { sprite.getGlobalBounds().left, sprite.getGlobalBounds().top };
        Utils::SetOrigin(spear, Origins::BC);
        if (attackDir.y > 0)
        {
            if (animation.GetCurrentClipId() != "LancerAttackDownShoot")
                animation.Play(stat.name + "AttackDownShoot");
            spear.setPosition({ pos.x + 88, pos.y + 68 });
        }
        else
        {
            if (animation.GetCurrentClipId() != "LancerAttackUpShoot")
                animation.Play(stat.name + "AttackUpShoot");
            spear.setPosition({ position.x + 30, position.y - 15 });
        }
        SetFlipX(false);
    }
    collider.SetPosition(spear.getPosition());
    collider.GetObbCol().setRotation(Utils::Angle(attackDir) + 90);

    if (spearAni.GetCurrentClipId() != "LancerSpearAttack")
    {
        spearAni.Play("LancerSpearAttack");
        Utils::SetOrigin(spear, Origins::BC);
        attackEffect.SetActive(true);
        attackEffect.Play("LancerAttackEffect", spear.getPosition() + attackDir
            * 30.f, attackDir);
        isAttacked = false;
    }

    SetOrigin(origin);
    SetRectBox();

    if (!isAttacked && player->IsAlive())
    {
        if (collider.ObbCol(player->rect))
        {
            player->SetHitDir(direction);
            player->SetHp(-stat.damage);

            collider.SetActive(false);
            isAttacked = true;
        }
    }

    shootTimer += dt;

    if (shootTimer > shootRate)
    {
        shootTimer = 0;
        currentAttackState = AttackState::Cool;
        SetState(MonsterState::Idle);
    }
}
