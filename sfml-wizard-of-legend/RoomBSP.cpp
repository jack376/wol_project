#include "stdafx.h"
#include "RoomBSP.h"

void RoomBSP::Reset()
{
	shape.setOutlineThickness(2.0f);
	shape.setOutlineColor(sf::Color(255, 255, 255, 32));
}

void RoomBSP::Update(float dt)
{
}

void RoomBSP::Draw(sf::RenderWindow& window)
{
	/*
	for (int i = 0; i < tileSize; i++)
	{
		for (int j = 0; j < tileSize; j++)
		{
			switch (room[i][j])
			{
			case 0: // Not draw
				continue;
			case 1: // Room
				shape.setFillColor(sf::Color::Green);
				break;
			case 3: // Horizontal Door
			case 4: // Vertical Door
				shape.setFillColor(sf::Color(0, 255, 0, 96));
				break;
			default: // Default Color
				shape.setFillColor(sf::Color::Black); 
				break;
			}
			shape.setPosition(i * tileSize, j * tileSize);
			window.draw(shape);
		}
	}
	*/
}

Room RoomBSP::DivideRoom(int depth, int left, int top, int width, int height)
{
    Room location;

	int size = 7;
	int gap = 1;

	if (depth == 0 || (width - left <= size || height - top <= size))
	{
		for (int i = left + gap; i < width - gap; i++)
		{
			for (int j = top + gap; j < height - gap; j++)
			{
				room[i][j] = 1;
			}
		}
		return { left + 2, top + 2, width - 3, height - 3, left + 2, top + 2, width - 3, height - 3 };
	}

	int rowLength = width - left;
	int colLength = height - top;

	Room temp1;
	Room temp2;

	if (rowLength / colLength > 1 || (colLength / rowLength <= 1 && rand() % 2))
	{ 
		int divideNumber = (width - left) * Utils::RandomRange(0.4f, 0.6f);

		temp1 = DivideRoom(depth - 1, left, top, left + divideNumber, height);
		temp2 = DivideRoom(depth - 1, left + divideNumber, top, width, height);

		room[temp1.widthTwo + 1][(temp1.topTwo + temp1.heightTwo) / 2] = 4;
		room[temp1.widthTwo + 2][(temp1.topTwo + temp1.heightTwo) / 2] = 4;
		room[temp2.leftOne - 1][(temp2.topOne + temp2.heightOne) / 2] = 4;
		room[temp2.leftOne - 2][(temp2.topOne + temp2.heightOne) / 2] = 4;

		int rmin = std::min((temp1.topTwo + temp1.heightTwo) / 2, (temp2.topOne + temp2.heightOne) / 2);
		int rmax = std::max((temp1.topTwo + temp1.heightTwo) / 2, (temp2.topOne + temp2.heightOne) / 2);
		
		for (int i = rmin; i <= rmax; i++) 
		{
			room[temp2.leftOne - 2][i] = 4;
		}
	}
	else 
	{
		int divideNumber = (height - top) * Utils::RandomRange(0.4f, 0.6f);

		temp1 = DivideRoom(depth - 1, left, top, width, top + divideNumber);
		temp2 = DivideRoom(depth - 1, left, top + +divideNumber, width, height);

		room[(temp1.leftTwo + temp1.widthTwo) / 2][temp1.heightTwo + 1] = 3;
		room[(temp1.leftTwo + temp1.widthTwo) / 2][temp1.heightTwo + 2] = 3;
		room[(temp2.leftOne + temp2.widthOne) / 2][temp2.topOne - 1] = 3;
		room[(temp2.leftOne + temp2.widthOne) / 2][temp2.topOne - 2] = 3;

		int rmin = std::min((temp1.leftTwo + temp1.widthTwo) / 2, (temp2.leftOne + temp2.widthOne) / 2);
		int rmax = std::max((temp1.leftTwo + temp1.widthTwo) / 2, (temp2.leftOne + temp2.widthOne) / 2);

		for (int i = rmin; i <= rmax; i++) 
		{
			room[i][temp2.topOne - 2] = 3;
		}
	}
	location.leftOne = temp1.leftOne, location.widthOne = temp1.widthOne, location.topOne = temp1.topOne, location.heightOne = temp1.heightOne;
	location.leftTwo = temp2.leftTwo, location.widthTwo = temp2.widthTwo, location.topTwo = temp2.topTwo, location.heightTwo = temp2.heightTwo;

	return location;
}