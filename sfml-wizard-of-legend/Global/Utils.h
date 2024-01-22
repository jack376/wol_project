#pragma once

class Utils
{
private:
	static std::random_device rd;
	static std::mt19937 gen;

public:
	static int RandomRange(int min, int maxExclude);
	static float RandomRange(float min, float max);
	static sf::Vector2f RandomOnCircle(float radius);
	static sf::Vector2f RandomInCircle(float radius);
	static float RandomValue();

	static void SetOrigin(sf::Sprite& sprite, Origins origin);
	static void SetOrigin(sf::Text& text, Origins origin);
	static void SetOrigin(sf::Shape& shape, Origins origin);
	static void SetOrigin(sf::Transformable& obj, Origins origin, const sf::FloatRect& rect);

	static float Clamp(float v, float min, float max);
	static sf::Vector2f Clamp(const sf::Vector2f& v, const sf::Vector2f& min, const sf::Vector2f& max);
	static float Lerp(float a, float b, float t, bool clamping = true);
	static sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t, bool clamping = true);
	static sf::Color Lerp(const sf::Color& a, const sf::Color& b, float t, bool clamping = true);

	static const sf::Vector2f Normalize(const sf::Vector2f& vector);
	static float Magnitude(const sf::Vector2f& vector2);		// getLength(); 벡터의 길이
	static float SqrMagnitude(const sf::Vector2f& vector);
	static float Distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

	static float Angle(const sf::Vector2f& start, const sf::Vector2f& end);
	static float Angle(const sf::Vector2f& dir);
	static sf::Vector2f Angle(float angle);

	// 팔레트 함수
	// 쉐이더 적용 방법 window.draw(sprite, &shader);
	static void SetShader(sf::Shader& shader, sf::Sprite& sprite, sf::Texture& palette);

	// 픽셀 적용
	static sf::Sprite& SetPixelColor(sf::Image& grayImage, sf::Image& paletteImage);

	// OBB 함수
	// 두 백터 내적 계산
	static float DotProduct(const sf::Vector2f& a, const sf::Vector2f& b);
	// 주어진 벡터의 오른쪽 방향 수직 벡터 반환
	static sf::Vector2f GetNormal(const sf::Vector2f& v);

	// 축 정의 및 관련 함수
	// OBB의 꼭지점을 주어진 축에 투영후, 투영된 범위의 최소 최대값 반환
	static float ProjectOnAxis(const RectVertexArray& vertices, const sf::Vector2f& axis);
	// 두 범위 충돌검사
	static bool AreOverlapping(const float a, const float b);
	// 충돌범위 길이 계산
	//static float GetOverlapLength(const sf::Vector2f& a, const sf::Vector2f& b);

	// OBB 관련 함수
	// OBB의 중심점 반환
	static sf::Vector2f GetCenter(const sf::RectangleShape& shape);
	// OBB의 꼭지점들 반환
	static RectVertexArray GetVertices(const sf::RectangleShape& shape);
	// OBB의 인덱스 기반으로 해당 면의 수직 축을 계산해 반환
	static sf::Vector2f GetPerpendicularAxis(const RectVertexArray& vertices, std::size_t index);
	// 두 개의 OBB에 대한 수직 축 목록을 계산하여 반환
	static std::array<sf::Vector2f, 4> GetPerpendicularAxes(const RectVertexArray& vertices1, const RectVertexArray& vertices2);

	static sf::Vector2f Direction(const float angle);
};

