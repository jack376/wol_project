#include "stdafx.h"
#include "Monster.h"
#include "MonsterTable.h"
#include "DataTableMgr.h"
#include "ResourceMgr.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "Player.h"
#include "SpriteEffect.h"

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
    
    if ((int)monsterId >= 2)
    {
        animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_AttackUp.csv"));
        animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_AttackDown.csv"));
        attackEffect.textureId = "animations/" + stat.name + "_AttackEffect.csv";
        attackEffect.Init();
    }
}

void Monster::Release()
{
}

void Monster::Reset()
{
    SceneGame* scene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrScene());
    player = scene->GetPlayer();

    attackEffect.Reset();

    animation.SetTarget(&sprite);
    sprite.setScale({ 4.f, 4.f });
    animation.Play(stat.name + "Idle");

    SetPosition({ -500, 0 });
    SetOrigin(Origins::MC);
    SetFlipX(false);
    SetRectBox();
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
    if (attackEffect.GetActive())
        attackEffect.Update(dt);

    HandleBehavior(dt);
    HandleState(dt);
 
    //Debug Mode
    searchRange.setPosition(position);
    attackRange.setPosition(position);
    rect.setPosition(position);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Blue);
    rect.setOutlineThickness(1.f);
    //Test
    if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
        OnAttacked(2);
}

void Monster::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);

    if(attackEffect.GetActive())
        attackEffect.Draw(window);

    //Debug Mode
    window.draw(searchRange);
    window.draw(attackRange);
}

void Monster::SetState(MonsterState newState)
{
	currentState = newState;
}

void Monster::HandleState(float dt)
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
        attackTimer += dt;
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
    SetRectBox();
}

void Monster::Attack(float dt)
{
    SetState(MonsterState::Attacking);
    //attackTimer += dt;
    if (attackTimer >= stat.attackRate)
    {
        if ((int)monsterId < 2)
            animation.Play(stat.name + "Attack");
        else
        {
            if (abs(look.x) >= abs(look.y))
                animation.Play(stat.name + "Attack");
            else if (look.y > 0)
                animation.Play(stat.name + "AttackDown");
            else
                animation.Play(stat.name + "AttackUp");
            attackEffect.Play(dt, position, look);
        }
        SetOrigin(origin);
        SetRectBox();
        attackTimer = 0.f;
        isAttacked = false;
    }
    if (!isAttacked && player->IsAlive() &&
        sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
    {
        attackTimer = 0.f;
        player->SetHp(-stat.damage);
        isAttacked = true;
    }

    
}

void Monster::Move(float dt)
{
    SetState(MonsterState::Moving);
    if (animation.GetCurrentClipId() != stat.name + "Run")
    {
        animation.Play(stat.name + "Run");
        SetOrigin(origin);
        SetRectBox();
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
        SetRectBox();
    }
    else if (animation.IsAnimEndFrame())
        SetActive(false);
}

void Monster::KnockBack()
{
    SetState(MonsterState::KnockBack);
    animation.Play(stat.name + "Hurt");
    SetOrigin(origin);
    SetRectBox();
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
            else if (attackTimer >= stat.attackRate)
                Move(dt);
        }
        else
           Idle();            
    }
}

void Monster::SetRectBox()
{
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    rect.setSize({ spriteBounds.width, spriteBounds.height });
    Utils::SetOrigin(rect, Origins::MC);
}
