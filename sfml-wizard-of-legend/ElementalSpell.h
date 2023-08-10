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

	std::queue <float> comboQueue;


	// ���� ���� sf::Vector2f
	sf::Vector2f dir;



	// ��ġ ���� sf::Vector2f
	sf::Vector2f spawnPos;



	float speed;


	// ���� ����

	bool isCombo = false;

	// ��ų�� �����Ǿ����� ����
	bool isSpawn = false;

	// ���� �浹
	bool isCol = false;

	// ��ų�� �����ߴ��� ���� / �ߺ�����
	bool isAttack = false;
	
	// �ߺ� ���� ����
	float attackTimer = 0.f;
	float attackDuration = 0.5f;

	// �޺� Ÿ�̸� ����
	float prevComboTime = -100.f;

	float comboTimer = 0.f;
	float comboDuration = 0.6f;

	// �޺� Ƚ��
	int attackCount = 0;

	// �ִϸ��̼� ����
	float angle = 0.f;

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

