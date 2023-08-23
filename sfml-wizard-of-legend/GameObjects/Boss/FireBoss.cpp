#include "stdafx.h"
#include "FireBoss.h"
#include "ResourceMgr.h"
#include "DataTableMgr.h"
#include "MonsterTable.h"
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

    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(1.f);
    rect.setOutlineColor(sf::Color::Green);
}

void FireBoss::Release()
{
    Monster::Release();
}

void FireBoss::Reset()
{
    Monster::Reset();
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
    switch (currentAttackState)
    {
    case FireBossAttackPattern::Dash:
        break;
    case FireBossAttackPattern::Jump:
        break;
    case FireBossAttackPattern::Kick:
        break;
    case FireBossAttackPattern::Shoot:
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
        std::vector<int> nums;
        for (int i = minValue; i < maxValue; ++i)
            nums.push_back(i);

        // 벡터를 섞음
        std::shuffle(nums.begin(), nums.end(), gen);

        // 중복되지 않는 숫자 3개를 뽑음
        for (int i = 0; i < 3; ++i) {
            int randomNumber = nums[i];
        }
    }
    

    sf::Vector2f playerPos = player->GetPosition();
    float distance = Utils::Distance(playerPos, position);

    SetLook(playerPos);

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
    if (animation.GetCurrentClipId() != "FireBossDash")
    {
        animation.Play("MageAttackAim");
        SetOrigin(origin);
        SetRectBox();

        sf::Vector2f playerPos = player->GetPosition();
        attackDir = SetLook(playerPos);

        std::vector<sf::Vector2f> fireballPoss = CalculateProjectilePositions(playerPos, position, 150, 5, 120);

        for (int i = 0; i < 5; i++)
        {
            fireballs[i].Reset();
            fireballs[i].SetActive(true);
            fireballs[i].SetPosition(fireballPoss[i]);
            fireballs[i].SetRotation(Utils::Angle(fireballPoss[i] - position) - 90);
            fireballs[i].Play("MageFireballCreate");
            fireballs[i].PlayQueue("MageFireballActive");
        }
    }
}

void FireBoss::Jump(float dt)
{
}

void FireBoss::Kick(float dt)
{
}

void FireBoss::Fire(float dt)
{
}

void FireBoss::OnAttacked(float damage)
{
}
