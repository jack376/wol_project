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


enum class UiType
{
	Text,
	Box,
};

#include "AnimationClip.h"
using RectVertexArray = std::array<sf::Vector2f, 4>;