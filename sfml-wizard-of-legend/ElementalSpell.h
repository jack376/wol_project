#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class SpriteGo;
class SceneGame;
class Player;

class ElementalSpell : public SpriteGo
{
protected:
	// ����� �ϵ��ڵ������� �����͸� �Ѱ� �޴� ������ ����
	SceneGame* scene;
	Player* player;

	AnimationController anim;
	SpriteGo* collider;


	// ���� ���� sf::Vector2f
	sf::Vector2f dir;


	// �ӵ� ���� sf::Vector2f
	sf::Vector2f speed;


	// ���� ����
	bool isSpawn = false;

	// ���� �浹
	bool isCol = false;

	bool isAttack = false;
	
	float attackTimer = 0.f;
	float attackDuration = 0.5f;

	int attackCount = 0;



public:
	ElementalSpell(const std::string& textureId = "", const std::string& n = "");
	virtual ~ElementalSpell() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;


	void SetScene(SceneGame* scene) { this->scene = scene; }
	void SetPlayer(Player* player) { this->player = player; }
};

