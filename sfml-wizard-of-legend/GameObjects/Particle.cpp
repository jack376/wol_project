#include "stdafx.h"
#include "Particle.h"
#include "Utils.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneEditor.h"
#include "SceneGame.h"

void Particle::Reset()
{
	// Texture Load
	sf::Texture* tex = RESOURCE_MGR.GetTexture(textureId);
	if (tex != nullptr) { sprite.setTexture(*tex); }

	flowTime = setFlowTime;
	velocity = setVelocity;
	duration = setDuration;
	speed    = setSpeed;
	rotation = setRotation;

	currentTime = 0.0f;

	float randomVal = Utils::RandomRange(0.2f, 1.0f);
	sf::Vector2f randomDir = Utils::RandomOnCircle(1.0f);

	direction  = randomDir;
	speed     *= randomVal;
	duration  *= randomVal;

	sprite.setRotation((rotation * randomVal) * 360.0f);
	sprite.setOrigin(8.0f, 8.0f); // Based 16px, Origin MC
	sprite.setPosition(0.0f, 0.0f);
	sprite.setScale(setScale);
	sprite.setColor(setColor);
}

void Particle::Update(float dt)
{		
	flowTime += dt;
	currentTime += dt;
	speed -= slowdown * dt;
	velocity += gravity * flowTime;

	position += direction * speed * dt;
	sprite.setPosition(setAddPosition.x + position.x, setAddPosition.y + position.y + velocity);

	UINT8 alpha = Utils::Lerp(255, 0, (flowTime / duration));
	sprite.setColor({ setColor.r, setColor.g, setColor.b, alpha });

	if (isAnimation)
	{
		if (currentFrame == 5)
		{
			currentFrame = 0;
		}
		if (currentTime >= frameTime)
		{
			sprite.setTextureRect(animationRect);
			currentFrame++;
			currentTime = 0.0f;
		}
	}

	if (flowTime > duration)
	{
		if (pool != nullptr)
		{
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);
		}
		else
		{
			SetActive(false);
		}
	}
}

void Particle::Draw(sf::RenderWindow& window)
{
	isLighting ? window.draw(sprite, sf::BlendAdd) : window.draw(sprite);
}

void Particle::SetPosition(const sf::Vector2f& p)
{
	position = p;
	sprite.setPosition(position);
}

void Particle::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	sprite.setPosition(position);
}

void Particle::SetOrigin(Origins origin)
{
	const sf::Texture* tex = sprite.getTexture();

	if (!tex)
	{
		std::cerr << "Particle SetOrigin Error" << std::endl;
		return;
	}
	sf::Vector2f originPos(sprite.getTexture()->getSize());
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;

	sprite.setOrigin(originPos);
}

void Particle::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	sprite.setOrigin(x, y);
}

void Particle::SetTexture(const std::string& id)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(id));
	textureId = id;
}

void Particle::SetTextureRect(const sf::IntRect& rect)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(textureId));
	animationRect = rect;
	sprite.setTextureRect(animationRect);
}

void Particle::SetScale(float x, float y)
{
	setScale = { x, y };
	sprite.setScale(setScale);
}

void Particle::SetColor(sf::Color color)
{
	setColor = color;
	sprite.setColor(setColor);
}

sf::Vector2f Particle::ScaleAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed)
{
	float animation = sin(flowTimeBySpeed * 10.0f * M_PI);
	float scale = defaultScale + ((animation + 1.0f) / 2.0f) * scaleRange;

	return sf::Vector2f(scale, scale);
}