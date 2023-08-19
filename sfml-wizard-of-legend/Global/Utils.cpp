#include "stdafx.h"
#include "Utils.h"
#include <SFML/Graphics.hpp>

std::random_device Utils::rd;
std::mt19937 Utils::gen(Utils::rd());

int Utils::RandomRange(int min, int maxExclude)
{
	std::uniform_int_distribution<> dist(min, maxExclude - 1);
	return dist(gen);
}

float Utils::RandomRange(float min, float max)
{
	std::uniform_real_distribution<> dist(min, max);
	return (float)dist(gen);
}

sf::Vector2f Utils::RandomOnCircle(float radius)
{
	sf::Vector2f result(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f));
	result = Normalize(result) * radius;
	return result;
}

sf::Vector2f Utils::RandomInCircle(float radius)
{
	return RandomOnCircle(radius) * RandomValue();
}

float Utils::RandomValue()
{
	return RandomRange(0.0f, 1.0f);
}

void Utils::SetOrigin(sf::Sprite& sprite, Origins origin)
{
	SetOrigin(sprite, origin, sprite.getLocalBounds());
}

void Utils::SetOrigin(sf::Text& text, Origins origin)
{
	SetOrigin(text, origin, text.getLocalBounds());
}

void Utils::SetOrigin(sf::Shape& shape, Origins origin)
{
	SetOrigin(shape, origin, shape.getLocalBounds());
}

void Utils::SetOrigin(sf::Transformable& obj, Origins origin, const sf::FloatRect& rect)
{
	sf::Vector2f originPos(rect.width, rect.height);
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;
 	obj.setOrigin(originPos);
}

float Utils::Clamp(float v, float min, float max)
{
	return std::max(min, std::min(max, v));
}

sf::Vector2f Utils::Clamp(const sf::Vector2f& v, const sf::Vector2f& min, const sf::Vector2f& max)
{
	sf::Vector2f result;
	result.x = Clamp(v.x, min.x, max.x);
	result.y = Clamp(v.y, min.y, max.y);
	return result;
}

float Utils::Lerp(float a, float b, float t, bool clamping)
{
	float v = a + (b - a) * t;
	if (clamping)
	{
		v = Utils::Clamp(v, std::min(a, b), std::max(a, b));
	}
	return v;
}

sf::Vector2f Utils::Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t, bool clamping)
{
	if (clamping)
	{
		t = Utils::Clamp(t, 0.f , 1.f);
	}
	return  a + Utils::Normalize(b - a) * (Utils::Distance(a, b) * t);
}

sf::Color Utils::Lerp(const sf::Color& a, const sf::Color& b, float t, bool clamping)
{
	sf::Color v;
	v.r = (UINT8)Lerp(a.r, b.r, t, clamping);
	v.g = (UINT8)Lerp(a.g, b.g, t, clamping);
	v.b = (UINT8)Lerp(a.b, b.b, t, clamping);
	v.a = (UINT8)Lerp(a.a, b.a, t, clamping);
	return v;
}

const sf::Vector2f Utils::Normalize(const sf::Vector2f& vector)
{
	float mag = Magnitude(vector);
	if (mag == 0.f)
	{
		return vector;
	}
	return vector / mag;
}

float Utils::Magnitude(const sf::Vector2f& vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float Utils::SqrMagnitude(const sf::Vector2f& vector)
{
	return vector.x * vector.x + vector.y * vector.y;
}

float Utils::Distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	return Magnitude(p2 - p1);
}

float Utils::Angle(const sf::Vector2f& start, const sf::Vector2f& end)
{
	return Angle(Normalize(end - start));
}

float Utils::Angle(const sf::Vector2f& dir)
{
	return (float)(atan2(dir.y, dir.x) * (180.f / M_PI));
}

void Utils::SetShader(sf::Shader& shader, sf::Sprite& sprite, sf::Texture& palette)
{
	shader.setUniform("texture", sprite.getTexture());
	shader.setUniform("paletteTexture", palette);

	if (!shader.loadFromFile("ex_fragment_shader.frag", sf::Shader::Fragment)) 
	{
		std::cout << "Shader Load Failed!" << std::endl;
	}

	//"    vec4 grayColor = texture2D(texture, gl_TexCoord[0].xy);\n"
	//"    float grayValue = grayColor.r / 255.0;\n"
	//"    float paletteSize = float(textureSize(paletteTexture).x);\n"
	//"    vec2 paletteCoord = vec2(grayValue * paletteSize, float(textureSize(paletteTexture).y));\n"
	//"    vec4 paletteColor = texture2D(paletteTexture, paletteCoord);\n"
	//"    gl_FragColor = vec4(paletteColor, paletteColor, paletteColor, 1.0);\n"
}

sf::Sprite& Utils::SetPixelColor(sf::Image& grayImage, sf::Image& paletteImage)
{

	sf::Color paletteLine[28]; // Assuming you have a palette of 256 colors
	for (int i = 0; i < 28; ++i) {
		paletteLine[i] = paletteImage.getPixel(i * 2, 0);
		paletteLine[i].a = 255;
	}

	sf::Vector2u imageSize = grayImage.getSize();
	for (unsigned int x = 0; x < imageSize.x; ++x) 
	{
		for (unsigned int y = 0; y < imageSize.y; ++y) 
		{
			sf::Color pixelColor = grayImage.getPixel(x, y);
			int grayValue = pixelColor.r; // Assuming grayscale is stored in the red channel
			grayValue /= 28;
			sf::Color newColor = paletteLine[grayValue];
			grayImage.setPixel(x, y, newColor);
		}
	}

	sf::Texture resultTexture;
	resultTexture.loadFromImage(grayImage);

	// Display the result
	sf::Sprite resultSprite(resultTexture);
	return resultSprite;
}

float Utils::DotProduct(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

sf::Vector2f Utils::GetNormal(const sf::Vector2f& v)
{
	return sf::Vector2f(-v.y, v.x);
}

sf::Vector2f Utils::ProjectOnAxis(const RectVertexArray& vertices, const sf::Vector2f& axis)
{
	float min = std::numeric_limits<float>::infinity();
	float max = -std::numeric_limits<float>::infinity();
	for (auto& vertex : vertices)
	{
		float projection = DotProduct(vertex, axis);
		if (projection < min)
			min = projection;
		if (projection > max)
			max = projection;
	}
	return sf::Vector2f(min, max);
}

bool Utils::AreOverlapping(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return a.x <= b.y && a.y >= b.x;
}

float Utils::GetOverlapLength(const sf::Vector2f& a, const sf::Vector2f& b)
{
	if (!AreOverlapping(a, b)) return 0.f;
	return std::min(a.y, b.y) - std::max(a.x, b.x);
}

sf::Vector2f Utils::GetCenter(const sf::RectangleShape& shape)
{
	const sf::Transform& transform = shape.getTransform();
	sf::FloatRect local = shape.getLocalBounds();
	return transform.transformPoint(local.width / 2.f , local.height / 2.f);
}

RectVertexArray Utils::GetVertices(const sf::RectangleShape& shape)
{
	RectVertexArray vertices;
	const sf::Transform& transform = shape.getTransform();
	for (std::size_t i = 0u; i < shape.getPointCount(); ++i)
	{
		vertices[i] = transform.transformPoint(shape.getPoint(i));
	}
	return vertices;
}

sf::Vector2f Utils::GetPerpendicularAxis(const RectVertexArray& vertices, std::size_t index)
{
	assert(index >= 0 && index < 4);
	return GetNormal(Normalize(vertices[index + 1] - vertices[index]));
}

std::array<sf::Vector2f, 4> Utils::GetPerpendicularAxes(const RectVertexArray& vertices1, const RectVertexArray& vertices2)
{
	std::array<sf::Vector2f, 4> axes;

	axes[0] = GetPerpendicularAxis(vertices1, 0);
	axes[1] = GetPerpendicularAxis(vertices1, 1);

	axes[2] = GetPerpendicularAxis(vertices2, 0);
	axes[3] = GetPerpendicularAxis(vertices2, 1);
	return axes;
}

//각도를 넘겨서 x, y좌표 계산
sf::Vector2f Utils::Direction(const float angle)
{
	//PI는 원의 지름 / 원의 둘레 == 반지름 / (원의 둘레/2) 
	//각도 단위는 반지름이 1인 원의 라디안
	float radians = angle * (M_PI / 180.f);

	// x와 y 좌표 계산
	float x = cos(radians);
	float y = sin(radians);

	return sf::Vector2f(x, y);
}

//sf::Vector2f Utils::ProjectOnAxis(const RectVertexArray& vertices, const sf::Vector2f& axis)
//{
//	float min = std::numeric_limits<float>::infinity();
//	float max = -std::numeric_limits<float>::infinity();
//	//for (auto& vertex : vertices) {
//		//float projection = dotProduct(vertex, axis);
//		if (projection < min) { min = projection; }
//		if (projection > max) { max = projection; }
//	}
//	return sf::Vector2f(min, max);
//}
