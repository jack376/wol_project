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
	// 방향
	const int dx1[4] = { 0, 0, 1, -1 };
	const int dy1[4] = { -1, 1, 0, 0 };

	using Pair = std::pair<int, int>;		// 좌표
	using pPair = std::pair<double, Pair>;	// 가중치

	struct Cell {
		int parent_x, parent_y;
		double f, g, h;
	};

	//char zmap[MAX][MAX];
	const int ROW = 32, COL = 32;

	//주어진 현재 셀의 좌표가 목적지 셀의 좌표와 일치하는지 확인하여 결과를 반환
	bool isDestination(int row, int col, Pair dst);
	//주어진 행과 열 인덱스가 그리드의 유효한 범위 내에 있는지 확인하여 결과를 반환
	bool isInRange(int row, int col);

	//주어진 행과 열 인덱스에 해당하는 맵의 값이 0인지 (통행 가능) 확인하여 결과를 반환
	bool isUnBlocked(std::vector<std::vector<int>>& map, int row, int col);

	//현재 셀과 목적지 셀 사이의 유클리드 거리를 계산하여 휴리스틱 값으로 반환
	double GethValue(int row, int col, Pair dst);

	//목적지에서 출발지까지의 경로를 cellDetails 정보를 사용하여 추적하고, 해당 경로를 data 맵 데이터에 표시
	std::stack<Pair> tracePath(Cell cellDetails[MAX][MAX], Pair dst, std::vector<std::vector<int>>& data);

	//주어진 그리드 맵에서 출발지에서 목적지까지의 최단 경로를 찾는 A* 알고리즘을 구현
	std::pair<bool, std::stack<Pair>> aStarSearch(std::vector<std::vector<int>>& map, Pair src, Pair dst);
};

#define _AS (AS::Instance())
