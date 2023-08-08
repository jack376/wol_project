#pragma once

#define _USE_MATH_DEFINES

enum class AttactType
{
	Melee,
	Ranged,
};

struct MonsterInfo
{
	std::string name;
	AttactType type;
	int maxHp;
	float speed;
	int damage;
	float attackRate;	 //공격 속도
	float attackRange;	//공격 범위
	float searchRange;	//탐색 범위
};


enum class ElementTypes
{
	Fire,
	Water,
	Thunder,
	Earth,
	Wind,
};

enum class Languages
{
	KOR,
	ENG,
	JP,
	COUNT,
};

enum class Origins
{
	TL,
	TC,
	TR,
	ML,
	MC,
	MR,
	BL,
	BC,
	BR,
	CUSTOM,
};

enum class ResourceTypes
{
	Texture,
	Font,
	SoundBuffer,
	AnimationClip,
	AnimationSheetClip,
};

enum class SceneId
{
	None = -1,
	Game,
	Title,
	Game,
	Count,
};

enum class ObjectType
{
	None = -1,
	Platform,
	Item,
	Coin,
	Count,
};

enum class PatternObjectType
{
	None = -1,
	Platform,
	Obstacle,
	ItemSpeedUp,
	ItemBigHealPack,
	ItemHealPack,
	ItemMagnet,
	ItemBig,
	//ItemBonus,
	ItemCoin,
	Coin,
	BigCoin,
	GoldCoin,
	BigGoldCoin,
	Diamond,
	DiamondBox,
	LuckyBox,
	Count,

};
#include "AnimationClip.h"