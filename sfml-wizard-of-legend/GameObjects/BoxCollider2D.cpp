#include "stdafx.h"
#include "BoxCollider2D.h"

BoxCollider2D::BoxCollider2D(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

BoxCollider2D::~BoxCollider2D()
{
}

void BoxCollider2D::Init()
{
	obbCol.setFillColor(sf::Color::Transparent);
	obbCol.setOutlineColor(sf::Color::Magenta);
	obbCol.setOutlineThickness(1.f);
	obbCol.setSize({100, 100});
	SetOrigin(Origins::MC);
}

void BoxCollider2D::Release()
{
}

void BoxCollider2D::Reset()
{
	SpriteGo::Reset();
}

void BoxCollider2D::Update(float dt)
{
	SpriteGo::Update(dt);
	SetColSize();
}

void BoxCollider2D::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	window.draw(obbCol);
}

void BoxCollider2D::SetPosition(const sf::Vector2f& p)
{
	SpriteGo::SetPosition(p);
	obbCol.setPosition(p);
}

void BoxCollider2D::SetPosition(float x, float y)
{
	SpriteGo::SetPosition(x, y);
	obbCol.setPosition(position);

}

void BoxCollider2D::SetOrigin(Origins origin)
{
	SpriteGo::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(obbCol, origin);
	}

}

void BoxCollider2D::SetOrigin(float x, float y)
{
	SpriteGo::SetOrigin(x, y);
	obbCol.setOrigin(x, y);
}

void BoxCollider2D::SetColSize()
{
	//sf::Vector2f size = { sprite1.getGlobalBounds().width, sprite1.getGlobalBounds().height };
	sf::Vector2f size = { sprite1.getLocalBounds().width, sprite1.getLocalBounds().height };

	obbCol.setSize(size);
	obbCol.setScale(sprite1.getScale());
	//std::cout << "X : " << size.x << std::endl;
	//std::cout << "Y : " << size.y << std::endl;}
}

bool BoxCollider2D::ObbCol(const sf::RectangleShape& other)
{
	RectVertexArray vertices1 = Utils::GetVertices(obbCol);
	RectVertexArray vertices2 = Utils::GetVertices(other);
	std::array<sf::Vector2f, 4> axes = Utils::GetPerpendicularAxes(vertices1, vertices2);

	for (const auto& axis : axes)
	{
		sf::Vector2f proj1 = Utils::ProjectOnAxis(vertices1, axis);
		sf::Vector2f proj2 = Utils::ProjectOnAxis(vertices2, axis);

		if (!Utils::AreOverlapping(proj1, proj2))
		{
			return false;
		}
	}
	return true;
}

void BoxCollider2D::ObbSet()
{
	SetObbRotation();
	SetObbOrigins();
	SetObbPosition();
}
