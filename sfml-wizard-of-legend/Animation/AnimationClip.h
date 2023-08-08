#pragma once

enum class AnimationLoopTypes
{
	Single,
	Loop,
};

struct AnimationFrame
{
<<<<<<< HEAD
	std::string textureId;	
=======
	std::string textureId;
>>>>>>> feature/player
	sf::IntRect texCoord;	//texture ��ǥ
	std::function<void()> action;	//�Լ� ������ ��� std::function���
};


struct AnimationClip
{
	std::string id;
	std::vector<AnimationFrame> frames;
	AnimationLoopTypes loopType;
	int fps;

	bool LoadFromFile(const std::string path);
};