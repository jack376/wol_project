#include "stdafx.h"
#include "Monster.h"
#include "MonsterTable.h"
#include "DataTableMgr.h"
#include "ResourceMgr.h"
#include "SceneGame.h"
#include "SceneMgr.h"   //test
#include "Player.h"


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
    sprite.setScale({ 4.5f, 4.5f });
    sortLayer = 10;


}

void Monster::Release()
{
}

void Monster::Reset()
{
    SceneGame* scene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrScene());
    player = scene->GetPlayer();

    animation.Play(stat.name + "Idle");
    SetOrigin(origin);
    SetPosition({ 0, 0 });
    SetFlipX(false);
    hp = stat.maxHp;
    attackTimer = stat.attackRate;


    //디버그 모드
    {
        searchRange.setRadius(stat.searchRange);
        attackRange.setRadius(stat.attackRange);
        Utils::SetOrigin(searchRange, Origins::MC);
        Utils::SetOrigin(attackRange, Origins::MC);
        searchRange.setOutlineThickness(1.f);
        attackRange.setOutlineThickness(1.f);
        searchRange.setOutlineColor(sf::Color::Green);
        attackRange.setOutlineColor(sf::Color::Red);
        searchRange.setFillColor(sf::Color::Transparent);
        attackRange.setFillColor(sf::Color::Transparent);
    } 
}

void Monster::Update(float dt)
{
    animation.Update(dt);

    HandleBehavior(dt);
    HandleState();

    searchRange.setPosition(position);
    attackRange.setPosition(position);
}

void Monster::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);
    window.draw(searchRange);
    window.draw(attackRange);
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

    case MonsterState::KnockBack:
        std::cout << "Monster is KnockBack.\n";
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
        if (player->IsAlive() && sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
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

void Monster::OnAttacked(float damage)
{
    hp -= damage;
    animation.Play(stat.name + "Hurt");
}

void Monster::HandleBehavior(float dt)
{
    if (player == nullptr)
        return;
    else
    {
        sf::Vector2f playerPos = player->GetPosition();
        float distance = Utils::Distance(playerPos, position);

        if (distance <= stat.searchRange)  //공격범위 ~ 탐색 범위
        {
            SetLook(playerPos);
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
