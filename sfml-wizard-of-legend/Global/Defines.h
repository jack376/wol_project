#pragma once

#define _USE_MATH_DEFINES

enum class SkillEvents
{
	None = -1,
	Left,
	Right,
	Space,
	Q,
	Count,
};

enum class SkillTypes
{
	None = -1,
	Melee,
	Range,
};

enum class PlayerActions
{
	None = -1,
	Hand,
	Kick,
	Jump,
	Slam,
	Focus,
};

enum class ElementTypes
{
	None = -1,
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
	Title,
	Game,
	Editor,
	SkillEditor,
	Count,
};

enum class TileType
{
	None,
	Ground,
	Cliff,
	Wall,
	MonsterSpawn,
	EventTrigger,
};

enum class ObjectType
{
	None = -1,
	Platform,
	Item,
	Coin,
	Count,
};


enum class UiType
{
	Text,
	Box,
};

#include "AnimationClip.h"
#define _TileSize 64
using RectVertexArray = std::array<sf::Vector2f, 4>;