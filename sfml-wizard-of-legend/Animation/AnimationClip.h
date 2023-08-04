#pragma once

enum class AnimationLoopTypes
{
	Single,
	Loop,
};

struct AnimationFrame
{
	std::string textureId;
	//sf::IntRect texCoord;
	std::function<void()> action;
};

struct AnimationSheetFrame
{
	std::string textureId;
	sf::IntRect texCoord;
	std::function<void()> action;
};



struct AnimationClip
{
	std::string id;
	AnimationLoopTypes loopType;
	int fps;

	std::vector<AnimationFrame> frames;
	std::vector<AnimationSheetFrame> framesSheet;

	bool LoadFromFile(const std::string path);
	bool LoadFromFileSheet(const std::string path);
};