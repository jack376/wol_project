#include "stdafx.h"
#include "Monster.h"
#include "MonsterTable.h"
#include "DataTableMgr.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "Player.h"
#include "Tile.h"
#include "CustomEffect.h"
#include "AS.h"
#include "Particle.h"
#include "SpriteEffect.h"

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

    //rect.setSize({ 100, 150 });
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(1.f);
    rect.setOutlineColor(sf::Color::Green);

    playerHitEffectPool.OnCreate = [this](SpriteEffect* effect) {
        effect->sprite.setScale(3, 3);
        effect->SetAnimId("HitEffect");
        effect->SetDuration(0.5f);
        effect->sortLayer = 20;
    };
    playerHitEffectPool.Init();

}

void Monster::Release()
{
    for (auto obj : playerHitEffectPool.GetUseList())
    {
        SCENE_MGR.GetCurrScene()->RemoveGo(obj);
    }
    playerHitEffectPool.Clear();
}

void Monster::Reset()
{
    SpriteGo::Reset();
    playerHitEffectPool.Clear();
    attackEffect.Reset();

    SceneGame* scene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrScene());
    player = scene->GetPlayer();

    SetState(MonsterState::Idle);

    sprite.setScale({ 4.f, 4.f });
    animation.SetTarget(&sprite);
    animation.Play(stat.name + "Idle");

    SetPosition(position);
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
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Blue);
        rect.setOutlineThickness(1.f);
    }  

    paletteTexture.loadFromFile("shader/" + stat.name + "ColorIndex.png");
    shader.loadFromFile("shader/FragShader.frag", sf::Shader::Fragment);
    shader.setUniform("paletteTexture", paletteTexture);
    shader.setUniform("paletteYOffset", yOffset);
}

void Monster::Update(float dt)
{
    animation.Update(dt);
    attackTimer += dt;

    CalculatorCurrentTile();
    HandleState(dt);

    //Debug Mode
    searchRange.setPosition(position);
    attackRange.setPosition(position);
    rect.setPosition(position);
}

void Monster::Draw(sf::RenderWindow& window)
{
    //SpriteGo::Draw(window);
    if (monsterId == MonsterId::Ghoul || monsterId == MonsterId::GhoulLarge)
        window.draw(sprite);
    else
        window.draw(sprite, &shader);

    //Debug Mode
    //window.draw(searchRange);
    //window.draw(attackRange);
    //raycaster.draw(window);
}

void Monster::SetPosition(const sf::Vector2f& p)
{
    SpriteGo::SetPosition(p);
    raycaster.move(p.x, p.y);
}

void Monster::SetPosition(float x, float y)
{
    SpriteGo::SetPosition(x, y);
    raycaster.move(x, y);
}

void Monster::SetState(MonsterState newState)
{
	currentState = newState;
}

void Monster::HandleState(float dt)
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
    }
}

void Monster::Idle()
{
    animation.Play(stat.name + "Idle");
    SetOrigin(origin);
    SetRectBox();

    sf::Vector2f playerPos = player->GetPosition();
    float distance = Utils::Distance(playerPos, position);

    if (hp <= 0 || !player->IsAlive())
    {
        SetState(MonsterState::Dead);
        return;
    }
    else if (distance <= stat.searchRange || isAwake)  //���ݹ��� ~ Ž�� ����
    {
        isAwake = true;
        if (distance <= stat.attackRange)
        {
            if (attackTimer >= stat.attackRate)
                SetState(MonsterState::Attacking);      
            return;
        }  
        else
            SetState(MonsterState::Moving);
    }
}

void Monster::Attack(float dt)
{
    if (attackTimer >= stat.attackRate)
    {
        animation.Play(stat.name + "Attack");
        SetOrigin(origin);
        SetRectBox();
        attackTimer = 0.f;
        isAttacked = false;
    }
    if (!isAttacked && player->IsAlive())
    {
        if (sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
        {
            SpriteEffect* playerHitEffect = playerHitEffectPool.Get();
            sf::Vector2f randPos(Utils::RandomRange(0.f, 10.f), Utils::RandomRange(0.f, 10.f));
            playerHitEffect->SetPosition(player->GetPosition() + randPos);
            float randAngle = Utils::RandomRange(0.f, 360.f);
            playerHitEffect->sprite.setRotation(randAngle);
            SCENE_MGR.GetCurrScene()->AddGo(playerHitEffect);

            attackTimer = 0.f;
            player->SetHp(-stat.damage);
            isAttacked = true;
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
    else if (distance <= stat.searchRange || isAwake)  //���ݹ��� ~ Ž�� ����
    {
        if (distance > stat.attackRange)
        {
            SetState(MonsterState::Moving);
            return;
        }      
    }
    else if (animation.IsAnimEndFrame())
        SetState(MonsterState::Idle());
}

void Monster::Move(float dt)
{
    if (animation.GetCurrentClipId() != stat.name + "Run")
    {
        animation.Play(stat.name + "Run");
        SetOrigin(origin);
        SetRectBox();
    }

    sf::Vector2f playerPos = player->GetPosition();
    SetLook(playerPos);
    raycaster.Rotation(Utils::Angle(look));
    prevPos = position;

    
    if (raycaster.CheckShortestPath(position, player->GetPosition(), *nongroundTiles, tilesWorld).first)
        SetPosition(position + look * stat.speed * dt);    
    else
    {     
        pathUpdateTimer += dt;

        if (path.second.empty() || pathUpdateTimer > pathUpdateRate)
        {
            Pair src(currentTile->GetIndex().x, currentTile->GetIndex().y);
            Pair dst(player->GetCurrentTile()->GetIndex().x, player->GetCurrentTile()->GetIndex().y);
            path = _AS.aStarSearch(*intMap, src, dst);  //false면 isAwake를 false로. SetState(Idle).
            
            if (!path.first)
            {
                SetState(MonsterState::Idle);
                isAwake = false;
                //return;
            }
            
            pathUpdateTimer = 0.f;
        }
        else if (!path.second.empty())
        {
            Pair nextIndex = path.second.top();
            if ((*tilesWorld)[nextIndex.first][nextIndex.second]->GetTileGlobalBounds().contains(position))
                path.second.pop();
            if (!path.second.empty())
                nextIndex = path.second.top();
            nextTile = (*tilesWorld)[nextIndex.first][nextIndex.second];
            sf::Vector2f center = nextTile->GetPosition() + sf::Vector2f(_TileSize / 2, _TileSize / 2);
            direction = Utils::Normalize(center - position);
        }
        SetPosition(position + direction * stat.speed * dt);
    }
    

    CalculatorCurrentTile();
    if (currentTile->GetType() == TileType::Wall
        || currentTile->GetType() == TileType::Cliff)
        SetPosition(prevPos);

    float distance = Utils::Distance(playerPos, position);
    if (hp <= 0 || !player->IsAlive())
    {
        SetState(MonsterState::Dead);
        return;
    }
    else if (distance > stat.searchRange && !isAwake)
    {
        SetState(MonsterState::Idle);
        return;
    }
    else if (distance <= stat.attackRange)
    {
        if (attackTimer >= stat.attackRate)
            SetState(MonsterState::Attacking);
        return;
    }
}

void Monster::Die()
{
    if (animation.GetCurrentClipId() != stat.name + "Death")
    {
        animation.Play(stat.name + "Death");
        SetOrigin(origin);
        SetRectBox();
        std::cout << "Monster::Die()" << std::endl;
    }
    else if (animation.IsAnimEndFrame())
        SetActive(false);
}

void Monster::KnockBack(float dt)
{
    if (animation.GetCurrentClipId() != stat.name + "Hurt")
    {
        animation.Play(stat.name + "Hurt");
        SetOrigin(origin);
        SetRectBox();
    }

    prevPos = position;
    SetPosition(position + -look * 500.f * dt);
    CalculatorCurrentTile();
    if (currentTile->GetType() == TileType::Wall)
        SetPosition(prevPos);
    
    knockBackTimer += dt;
    if (knockBackTimer > knockBackTime)
    {
        knockBackTimer = 0;

        sf::Vector2f playerPos = player->GetPosition();
        float distance = Utils::Distance(playerPos, position);


        if (hp <= 0 || currentTile->GetType() == TileType::Cliff)
        {
            SetState(MonsterState::Dead);
            return;
        }
        else if (distance <= stat.searchRange || isAwake)
        {
            SetState(MonsterState::Moving);
            return;
        }
        else if (distance <= stat.attackRange && attackTimer >= stat.attackRate)
        {
            SetState(MonsterState::Attacking);
            return;
        }
        else
            SetState(MonsterState::Idle);
    }
}

const sf::Vector2f Monster::SetLook(sf::Vector2f playerPos)
{
    look = Utils::Normalize(playerPos - position);
    if (look.x < 0)
        SetFlipX(true);
    else
        SetFlipX(false);

    return look;
}

void Monster::OnAttacked(float damage)  //�÷��̾�� ���͸� ������ �� ȣ��
{
    if (currentState != MonsterState::Dead)
    {
        SetState(MonsterState::KnockBack);
        hp -= damage;
    }
}

void Monster::CalculatorCurrentTile()
{
    int rowIndex = position.x / _TileSize;
    int columnIndex = position.y / _TileSize;

    currentTile = (*tilesWorld)[rowIndex][columnIndex];
}

//객체를 중심으로 임의 범위 내의 타일을 반환
std::vector<Tile*> Monster::CalculatorRangeTiles(int row, int col)
{
    int searchRowRange = row;
    int searchColRange = col;

    sf::Vector2i index = currentTile->GetIndex();
    std::vector<Tile*> tiles;

    int topRowIndex = index.x - searchRowRange < 0 ? 0 : index.x;
    int leftColumnIndex = index.y - searchColRange < 0 ? 0 : index.y;
    int bottomRowIndex = index.x + searchRowRange >= tilesWorld->size() ? tilesWorld->size() - 1 : index.x + searchRowRange;
    int rightColumnIndex = index.y + searchColRange >= tilesWorld[0].size() ? tilesWorld[0].size() - 1 : index.y + searchColRange;

    for (int i = topRowIndex; i < bottomRowIndex; i++)
    {
        for (int j = leftColumnIndex; j < rightColumnIndex; j++)
        {
            tiles.push_back((*this->tilesWorld)[i][j]);
        }
    }
    return tiles;
}

void Monster::SetRectBox()
{
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    rect.setSize({ spriteBounds.width, spriteBounds.height });
    Utils::SetOrigin(rect, Origins::MC);
}

void Monster::SetParticle(sf::Vector2f position, int count)
{
    for (int i = 0; i < count; i++)
    {
        Particle* particle = particlePool->Get();
        particle->SetPosition(position);
        SCENE_MGR.GetCurrScene()->AddGo(particle);
    }
}

void Monster::SetParticlePool(ObjectPool<Particle>* pool)
{
    particlePool = pool;
};
