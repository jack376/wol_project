#include "stdafx.h"
#include "ElementalSpell.h"
#include "SceneGame.h"
#include "Player.h"
#include "Monster.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "Framework.h"
#include "Tile.h"
#include "SceneMgr.h"

ElementalSpell::ElementalSpell(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

ElementalSpell::~ElementalSpell()
{
}

void ElementalSpell::Init()
{
	Collider.Init();
}

void ElementalSpell::Release()
{
}

void ElementalSpell::Reset()
{
	SpriteGo::Reset();

	// ���Ϸ� �޾Ƽ� �����ϴ� ��� �����ϱ�
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlash.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/ExplodingFireball/ExplodingFireball.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/FrostFan/FrostFan.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/IceDagger/IceDagger.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/GustVolley/GustVolley.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/VoltDisc/VoltDisc.csv"));

	
	amplitude = currentInfo.amplitude;
	damage = currentInfo.damage;
	attackDuration = currentInfo.delayDuration;
	currentInfo.explosionRange;
	frequency = currentInfo.frequency;
	range = currentInfo.range;
	currentAnimId = currentInfo.skillName;
	speed = currentInfo.speed;
	curveSpeed = currentInfo.speed;

	if (currentAnimId.compare("FrostFan") == 0)
	{
		sprite.setScale(2, 3);
	}
	else if (currentAnimId.compare("GustVolley") == 0)
	{
		sprite.setScale(4, 4);
	}
	else if (currentAnimId.compare("VoltDisc") == 0)
	{
		sprite.setScale(4, 4);
	}
	else
		sprite.setScale(2, 2);

	anim.SetTarget(&sprite);
}

void ElementalSpell::Update(float dt)
{
	SpriteGo::Update(dt);

	Collider.SetSprite(sprite);
	Collider.SetColSize();


	raycaster.move(position.x, position.y);
	for (auto monster : monsters)
	{
		if(monster->GetActive())
			raycaster.checkCollision(monster);
	}

	switch (currentSkillType)
	{
	case SkillTypes::Melee:
		angle = player->GetPlayerLookAngle() + 90;
		MeleeUpdate(dt);
		break;

	case SkillTypes::Range:
		//angle = player->GetPlayerLookAngle() + 90;
		RangeUpdate(dt);
		break;
	}



	anim.Update(dt);
	SetOrigin(Origins::MC);
	Collider.SetOrigin(Origins::MC);
	//CalculatorCurrentTile();
}

void ElementalSpell::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	if(Collider.GetActive())
		Collider.Draw(window);
	raycaster.draw(window);
}

void ElementalSpell::MeleeUpdate(float dt)
{
	// isCol�� �ݶ��̴����� �ε����� true
	// player->IsAttack() �� ��¥ ���� Ÿ�̹� 

	// ���� ����
	if (!isSpawn)
	{
		isSpawn = true;
		Collider.SetActive(true);
		Collider.SetPosition(player->GetAttackPos());
		SetPosition(player->GetAttackPos());
		sprite.setRotation(angle);
		raycaster.Rotation(player->GetPlayerLookAngle());
		Collider.GetObbCol().setRotation(angle);
		comboQueue.push(FRAMEWORK.GetGamePlayTime());
	}

	// �޺� �ִ� ����
	if (!comboQueue.empty())
	{
		if (comboQueue.front() - prevComboTime > comboDuration)
		{
			attackCount = 0;
		}

		if (comboQueue.front() - prevComboTime < comboDuration)
		{
			attackCount++;
		}
		anim.Play(currentAnimId);

		// �ε����� �̿��ؼ� ���� �����̳� �� ������ �ش� �ִϸ��̼� �����ϴ� ��� ����
		//if (attackCount % 3 == 0)
		//{
		//	anim.Play("WindSlashSmall");
		//}
		//if (attackCount % 3 == 1)
		//{
		//	anim.Play("WindSlashMed");
		//}
		//if (attackCount % 3 == 2)
		//{
		//	anim.Play("WindSlashLarge");
		//}

		prevComboTime = comboQueue.front();
		comboQueue.pop();
	}


	// �̰ɷ� ����
	for (auto monster : monsters)
	{
		isCol = Collider.ObbCol(monster->rect) && monster->GetActive();

		auto it = std::find(colMonsters.begin(), colMonsters.end(), monster);

		// ������ ���� Ÿ�̹�
		if (isCol && it == colMonsters.end())	// �ѹ��� �ƴ� �ǽð����� �����
		{
			// ����ĳ��Ʈ�� ���� ������ �����ֱ�
			attackTimer = 0.f;
			raycaster.GetEndPos();
			monster->OnAttacked(damage);
			colMonsters.push_back(monster);
		}
	}



	// �ߺ� ���� ������
	if (isSpawn)
	{
		attackTimer += dt;
	}

	if (attackTimer > attackDuration)
	{
		attackTimer = 0.f;

		//isSpawn = false;
		Collider.SetActive(false);
	}

	// �����Ÿ� ���̸� isSpawn false��



	// isSpawn �� false�� �Ǹ� �������
	if (!isSpawn)
	{
		colMonsters.clear();
		scene->RemoveGo(this);
		pool.Return(this);
	}
}

void ElementalSpell::RangeUpdate(float dt)
{

	// ���� ����
	if (!isSpawn)
	{
		isSpawn = true;
		Collider.SetActive(true);
		SetPosition(player->GetAttackPos());
		sprite.setRotation(angle);
		raycaster.Rotation(player->GetPlayerLookAngle());
		Collider.GetObbCol().setRotation(angle);
		anim.Play(currentAnimId);
		//dir = player->GetLook();
		curveAngle = angle;
		time = 0.f;
	}


	// �̰ɷ� ����
	for (auto monster : monsters)
	{
		isCol = Collider.ObbCol(monster->rect);
		auto it = std::find(colMonsters.begin(), colMonsters.end(), monster);

		// ������ ���� Ÿ�̹�
		if (isCol && it == colMonsters.end())	// �ѹ��� �ƴ� �ǽð����� �����
		{
			// ����ĳ��Ʈ�� ���� ������ �����ֱ�
			raycaster.GetEndPos();
			monster->OnAttacked(damage);
			monster->SetIsHit(true);
			colMonsters.push_back(monster);
		}
	}


	// �ߺ� ���� ������
	if (isSpawn)
	{
		attackTimer += dt;
	}

	if (attackTimer > attackDuration)
	{
		attackTimer = 0.f;
		//isSpawn = false;
		Collider.SetActive(false);
	}




	
	// � ���� ǥ��
	switch (currentRangeType)
	{
	case RangeTypes::Straight:
		StraightUpdate(dt);
		break;

	case RangeTypes::Curve:
		CurveUpdate(dt);
		break;
	}


	// �����Ÿ� ���̸� isSpawn false��
	

	// isSpawn �� false�� �Ǹ� �������
	if (!isSpawn)
	{
		pool.Return(this);
		colMonsters.clear();
		scene->RemoveGo(this);
	}

}

void ElementalSpell::CurveUpdate(float dt)
{
	time += dt;
	sf::Vector2f movePos = GetPosition() + CalAxisSin(time, curveSpeed, frequency, amplitude, dir, curveAngle);
	SetPosition(movePos);
	Collider.SetPosition(GetPosition());
}

void ElementalSpell::StraightUpdate(float dt)
{
	sf::Vector2f movePos = GetPosition();
	movePos += dir * speed * dt;
	SetPosition(movePos);
	Collider.SetPosition(GetPosition());
}

sf::RectangleShape& ElementalSpell::GetCollider()
{
	return Collider.GetObbCol();
}

sf::Vector2f ElementalSpell::CalAxisSin(float time, float speed, float frequency, float amplitude, const sf::Vector2f& axis, float angleInDegrees)
{
	float x = time * speed;
	float y = amplitude * std::sin(2 * M_PI * frequency * (x / speed));

	// Calculate rotated position
	float angleInRadians = angleInDegrees * (M_PI / 180.0f);
	float rotatedX = axis.x + x * std::cos(angleInRadians) - y * std::sin(angleInRadians);
	float rotatedY = axis.y + x * std::sin(angleInRadians) + y * std::cos(angleInRadians);

	return sf::Vector2f(rotatedX, rotatedY);
}

void ElementalSpell::CalculatorCurrentTile()
{
	int rowIndex = position.x < 0 ? 0 : position.x / _TileSize;
	int columnIndex = position.y < 0 ? 0 : position.y / _TileSize;

	currentTile = (*wouldTiles)[rowIndex][columnIndex];
}

std::vector<Tile*> ElementalSpell::CalculatorRangeTiles(int row, int col)
{
	//32x16
	int searchRowRange = row;
	int searchColRange = col;

	sf::Vector2i index = currentTile->GetIndex();
	std::vector<Tile*> tiles;

	int topRowIndex = index.x - searchRowRange < 0 ? 0 : index.x > wouldTiles->size() * _TileSize ? wouldTiles->size() * _TileSize : index.x;
	int leftColumnIndex = index.y - searchColRange < 0 ? 0 : index.y > wouldTiles[0].size() * _TileSize ? wouldTiles[0].size() * _TileSize : index.y;
	for (int i = topRowIndex; i < index.x + searchRowRange; i++)
	{
		for (int j = leftColumnIndex; j < index.y + searchColRange; j++)
		{
			tiles.push_back((*this->wouldTiles)[i][j]);
		}
	}

	return tiles;
}
