#pragma once
#include "SpriteGo.h"
class Skill : public SpriteGo
{
protected:
	// UI������ �ؽ��� �ʿ�?
	// ���� ��ų �ִϸ��̼� Ŭ�� ��� ��������

public:
	Skill(const std::string& textureId = "", const std::string& n = "");
	virtual ~Skill() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

};

