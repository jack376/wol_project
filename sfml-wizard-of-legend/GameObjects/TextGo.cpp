#include "stdafx.h"
#include "TextGo.h"
#include "Utils.h"
#include "ResourceMgr.h"

TextGo::TextGo(const std::string textureId, const std::string n)
	: GameObject(n), textureId(textureId)
{
}

TextGo::~TextGo()
{
}

void TextGo::SetPosition(const sf::Vector2f& p)
{
	position = p;
	text.setPosition(position);
}

void TextGo::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	text.setPosition(position);
}

void TextGo::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(text, origin);
	}
}

void TextGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	text.setOrigin(x, y);
}

void TextGo::Init()
{

}

void TextGo::Release()
{
}

void TextGo::Reset()
{
	sf::Font* font = RESOURCE_MGR.GetFont(textureId);
	if (font != nullptr)
	{
		text.setFont(*font);
	}
	SetOrigin(origin);
}

void TextGo::Update(float dt)
{
}

void TextGo::Draw(sf::RenderWindow& window)
{
	window.draw(text);
}

void TextGo::SetString(const std::string& str)
{
	//std::wstring unicode = L"";
	//convert_ansi_to_unicode_string(unicode, getString.c_str(), getString.size());
	//return unicode.c_str();

	//std::string utf8String = str;
	//sf::String unicodeString = sf::String::fromUtf8(utf8String.begin(), utf8String.end());
	//text.setString(unicodeString);
}
