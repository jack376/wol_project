#include "stdafx.h"
#include "Player.h"

Player::Player(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

Player::~Player()
{
}

void Player::Init()
{
	SpriteGo::Init();
}

void Player::Release()
{
	SpriteGo::Release();

}

void Player::Reset()
{
	SpriteGo::Reset();

	//anim.Play();
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);

}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

}
