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

}

void BoxCollider2D::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	
}

bool BoxCollider2D::ObbCol(const sf::RectangleShape& col1, const sf::RectangleShape& col2)
{
	RectVertexArray vertices1 = Utils::GetVertices(col1);
	RectVertexArray vertices2 = Utils::GetVertices(col2);
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