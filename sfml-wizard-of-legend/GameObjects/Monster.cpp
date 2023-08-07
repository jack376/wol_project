#include "stdafx.h"
#include "Monster.h"
#include "MonsterTable.h"
#include "DataTableMgr.h"
#include "ResourceMgr.h"

Monster::Monster(MonsterId id, const std::string& textureId, const std::string& n)
    : monsterId(id)
{
}

Monster::~Monster()
{
}

void Monster::Init()
{
    MonsterInfo info = DATATABLE_MGR.Get<MonsterTable>(DataTable::Ids::Monster)->Get((int)monsterId);
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animation/" + info.name + "_Run.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animation/" + info.name + "_Attact.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animation/" + info.name + "_Death.csv"));
    animation.SetTarget(&sprite);
    textureId = "sprites/"
    sortLayer = 10;
    
    //SetPlayer();  씬 전환 시에 (마을 <-> 던전) 플레이어 객체를 어떻게 관리할지에 따라 Init() 또는 Reset()에서 해줄지 결정.

}

void Monster::Release()
{
}

void Monster::Reset()
{
    SpriteGo::Reset();
}

void Monster::Update(float dt)
{
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
}

void Monster::Move(float dt)
{
    SetState(MonsterState::Moving);
    SetPosition(look * stat.speed * dt);
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
    if (player == nullptr)
        return;
    else
    {
        sf::Vector2f playerPos = player->GetPosition();
        float distance = Utils::Distance(playerPos, position);

        if (distance <= searchRange)  //공격범위 ~ 탐색 범위
        {
            SetLook(playerPos);
            if (distance <= stat.attackRange)
                Attack();
            else
                Move(dt);
        }
    }
}
