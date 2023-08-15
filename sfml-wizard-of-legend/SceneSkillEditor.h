#pragma once
#include "Scene.h"

class TextGo;
class SpriteGo;
class UIButton;
class SkillMgr;
class Player;


class SceneSkillEditor : public Scene
{
protected:
	sf::Vector2f windowSize;

	UIButton* playerAction;
	UIButton* skillIconId;
	UIButton* damage;
	UIButton* comboDamage;
	UIButton* maxSkillCharge;
	UIButton* shotCount;

	UIButton* speed;
	UIButton* range;
	UIButton* explosionRange;
	UIButton* delayDuration;
	UIButton* damageDelay;
	UIButton* coolTime;
	UIButton* rotateSpeed;

	UIButton* isPenetrating;
	UIButton* canMoveDuringSkill;

	Player* player;


public:
	SceneSkillEditor();
	virtual ~SceneSkillEditor() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

};

