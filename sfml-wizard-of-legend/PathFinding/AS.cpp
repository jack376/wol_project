#include "stdafx.h"
#include "AS.h"


bool AS::isDestination(int row, int col, Pair dst) {
	return (row == dst.first && col == dst.second);
}

bool AS::isInRange(int row, int col) {
	return (row >= 0 && row < ROW && col >= 0 && col < COL);
}

bool AS::isUnBlocked(std::vector<std::vector<int>>& map, int row, int col) {
	return (map[row][col] == 0);
}

double AS::GethValue(int row, int col, Pair dst) {
	return (double)std::sqrt(std::pow(row - dst.first, 2) + std::pow(col - dst.second, 2));
}

std::stack<AS::Pair> AS::tracePath(Cell cellDetails[MAX][MAX], Pair dst, std::vector<std::vector<int>>& data) {
	std::stack<Pair> s;
	int y = dst.first;
	int x = dst.second;

	s.push({ y, x });
	// cellDetails�� x, y�� �θ���ǥ�� ��� ������ǥ�� �����Ҷ����� �ݺ�
	while (!(cellDetails[y][x].parent_x == x && cellDetails[y][x].parent_y == y)) {
		int tempy = cellDetails[y][x].parent_y;
		int tempx = cellDetails[y][x].parent_x;
		y = tempy;
		x = tempx;
		s.push({ y, x });
	}
	
	std::stack<Pair> path = s;

	while (!s.empty()) {
		data[s.top().first][s.top().second] = 9;
		s.pop();
	}

	return path;
}

std::pair<bool, std::stack<AS::Pair>> AS::aStarSearch(std::vector<std::vector<int>>& map, Pair src, Pair dst) {

	std::stack<AS::Pair> path;

	if (!isInRange(src.first, src.second) || !isInRange(dst.first, dst.second)) return std::pair<bool, std::stack<AS::Pair>>(false, path);
	if (!isUnBlocked(map, src.first, src.second) || !isUnBlocked(map, dst.first, dst.second)) return std::pair<bool, std::stack<AS::Pair>>(false, path);
	if (isDestination(src.first, src.second, dst)) return std::pair<bool, std::stack<AS::Pair>>(false, path);

	bool closedList[MAX][MAX];
	std::memset(closedList, false, sizeof(closedList));

	Cell cellDetails[MAX][MAX];

	// �����ʱ�ȭ
	// �ִ������˰���� ����
	// ����ؾ��� ���κ��� INF���ϰ�, ����� ��δ� -1�� �ʱ�ȭ
	for (int i = 0; i < ROW; ++i) {
		for (int j = 0; j < COL; ++j) {
			cellDetails[i][j].f = cellDetails[i][j].g = cellDetails[i][j].h = INF;
			cellDetails[i][j].parent_x = cellDetails[i][j].parent_y = -1;
		}
	}

	// src�� ��ǥ�� ù��ǥ�� �ȴ�.
	int sy = src.first;
	int sx = src.second;
	cellDetails[sy][sx].f = cellDetails[sy][sx].g = cellDetails[sy][sx].h = 0.0;
	cellDetails[sy][sx].parent_x = sx;
	cellDetails[sy][sx].parent_y = sy;

	std::set<pPair> openList;
	openList.insert({ 0.0, { sy, sx } });

	// �� �ݺ����� bfs�� ��û �Ȱ����ϴ�.
	while (!openList.empty()) {
		pPair p = *openList.begin();
		openList.erase(openList.begin());

		int y = p.second.first;
		int x = p.second.second;
		closedList[y][x] = true;

		double ng, nf, nh;

		// ����
		for (int i = 0; i < 4; ++i) {
			int ny = y + dy1[i];
			int nx = x + dx1[i];

			if (isInRange(ny, nx)) {
				if (isDestination(ny, nx, dst)) {
					cellDetails[ny][nx].parent_y = y;
					cellDetails[ny][nx].parent_x = x;
					path = tracePath(cellDetails, dst, map);
					return std::pair<bool, std::stack<AS::Pair>>(true, path);
				}

				// bfs�� ���� �����ڸ�, closedList�� �湮���ζ�� �����Ͻø� �˴ϴ�.
				else if (!closedList[ny][nx] && isUnBlocked(map, ny, nx)) {
					// �̺κ� y x, ny nx �򰥸��°� ����
					ng = cellDetails[y][x].g + 1.0;
					nh = GethValue(ny, nx, dst);
					nf = ng + nh;

					// ���� �ѹ��� ������ �ȵ�f�ų�, ���ΰ��ŵ� f�� ����f���� ������ ��
					if (cellDetails[ny][nx].f == INF || cellDetails[ny][nx].f > nf) {
						cellDetails[ny][nx].f = nf;
						cellDetails[ny][nx].g = ng;
						cellDetails[ny][nx].h = nh;
						cellDetails[ny][nx].parent_x = x;
						cellDetails[ny][nx].parent_y = y;
						openList.insert({ nf, { ny, nx } });
					}
				}
			}
		}
	}

	return std::pair<bool, std::stack<AS::Pair>>(false, path);;
}