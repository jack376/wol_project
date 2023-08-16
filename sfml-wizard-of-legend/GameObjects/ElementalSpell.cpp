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

	sprite.setScale(2, 2);
	anim.SetTarget(&sprite);
}

void ElementalSpell::Update(float dt)
{
	SpriteGo::Update(dt);

	Collider->SetSprite(sprite);
	Collider->SetColSize();

	angle = player->GetPlayerLookAngle() + 90;
	// ���ڿ��������� ���� ���ľ���
	// ��ġ �������� �Ǵ� ���� �ִµ� ������ ����
	// 
	// �ϵ� �ڵ���
	// isCol�� �ݶ��̴����� �ε����� true
	// player->IsAttack() �� ��¥ ���� Ÿ�̹� 


	// ���� �ֵθ����� ���� �ʴ� Ÿ�̹�
	// �÷��̾� ������ & ������ �ȵǾ� ������
	// �ִϸ��̼� ��� & Ÿ����ġ ����

	// ���� ����
	if (player->IsAttack() && !isSpawn)
	{
		isSpawn = true;
		Collider->SetActive(true);
		Collider->SetPosition(player->GetAttackPos());
		SetPosition(player->GetAttackPos());
		sprite.setRotation(angle);
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

	anim.Update(dt);
	SetOrigin(Origins::MC);
	Collider->SetOrigin(Origins::MC);
	CalculatorCurrentTile();
}

void ElementalSpell::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	if (isSpawn)
	{
		Collider->SetActive(true);
	}
	else
		Collider->SetActive(false);
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
