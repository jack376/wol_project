#pragma once

#define _USE_MATH_DEFINES

enum class CookieTypes
{
	None = -1,
	Pancake,
	Pirate,
	Moonlighter,
};

enum class SkinType
{
	None = -1,
	Normal,
	Skin1,
};

enum class LobbyType
{
	None = -1,
	Temple,
	WizardryTower,
	Cathedral,
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
	Title,
	Editor,
	Lobby,
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