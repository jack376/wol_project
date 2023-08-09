#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class SpriteGo;
class SceneGame;
class Player;

class ElementalSpell : public SpriteGo
{
protected:
	// 현재는 하드코딩이지만 데이터를 넘겨 받는 식으로 수정
	SceneGame* scene;
	Player* player;

	AnimationController anim;
	SpriteGo* collider;


	// 방향 벡터 sf::Vector2f
	sf::Vector2f dir;


	// 속도 벡터 sf::Vector2f
	sf::Vector2f speed;


	// 상태 변수
	bool isSpawn = false;

	// 몬스터 충돌
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

