#include "stdafx.h"
#include "ElementalSpell.h"
#include "SceneGame.h"
#include "Player.h"
#include "Monster.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "Framework.h"
#include "BoxCollider2D.h"
#include "Tile.h"

ElementalSpell::ElementalSpell(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

ElementalSpell::~ElementalSpell()
{
}

void ElementalSpell::Init()
{
	Collider = (BoxCollider2D*)scene->AddGo(new BoxCollider2D());
}

void ElementalSpell::Release()
{
}

void ElementalSpell::Reset()
{
	SpriteGo::Reset();

	// ���Ϸ� �޾Ƽ� �����ϴ� ��� �����ϱ�
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlashLarge.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlashMed.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlashSmall.csv"));
	
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/FireBall/FireBall.csv"));

	sprite.setScale(2, 2);
	anim.SetTarget(&sprite);
}

void ElementalSpell::Update(float dt)
{
	SpriteGo::Update(dt);

	Collider->SetSprite(sprite);
	Collider->SetColSize();

	raycaster.move(position.x, position.y);
	raycaster.checkCollision(monster);

	// �̰� �÷��̾�� �޴°� �ƴ϶� ��ų���� �޴´�.
	currentEvent = player->GetSkillEvent();
	switch(currentEvent)
	{
	case SkillEvents::Left:
		currentSkillType = SkillTypes::Melee;
		break;
	case SkillEvents::Right:
		currentSkillType = SkillTypes::Range;
		break;
	case SkillEvents::Q:
		break;
	case SkillEvents::Space:
		break;
	}


	switch (currentSkillType)
	{
	case SkillTypes::Melee:
		angle = player->GetPlayerLookAngle() + 90;
		MeleeUpdate(dt);
		break;

	case SkillTypes::Range:
		angle = player->GetPlayerLookAngle();
		RangeUpdate(dt);
		break;
	}



	anim.Update(dt);
	SetOrigin(Origins::MC);
	Collider->SetOrigin(Origins::MC);
	//CalculatorCurrentTile();
}

void ElementalSpell::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	raycaster.draw(window);
}

void ElementalSpell::MeleeUpdate(float dt)
{
	// isCol�� �ݶ��̴����� �ε����� true
	// player->IsAttack() �� ��¥ ���� Ÿ�̹� 

	// ���� ����
	if (player->IsAttack() && !isSpawn)
	{
		isSpawn = true;
		Collider->SetActive(true);
		Collider->SetPosition(player->GetAttackPos());
		SetPosition(player->GetAttackPos());
		sprite.setRotation(angle);
		raycaster.Rotation(player->GetPlayerLookAngle());
		Collider->GetObbCol().setRotation(angle);
		comboQueue.push(FRAMEWORK.GetGamePlayTime());
	}

	// �޺� �ִ� ����
	if (!comboQueue.empty())
	{
		if (comboQueue.front() - prevComboTime > comboDuration)
		{
			attackCount = 0;
			//std::cout << "combo is Failed" << std::endl;
		}

		if (comboQueue.front() - prevComboTime < comboDuration)
		{
			attackCount++;
			//std::cout << "combo is Succesed" << std::endl;
		}

		// �ε����� �̿��ؼ� ���� �����̳� �� ������ �ش� �ִϸ��̼� �����ϴ� ��� ����
		if (attackCount % 3 == 0)
		{
			anim.Play("WindSlashSmall");
		}
		if (attackCount % 3 == 1)
		{
			anim.Play("WindSlashMed");
		}
		if (attackCount % 3 == 2)
		{
			anim.Play("WindSlashLarge");
		}

		//std::cout << attackCount << std::endl;
		//std::cout << comboQueue.front() - prevComboTime << std::endl;
		prevComboTime = comboQueue.front();
		comboQueue.pop();
	}


	// �̰ɷ� ����
	isCol = Collider->ObbCol(monster->rect);

	// ������ ���� Ÿ�̹�
	if (player->IsAttack() && isCol && !isAttack)	// �ѹ��� �ƴ� �ǽð����� �����
	{
		// ����ĳ��Ʈ�� ���� ������ �����ֱ�
		raycaster.GetEndPos();
		player->GetMonster()->OnAttacked(1);
		isAttack = true;
	}

	// �ߺ� ���� ������
	if (isSpawn)
	{
		attackTimer += dt;
	}

	if (attackTimer > attackDuration)
	{
		attackTimer = 0.f;
		isAttack = false;
		isSpawn = false;
		Collider->SetActive(false);
	}
}

void ElementalSpell::RangeUpdate(float dt)
{

	// ���� ����
	if (player->IsAttack() && !isSpawn)
	{
		isSpawn = true;
		Collider->SetActive(true);
		SetPosition(player->GetAttackPos());
		sprite.setRotation(angle);
		raycaster.Rotation(player->GetPlayerLookAngle());
		Collider->GetObbCol().setRotation(angle);
		anim.Play("FireBall");
		dir = player->GetLook();

	}


	// �̰ɷ� ����
	isCol = Collider->ObbCol(monster->rect);

	// ������ ���� Ÿ�̹�
	if (player->IsAttack() && isCol && !isAttack)	// �ѹ��� �ƴ� �ǽð����� �����
	{
		// ����ĳ��Ʈ�� ���� ������ �����ֱ�
		raycaster.GetEndPos();
		player->GetMonster()->OnAttacked(1);
		isAttack = true;

	}

	// �ߺ� ���� ������
	if (isAttack)
	{
		attackTimer += dt;
	}

	if (attackTimer > attackDuration)
	{
		attackTimer = 0.f;
		isAttack = false;
		isSpawn = false;
		Collider->SetActive(false);
	}
	


	sf::Vector2f movePos = GetPosition();
	movePos.x += speed * dt;
	movePos.y = GetPosition().y + amplitude * std::sin(2 * M_PI * frequency * (movePos.x / speed));


	SetPosition(movePos);


	Collider->SetPosition(GetPosition());

	//sf::Vector2f movePos = GetPosition();
	//movePos += dir * speed * dt;
	//SetPosition(movePos);
	//Collider->SetPosition(GetPosition());
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
