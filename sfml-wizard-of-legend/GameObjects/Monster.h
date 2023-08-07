#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class Player;

enum class MonsterState 
{
	Idle,
	Moving,
	Attacking,
	Dead,
	KnockBack,
};

enum class MonsterId
{
	Ghoul,
	Knight,
};


class Monster :
    public SpriteGo
{
protected:
	MonsterId monsterId;
	MonsterState currentState = MonsterState::Idle;
	AnimationController animation;

	MonsterInfo stat;
	int hp;
	float attackTimer = 0.f; //timer�� rate���� Ŀ���� ���� Ÿ�̹�

	Player* player = nullptr;

	sf::Vector2f look; //�ٶ󺸴� ����
	sf::Vector2f direction; //�̵��ϴ� ����

public:
	Monster(MonsterId id, const std::string& textureId = "", const std::string& n = "");
	virtual ~Monster() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

    void SetState(MonsterState newState);
	void HandleState();

	virtual void Attack();
	void Move(float dt);
	void Die();
	
	void SetLook(sf::Vector2f playerPos);
	void SetPlayer(Player* player) { this->player = player; }
	void HandleBehavior(float dt);
};


//����
//��ġ(����)
//Ž��(�÷��̾�)
//�̵� �� ����
