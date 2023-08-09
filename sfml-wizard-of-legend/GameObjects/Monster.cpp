#include "stdafx.h"
#include "Monster.h"
#include "MonsterTable.h"
#include "DataTableMgr.h"
#include "ResourceMgr.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "Player.h"

#include "InputMgr.h"   //Test

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
}

void Monster::Release()
{
}

void Monster::Reset()
{
    SceneGame* scene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrScene());
    player = scene->GetPlayer();

    animation.SetTarget(&sprite);
    sprite.setScale({ 4.f, 4.f });
    animation.Play(stat.name + "Idle");
    
    SetPosition({ -500, 0 });
    SetOrigin(Origins::BC);
    SetFlipX(false);
    sortLayer = 10;

    hp = stat.maxHp;
    attackTimer = stat.attackRate;

    //Debug Mode
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
 
    //Debug Mode
    searchRange.setPosition(position);
    attackRange.setPosition(position);
    //Test
    if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
    {
        OnAttacked(2);
    }
}

void Monster::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);
    //Debug Mode
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
        //std::cout << "Monster is idle.\n";
        break;

    case MonsterState::Moving:
        //std::cout << "Monster is moving.\n";
        break;

    case MonsterState::Attacking:
        //std::cout << "Monster is attacking.\n";
        break;

    case MonsterState::Dead:
        //std::cout << "Monster is dead.\n";
        break;

    case MonsterState::KnockBack:
        //std::cout << "Monster is KnockBack.\n";
        break;
    }
}

void Monster::Idle()
{
    SetState(MonsterState::Idle);
    animation.Play(stat.name + "Idle");
    SetOrigin(origin);
}

void Monster::Attack(float dt)
{
    SetState(MonsterState::Attacking);
    attackTimer += dt;
    if(attackTimer > stat.attackRate)
    {
        animation.Play(stat.name + "Attack");
        SetOrigin(origin);
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
    if (animation.GetCurrentClipId() != stat.name + "Run")
    {
        animation.Play(stat.name + "Run");
        SetOrigin(origin);
    }
    SetPosition(position + look * stat.speed * dt);
}

void Monster::Die()
{
    SetState(MonsterState::Dead);
    if (animation.GetCurrentClipId() != stat.name + "Death")
    {
        animation.Play(stat.name + "Death");
        SetOrigin(origin);
    }
    else if (animation.IsAnimEndFrame())
        SetActive(false);
}

void Monster::KnockBack()
{
    SetState(MonsterState::KnockBack);
    animation.Play(stat.name + "Hurt");
    SetOrigin(origin);
}

void Monster::SetLook(sf::Vector2f playerPos)
{
    look = direction = Utils::Normalize(playerPos - position);
    if (look.x < 0)
        SetFlipX(true);
    else
        SetFlipX(false);
}

void Monster::OnAttacked(float damage)  //플레이어에서 몬스터를 공격할 때 호출
{
    if (currentState != MonsterState::Dead)
    {
        KnockBack();
        hp -= damage;
    }
}

void Monster::HandleBehavior(float dt)
{
    if (player == nullptr)
        return;
    else
    {
        sf::Vector2f playerPos = player->GetPosition();
        float distance = Utils::Distance(playerPos, position);

        if (hp <= 0)
        {
            Die();
            return;
        }
        if (currentState == MonsterState::KnockBack)
        {
            SetPosition(position + -look * 500.f * dt);  //공격 당한 반대 방향으로 이동 (공격의 주체가 플레이어가 아니라 발사체라면 발사체의 위치를 넘겨 받아 수정)
            knockBackTimer += dt;
            if (knockBackTimer > knockBackTime)
            {
                knockBackTimer = 0;
                Idle();
            }
            return;
        }
        else if (distance <= stat.searchRange)  //공격범위 ~ 탐색 범위
        {
            SetLook(playerPos);
            if (distance <= stat.attackRange)
                Attack(dt);
            else
                Move(dt);
        }
        else
           Idle();            
    }
}
