#pragma once
#include "stdafx.h"
#include "Singleton.h"
#include "Tile.h"

constexpr int MAX = 32;
constexpr double INF = 1e9 + 7;

class AS : public Singleton<AS>
{
	friend Singleton<AS>;

public:
	// ����
	const int dx1[4] = { 0, 0, 1, -1 };
	const int dy1[4] = { -1, 1, 0, 0 };

	using Pair = std::pair<int, int>;		// ��ǥ
	using pPair = std::pair<double, Pair>;	// ����ġ

	struct Cell {
		int parent_x, parent_y;
		double f, g, h;
	};

	//char zmap[MAX][MAX];
	const int ROW = 32, COL = 32;

	//�־��� ���� ���� ��ǥ�� ������ ���� ��ǥ�� ��ġ�ϴ��� Ȯ���Ͽ� ����� ��ȯ
	bool isDestination(int row, int col, Pair dst);
	//�־��� ��� �� �ε����� �׸����� ��ȿ�� ���� ���� �ִ��� Ȯ���Ͽ� ����� ��ȯ
	bool isInRange(int row, int col);

	//�־��� ��� �� �ε����� �ش��ϴ� ���� ���� 0���� (���� ����) Ȯ���Ͽ� ����� ��ȯ
	bool isUnBlocked(std::vector<std::vector<int>>& map, int row, int col);

	//���� ���� ������ �� ������ ��Ŭ���� �Ÿ��� ����Ͽ� �޸���ƽ ������ ��ȯ
	double GethValue(int row, int col, Pair dst);

	//���������� ����������� ��θ� cellDetails ������ ����Ͽ� �����ϰ�, �ش� ��θ� data �� �����Ϳ� ǥ��
	std::stack<Pair> tracePath(Cell cellDetails[MAX][MAX], Pair dst, std::vector<std::vector<int>>& data);

	//�־��� �׸��� �ʿ��� ��������� ������������ �ִ� ��θ� ã�� A* �˰����� ����
	std::pair<bool, std::stack<Pair>> aStarSearch(std::vector<std::vector<int>>& map, Pair src, Pair dst);
};

#define _AS (AS::Instance())
