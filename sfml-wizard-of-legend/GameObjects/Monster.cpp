#include "stdafx.h"
#include "Monster.h"
#include "MonsterTable.h"
#include "DataTableMgr.h"
#include "ResourceMgr.h"
#include "InputMgr.h"   //test
#include "SceneMgr.h"   //test


Monster::Monster(MonsterId id, const std::string& textureId, const std::string& n)
    : monsterId(id)
{
}

Monster::~Monster()
{
}

void Monster::Init()
{
    stat = DATATABLE_MGR.Get<MonsterTable>(DataTable::Ids::Monster)->Get((int)monsterId);
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_Run.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_Attack.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_Death.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_Idle.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_Hurt.csv"));
    animation.SetTarget(&sprite);
    sprite.setScale({ 2, 2 });
    sortLayer = 10;
    
    //SetPlayer();  씬 전환 시에 (마을 <-> 던전) 플레이어 객체를 어떻게 관리할지에 따라 Init() 또는 Reset()에서 해줄지 결정.
}

void Monster::Release()
{
}

void Monster::Reset()
{
    animation.Play(stat.name + "Idle");
    SetOrigin(origin);
    SetPosition({ 0, 0 });
    SetFlipX(false);

    hp = stat.maxHp;
    attackTimer = stat.attackRate;
}

void Monster::Update(float dt)
{
    animation.Update(dt);

    HandleBehavior(dt);
    HandleState();
}

void Monster::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);
}

void Monster::SetState(MonsterState newState)
{
	currentState = newState;
}

void Monster::HandleState()
{
    switch (currentState) {
    case MonsterState::Idle:
        std::cout << "Monster is idle.\n";
        break;

    case MonsterState::Moving:
        std::cout << "Monster is moving.\n";
        break;

    case MonsterState::Attacking:
        std::cout << "Monster is attacking.\n";
        break;

    case MonsterState::Dead:
        std::cout << "Monster is dead.\n";
        break;
    }
}

void Monster::Attack(float dt)
{
    SetState(MonsterState::Attacking);
    attackTimer += dt;
    if(attackTimer > stat.attackRate)
    {
        animation.Play(stat.name + "Attack");
        attackTimer = 0.f;
        if (player->isAlive && sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
        {
            attackTimer = 0.f;
            player->SetHp(-stat.damage);
        }
    }
}

void Monster::Move(float dt)
{
    SetState(MonsterState::Moving);
    if (animation.GetCurrentClipId() != "GhoulRun")
        animation.Play(stat.name + "Run");
    SetPosition(position + look * stat.speed * dt);
}

void Monster::Die()
{
    SetState(MonsterState::Dead);
    if (animation.GetCurrentClipId() != stat.name + "Death")
        animation.Play(stat.name + "Death");
    //else 현재 재생중인 애니메이션이 끝나면 (마지막 프레임이면) SetActive(false)
    
}

void Monster::SetLook(sf::Vector2f playerPos)
{
    look = direction = Utils::Normalize(playerPos - position);
    if (look.x < 0)
        SetFlipX(true);
    else
        SetFlipX(false);
}

void Monster::HandleBehavior(float dt)
{
    if (player != nullptr)
        return;
    else
    {
        //sf::Vector2f playerPos = player->GetPosition();
        //float distance = Utils::Distance(playerPos, position);

        sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
        sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
        float distance = Utils::Distance(worldMousePos, position);

        if (distance <= 1000.f)  //공격범위 ~ 탐색 범위
        {
            SetLook(worldMousePos);
            if (distance <= stat.attackRange)
                Attack(dt);
            else
                Move(dt);
        }
        else
            animation.Play(stat.name + "Idle");

        if (hp <= 0)
            Die();
    }
}
