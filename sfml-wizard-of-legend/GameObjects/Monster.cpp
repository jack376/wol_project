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
    sortLayer = 10;


    //SetPlayer();  씬 전환 시에 (마을 <-> 던전) 플레이어 객체를 어떻게 관리할지에 따라 Init() 또는 Reset()에서 해줄지 결정.
}

void Monster::Release()
{
}

void Monster::Reset()
{
    animation.Play("GhoulIdle");
    SetOrigin(origin);
    SetPosition({ 0, 0 });
    SetFlipX(false);
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

void Monster::Attack()
{
    SetState(MonsterState::Attacking);
    if (animation.GetCurrentClipId() != "GhoulAttack")
        animation.Play("GhoulAttack");
}

void Monster::Move(float dt)
{
    SetState(MonsterState::Moving);
    if (animation.GetCurrentClipId() != "GhoulRun")
        animation.Play("GhoulRun");
    SetPosition(position + look * stat.speed * dt);
}

void Monster::Die()
{
    SetState(MonsterState::Dead);
}

void Monster::SetLook(sf::Vector2f playerPos)
{
    look = direction = Utils::Normalize(playerPos - position);
    sprite.setRotation(Utils::Angle(look));
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
                Attack();
            else
                Move(dt);
        }
    }
}
