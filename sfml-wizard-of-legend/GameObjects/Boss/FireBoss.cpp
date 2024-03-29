#include "stdafx.h"
#include "FireBoss.h"
#include "Framework.h"
#include "ResourceMgr.h"
#include "DataTableMgr.h"
#include "MonsterTable.h"
#include "SceneMgr.h"
#include "Player.h"
#include "Particle.h"
#include "SceneGame.h"

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
        skill->AddClip("animations/FireBoss/MeteorStrike.csv");
        skill->pool = ptr;
        skill->SetPlayer(player);
        skill->SetTiles(tilesWorld);
        skill->SetParticlePool(particlePool);
    };
    projectilePool.Init();

    ObjectPool<CustomEffect>* ptr2 = &castingCirclePool;
    castingCirclePool.OnCreate = [ptr2, this](CustomEffect* circle)
    {
        circle->AddClip("animations/FireBoss/CastingCircle.csv");
        circle->pool = ptr2;
        circle->SetType(EffectTypes::Circle);
    };
    castingCirclePool.Init();

    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(1.f);
    rect.setOutlineColor(sf::Color::Green);
}

void FireBoss::Release()
{
    Monster::Release();
    projectilePool.Release();
    castingCirclePool.Release();
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

    for (auto effect : castingCirclePool.GetUseList())
    {
        SCENE_MGR.GetCurrScene()->RemoveGo(effect);
    }
    castingCirclePool.Clear();
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
    //raycaster.draw(window);   

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

void FireBoss::SetActive(bool active)
{
    GameObject::SetActive(active);
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
    case FireBossAttackPattern::Meteor:
        Meteor(dt);
        break;
    case FireBossAttackPattern::Kick:
        Kick(dt);
        break;
    case FireBossAttackPattern::Dash:
        Dash(dt);
        break;

    default:
        //std::cout << "HandleAttackState default" << std::endl;
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
            int maxValue = 2;
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
        else if (patternCount > 2)
        {
            SetState(MonsterState::Idle());
            patternCount = 0;
            stateStart = false;
        }
    }
}


void FireBoss::Die()
{
    if (animation.GetCurrentClipId() != stat.name + "Death")
    {
        SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrScene();
        scene->SetIsGameEnd(true);
        animation.Play(stat.name + "Death");
        SetOrigin(origin);
        SetRectBox();
        std::cout << "FireBoss::Die()" << std::endl;
    }
}

void FireBoss::KnockBack(float dt)
{
    Monster::KnockBack(dt);
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
        if (patternDelayTimer < 0.2f)
            SetParticle(position, 10);

        if (!isAttacked && player->IsAlive())
        {
            if (sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
            {
                attackTimer = 0.f;
                player->SetHp(-stat.damage);
                isAttacked = true;
            }
        }

        patternDelayTimer += dt;
        isDelay = true;
        if (patternDelayTimer > patternDelayRate)
        {
            if (patternCount < 2)
                patternCount++;
            else
            {
                SetState(MonsterState::Idle());
                stateStart = false;
                patternCount = 0;
            }
            patternDelayTimer = 0.f;
            jumpTimer = 0.f;
            patternStart = false;
            isDelay = false;
            isAttacked = false;
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
        fireballPoss = CalculateProjectilePositions(player->GetPosition(), position, 150, 5, 180);
        patternStart = true;
    }

    if (!isDelay)
    {
        if (fireCount != 5)
        {
            for (auto& pos : fireballPoss)
            {
                AnimationProjectile* fireball = projectilePool.Get();
                sf::Vector2f dir = Utils::Normalize(player->GetPosition() - position);
                fireball->SetOrigin(Origins::MC);
                fireball->SetPosition(fireballPoss[fireCount]);
                fireball->SetRotation(look);
                fireball->Play("Fireball");
                fireballs.push_back(fireball);
                SCENE_MGR.GetCurrScene()->AddGo(fireball);
                fireCount++;
            }
        }

        fireballTimer += dt;

        if (fireCount == 5)
        {
            if (fireballTimer > fireballRate)
            {
                fireballs[shotCount]->Fire(Utils::Normalize(player->GetPosition() - fireballPoss[shotCount]), 1000, stat.damage / 2);
                shotCount++;
                fireballTimer = 0;
            }    
        }
    }

    if (fireCount == 5 && shotCount == 5)
    {
        patternDelayTimer += dt;
        isDelay = true;
        if (patternDelayTimer > patternDelayRate)
        {
            if (patternCount < 2)
                patternCount++;
            else
            {
                SetState(MonsterState::Idle());
                stateStart = false;
                patternCount = 0;
            }
            fireCount = 0;
            shotCount = 0;
            fireballTimer = 0.f;
            patternDelayTimer = 0.f;
            patternStart = false; 
            isDelay = false;
            fireballs.clear();
        }
    }
}

void FireBoss::Meteor(float dt)
{
    if (!patternStart)
    {
        animation.Play("FireBossHeel");
        jumpUpPos = position;
        patternStart = true;
    }

    if (position != sf::Vector2f(1024, 980))
    {
        jumpTimer += dt;
        sf::Vector2f supVector = Utils::Lerp(jumpUpPos, {1024, 980}, jumpTimer / meteoJumpDuration);
        float t = std::sin((jumpTimer / meteoJumpDuration) * 3.14159);
        float supY = Utils::Lerp(0, 200, t);
        SetPosition(supVector.x, supVector.y - supY);
    }
    if (jumpTimer > meteoJumpDuration)
    {
        if (!isAttacked && player->IsAlive())
        {
            if (sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
            {
                SetParticle(position, 50);
                attackTimer = 0.f;
                player->SetHp(-stat.damage);
                isAttacked = true;
            }
        }

        if (!isDelay)
        {
            fireballTimer += dt;
            if (animation.IsAnimEndFrame() && fireballTimer > fireballRate && fireCount < 10)
            {
                AnimationProjectile* Meteor = projectilePool.Get();
                CustomEffect* effect = castingCirclePool.Get();

                sf::Vector2f randomPos = getRandomPositionInRadius({ 1024, 980 }, 700.f);

                sf::Vector2f createPos = randomPos - sf::Vector2f(400, 700);
                sf::Vector2f dir = Utils::Normalize(randomPos - createPos);

                Meteor->SetOrigin(Origins::MC);
                Meteor->SetPosition(createPos);
                Meteor->SetRotation(Utils::Angle(dir) - 90);
                Meteor->Play("MeteorStrike");
                Meteor->MeteorFire(createPos, randomPos, stat.damage);
                SCENE_MGR.GetCurrScene()->AddGo(Meteor);

                effect->SetOrigin(Origins::MC);
                effect->Play("CastingCircle", randomPos);
                SCENE_MGR.GetCurrScene()->AddGo(effect);

                fireCount++;
                fireballTimer = 0;
            }
        }
        if (fireCount == 10)
        {
            patternDelayTimer += dt;
            isDelay = true;
            if (patternDelayTimer > patternDelayRate)
            {
                if (patternCount < 2)
                    patternCount++;
                else
                {
                    SetState(MonsterState::Idle());
                    stateStart = false;
                    patternCount = 0; 
                }    
                fireCount = 0;
                fireballTimer = 0.f;
                patternDelayTimer = 0.f;
                jumpTimer = 0.f;
                patternStart = false;
                isDelay = false;
                isAttacked = false;
            }
        }
    }
}

void FireBoss::SetAttackPattern(FireBossAttackPattern pattern)
{
    currentAttackPattern = pattern;
}

void FireBoss::OnAttacked(float damage)
{
    if (currentState != MonsterState::Dead && currentState == MonsterState::Idle)
    {
        SetState(MonsterState::KnockBack);
        hp -= damage;
    }
    else
    {
        hp -= damage;
    }
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

sf::Vector2f FireBoss::getRandomPositionInRadius(const sf::Vector2f& center, float radius)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distAngle(0, 2 * 3.14159); // 0부터 2π까지의 랜덤 각도
    std::uniform_real_distribution<float> distRadius(0, radius);     // 0부터 반경까지의 랜덤 반지름

    float angle = distAngle(gen);
    float randomRadius = distRadius(gen);

    sf::Vector2f randomPosition;
    randomPosition.x = center.x + randomRadius * std::cos(angle);
    randomPosition.y = center.y + randomRadius * std::sin(angle);

    return randomPosition;
}
