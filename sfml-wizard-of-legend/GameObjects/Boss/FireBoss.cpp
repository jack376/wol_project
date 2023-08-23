#include "stdafx.h"
#include "FireBoss.h"
#include "ResourceMgr.h"
#include "DataTableMgr.h"
#include "MonsterTable.h"
#include "SceneMgr.h"
#include "Player.h"

FireBoss::FireBoss(MonsterId id, const std::string& textureId, const std::string& n)
	: Monster(id, textureId, n)
{
}

FireBoss::~FireBoss()
{
}

void FireBoss::Init()
{
    stat = DATATABLE_MGR.Get<MonsterTable>(DataTable::Ids::Monster)->Get((int)monsterId);

    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_BackhandDown.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_BackhandRight.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_BackhandUp.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_DashDown.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_DashRight.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_DashUp.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_Dead.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_ForehandDown.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_ForehandRight.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_ForehandUp.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_Heel.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_Hurt.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_Idle.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_Land.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_PointDown.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_PointRight.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_PointUp.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_RoundhouseKick.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_SpinKick.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/FireBoss/FireBoss_Squat.csv"));

    ObjectPool<AnimationProjectile>* ptr = &projectilePool;
    projectilePool.OnCreate = [ptr, this](AnimationProjectile* skill)
    {
        skill->AddClip("");
        skill->AddClip("");
        skill->AddClip("");
        skill->AddClip("");
        skill->textureId = "";
        skill->pool = ptr;
        skill->SetPlayer(player);
        skill->SetTiles(tilesWorld);
    };
    projectilePool.Init();

    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(1.f);
    rect.setOutlineColor(sf::Color::Green);
}

void FireBoss::Release()
{
    Monster::Release();
    projectilePool.Release();
}

void FireBoss::Reset()
{
    Monster::Reset();

    for (auto bullet : projectilePool.GetUseList())
    {
        SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
    }

    projectilePool.Clear();
}

void FireBoss::Update(float dt)
{
    Monster::Update(dt);
}

void FireBoss::Draw(sf::RenderWindow& window)
{
    Monster::Draw(window);
}

void FireBoss::SetPosition(const sf::Vector2f& p)
{
    Monster::SetPosition(p);
}

void FireBoss::SetPosition(float x, float y)
{
    Monster::SetPosition(x, y);
}

void FireBoss::HandleState(float dt)
{
    switch (currentState) {
    case MonsterState::Idle:
        Idle();
        break;

    case MonsterState::Moving:
        Move(dt);
        break;

    case MonsterState::Attacking:
        //std::cout << "Monster is attacking.\n";
        Attack(dt);
        break;

    case MonsterState::Dead:
        Die();
        break;

    case MonsterState::KnockBack:
        KnockBack(dt);
        break;
    case MonsterState::BossStart:
        break;
    }
}

void FireBoss::HandleAttackPattern(float dt)
{
    switch ((FireBossAttackPattern)randomNums[patternCount])
    {
    case FireBossAttackPattern::Dash:
        Dash(dt);
        break;
    case FireBossAttackPattern::Jump:
        Jump(dt);
        break;
    case FireBossAttackPattern::Kick:
        Kick(dt);
        break;
    case FireBossAttackPattern::Fire:
        Fire(dt);
        break;
    default:
        std::cout << "HandleAttackState default" << std::endl;
        break;
    }
}

void FireBoss::Start()
{
}

void FireBoss::Idle()
{
    animation.Play(stat.name + "Squat");
    SetOrigin(origin);
    SetRectBox();

    sf::Vector2f playerPos = player->GetPosition();
    float distance = Utils::Distance(playerPos, position);

    if (hp <= 0 || !player->IsAlive())
    {
        SetState(MonsterState::Dead);
        return;
    }
    if (distance <= stat.attackRange)
    {
        if (attackTimer >= stat.attackRate)
            SetState(MonsterState::Attacking);
        return;
    }
}

void FireBoss::Attack(float dt)
{
    if (patternCount == 0)
    {
        std::random_device rd;
        std::mt19937 gen(rd());

        // 가능한 숫자 범위와 벡터를 초기화
        int minValue = 0;
        int maxValue = 3;
        randomNums.clear();
        for (int i = minValue; i <= maxValue; ++i)
            randomNums.push_back(i);

        // 벡터를 섞음
        std::shuffle(randomNums.begin(), randomNums.end(), gen);
    }
    std::cout << patternCount << "번째 숫자: " << randomNums[patternCount] << std::endl;

    sf::Vector2f playerPos = player->GetPosition();
    SetLook(playerPos);

    HandleAttackPattern(dt);

    if (hp <= 0 || !player->IsAlive())
    {
        SetState(MonsterState::Dead);
        return;
    }
    else if (patternCount > 3)
        SetState(MonsterState::Idle());
}


void FireBoss::Die()
{
}

void FireBoss::KnockBack(float dt)
{
}

void FireBoss::Dash(float dt)
{
}

void FireBoss::Jump(float dt)
{
}

void FireBoss::Kick(float dt)
{
}

void FireBoss::Fire(float dt)
{
    if (animation.GetCurrentClipId() != "FireBossPointRight" && 
        animation.GetCurrentClipId() != "FireBossPointDown" &&
        animation.GetCurrentClipId() != "FireBossPointUp")
    {
        if (abs(look.x) > abs(look.y))
        {
            animation.Play("FireBossPointRight");
        }
        else if (look.y >= 0)
        {
            animation.Play("FireBossPointUp");
        }
        else
        {
            animation.Play("FireBossPointDown");
        }
        SetLook(player->GetPosition());
    }

    if (animation.IsAnimEndFrame())
    {
        AnimationProjectile* fireball = projectilePool.Get();
        fireball->Play(""); 
        //fireball->Fire();
        fireCount++;
    }  



    /*if (patternCount < 2)
        patternCount++;
    else
    {
        SetState(MonsterState::Idle);
        patternCount = 0;
    }*/

}

void FireBoss::SetAttackPattern(FireBossAttackPattern pattern)
{
    currentAttackPattern = pattern;
}

void FireBoss::OnAttacked(float damage)
{
}
