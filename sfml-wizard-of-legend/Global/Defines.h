#pragma once

#define _USE_MATH_DEFINES


enum class SkillDatas
{
	Exist,
	Buyed,
	Equiped,
	Count,
};

// Id 추가 시 ConvertNameToId 추가해야함
enum class SkillIds
{
	None = -1,
	ExplodingFireball,
	WindSlash,
	DragonArc,
	EarthKnuckles,
	FlameCleaver,
	FlameStrike,
	FrostFan,
	GustVolley,
	IceDagger,
	StoneShot,
	VoltDisc,
	Count,
};

enum class SkillEvents
{
	None = -1,
	Left,
	Space,
	Right,
	Q,
	E,
	R,
	Tab,
	M,
	Count,
};

enum class SkillTypes
{
	None = -1,
	Melee,
	Range,
	Petrol,
};

enum class RangeTypes
{
	None = -1,
	Straight,
	Curve,
};

enum class PlayerActions
{
	None = -1,
	Hand,
	Kick,
	Jump,
	Slam,
	Focus,
	HandKick,
	JumpKick,
	JumpSlam,
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
	CHINA,
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
	Pathfinding,
	Count,
};

enum class TileType
{
	None = -1,
	Ground,
	Cliff,
	Wall,
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

enum class SpawnLocation
{
	None = -1,
	Monster,
	Object,
	WallObject,
	Torch,
	Roof,
	FireLight,
	Portal,
};

#include "AnimationClip.h"
#define _TileSize 64
using RectVertexArray = std::array<sf::Vector2f, 4>;