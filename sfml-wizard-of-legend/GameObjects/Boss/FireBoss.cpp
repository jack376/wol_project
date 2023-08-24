#include "stdafx.h"
#include "FireBoss.h"
#include "ResourceMgr.h"
#include "DataTableMgr.h"
#include "MonsterTable.h"
#include "SceneMgr.h"
#include "Player.h"

#define _CenterPos sf::Vector2f(1024, 1024);

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
    Scene* scene = SCENE_MGR.GetCurrScene();

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

    attackEffect.AddClip("animations/FireBoss/FireWings.csv");
    attackEffect.SetOrigin(Origins::BC);
    
    ObjectPool<AnimationProjectile>* ptr = &projectilePool;
    projectilePool.OnCreate = [ptr, this](AnimationProjectile* skill)
    {
        skill->AddClip("animations/FireBoss/Fireball.csv");
        //skill->textureId = "";
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
    attackEffect.Reset();

    for (auto bullet : projectilePool.GetUseList())
    {
        SCENE_MGR.GetCurrScene()->RemoveGo(bullet);
    }
    projectilePool.Clear();
}

void FireBoss::Update(float dt)
{
    Monster::Update(dt);

    SetLook(player->GetPosition());
    raycaster.Rotation(Utils::Angle(look)); //방향 확인만

    if (attackEffect.GetActive())
        attackEffect.Update(dt);      
}

void FireBoss::Draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    raycaster.draw(window);   

    if (attackEffect.GetActive())
        window.draw(attackEffect.sprite);
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

    case FireBossAttackPattern::Jump:
        Jump(dt);
        break;
    case FireBossAttackPattern::Fire:
        Fire(dt);
        break;
    case FireBossAttackPattern::Kick:
        Kick(dt);
        break;
    case FireBossAttackPattern::Dash:
        Dash(dt);
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
    if (!stateStart)
    {
        animation.Play("FireBossIdle");
        animation.PlayQueue("FireBossSquat");
        stateStart = true;
    }

    SetOrigin(origin);
    SetRectBox();

    sf::Vector2f playerPos = player->GetPosition();
    float distance = Utils::Distance(playerPos, position);

    if (hp <= 0 || !player->IsAlive())
    {
        SetState(MonsterState::Dead);
        stateStart = false;
        return;
    }
    else if (animation.GetCurrentClipId() == "FireBossSquat" && animation.IsAnimEndFrame())
    {
        SetState(MonsterState::Attacking);
        stateStart = false;
        return;
    }
}

void FireBoss::Attack(float dt)
{
    if (!stateStart)
    {
        animation.Play("FireBossIdle");
        if (patternCount == 0)
            attackEffect.Play("FireWings", position, sf::Vector2f(0, -1));
        stateStart = true;
    }
        
    if (attackEffect.GetAnimation()->IsAnimEndFrame())
    {
        attackEffect.SetActive(false);
        if (patternCount == 0)
        {
            std::random_device rd;
            std::mt19937 gen(rd());

            // 가능한 숫자 범위와 벡터를 초기화
            int minValue = 0;
            int maxValue = 1;
            randomNums.clear();
            for (int i = minValue; i <= maxValue; ++i)
                randomNums.push_back(i);

            // 벡터를 섞음
            //std::shuffle(randomNums.begin(), randomNums.end(), gen);
        }
        //std::cout << patternCount << "번째 숫자: " << randomNums[patternCount] << std::endl;

        sf::Vector2f playerPos = player->GetPosition();
        SetLook(playerPos);

        HandleAttackPattern(dt);

        if (hp <= 0 || !player->IsAlive())
        {
            SetState(MonsterState::Dead);
            stateStart = false;
            return;
        }
        else if (patternCount > 1)
        {
            SetState(MonsterState::Idle());
            patternCount == 0;
            stateStart = false;
        }
    }
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
    if (!patternStart)
    {
        animation.Play("FireBossHeel");
        jumpUpPos = position;
        jumpDownPos = player->GetPosition();
        patternStart = true;
    }

    if (!isDelay)
    {
        jumpTimer += dt;
        sf::Vector2f supVector = Utils::Lerp(jumpUpPos, jumpDownPos, jumpTimer / jumpDuration);
        float t = std::sin((jumpTimer / jumpDuration) * 3.14159);
        float supY = Utils::Lerp(0, 200, t);

        SetPosition(supVector.x, supVector.y - supY);
    }

    if (jumpTimer > jumpDuration)
    {
        patternDelayTimer += dt;
        isDelay = true;
        if (patternDelayTimer > patternDelayRate)
        {
            if (patternCount < 1)
                patternCount++;
            else
                patternCount = 0;

            patternDelayTimer = 0.f;
            jumpTimer = 0.f;
            patternStart = false;
            isDelay = false;
        }  
    }
        
}

void FireBoss::Kick(float dt)
{
}

void FireBoss::Fire(float dt)
{
    if (!patternStart)
    {
        if (abs(look.x) > abs(look.y))
            animation.Play("FireBossPointRight");
        else if (look.y <= 0)
            animation.Play("FireBossPointUp");
        else
            animation.Play("FireBossPointDown");
        SetLook(player->GetPosition());
        fireballPoss = CalculateProjectilePositions(player->GetPosition(), position, 150, 10, 180);
        patternStart = true;
    }

    if (!isDelay)
    {
        fireballTimer += dt;
        if (animation.IsAnimEndFrame() && fireballTimer > fireballRate && fireCount < 10)
        {
            AnimationProjectile* fireball = projectilePool.Get();
            sf::Vector2f dir = Utils::Normalize(player->GetPosition() - position);
            fireball->SetPosition(position);
            fireball->SetRotation(Utils::Angle(dir));
            fireball->Play("Fireball");
            fireball->Fire(dir, 1000, stat.damage / 2);
            SCENE_MGR.GetCurrScene()->AddGo(fireball);
            fireCount++;
            fireballTimer = 0;
        }
    }
    
    else if (fireCount == 10)
    {
        patternDelayTimer += dt;
        isDelay = true;
        if (patternDelayTimer > patternDelayRate)
        {
            if (patternCount < 1)
                patternCount++;
            else
                patternCount = 0;
            fireCount = 0;
            fireballTimer = 0.f;
            patternDelayTimer = 0.f;
            patternStart = false; 
            isDelay = false;
        }
    }
}

void FireBoss::SetAttackPattern(FireBossAttackPattern pattern)
{
    currentAttackPattern = pattern;
}

void FireBoss::OnAttacked(float damage)
{
}

std::vector<sf::Vector2f> FireBoss::CalculateProjectilePositions(const sf::Vector2f& playerPosition, const sf::Vector2f& monsterPosition, float radius, int count, float angleRange) {
    std::vector<sf::Vector2f> positions;

    sf::Vector2f direction = playerPosition - monsterPosition;
    direction /= std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // 중심 투사체 위치 계산 (플레이어 방향)
    float centerAngle = Utils::Angle(direction);
    // 투사체 위치 계산
    float angleIncrement = angleRange / count;
    float startAngle = centerAngle - angleRange / 2.0f;

    for (int i = 0; i < count; ++i) {
        float adjustedAngle = startAngle + i * angleIncrement;
        sf::Vector2f offset = radius * Utils::Direction(adjustedAngle);
        sf::Vector2f projectilePosition = monsterPosition + offset;
        positions.emplace_back(projectilePosition);
    }
    return positions;
}