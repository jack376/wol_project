#pragma once
#include "GameObject.h"

#define ROOM_SIZE 64

struct Room
{
    int leftOne, topOne, widthOne, heightOne;
    int leftTwo, topTwo, widthTwo, heightTwo;
};

class RoomBSP : public GameObject
{
protected:
    int tileSize = ROOM_SIZE;
    int room[ROOM_SIZE][ROOM_SIZE];

    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));

public:
    RoomBSP(const std::string& n = "") : GameObject(n) {}
    virtual ~RoomBSP() { Release(); }

    virtual void Init() {}
    virtual void Release() {}

    virtual void Reset();
    virtual void Update(float dt);
    virtual void Draw(sf::RenderWindow& window);

    Room DivideRoom(int depth, int left, int top, int width, int height);
};

