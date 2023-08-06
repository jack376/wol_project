#include "stdafx.h"
#include "Monster.h"

Monster::Monster(const std::string& textureId, const std::string& n)
{
}

Monster::~Monster()
{
}

void Monster::Init()
{
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
    SetPosition(look * speed * dt);
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

        if (distance <= range)  //공격범위 ~ 탐색 범위
        {
            SetLook(playerPos);
            if (distance <= attackRange)
                Attack();
            else
                Move(dt);
        }
    }
}
