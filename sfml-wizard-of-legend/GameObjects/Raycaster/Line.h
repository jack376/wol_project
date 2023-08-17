#pragma once

class Line
{
private:
    void init();
    void calculateEndPoints();
    float m_x1, m_y1, m_x2, m_y2, m_thickness;
    float m_angle;
    sf::RectangleShape m_rect;
public:
    Line(int x1, int y1, int x2, int y2, int thickness);
    Line(int x1, int y1, float angle, int thickness);
    ~Line();
    void draw(sf::RenderWindow &window) const;
    void move(int x, int y);
    void Rotation(float angle);
    int getX1() const { return m_x1; }
    int getY1() const { return m_y1; }
    int getX2() const { return m_x2; }
    int getY2() const { return m_y2; }
    void setLength(float len);
    void setEndPoint(int x, int y);
    sf::Vector2f getEndPoint() { return { m_x2, m_y2 }; }

    float GetLength();
};