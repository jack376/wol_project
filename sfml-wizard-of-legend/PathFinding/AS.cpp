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
	//(double)std::sqrt(std::pow(row - dst.first, 2) + std::pow(col - dst.second, 2));
	return std::abs(dst.first - row) + std::abs(dst.second - col);
}

std::stack<AS::Pair> AS::tracePath(Cell cellDetails[MAX][MAX], Pair dst, std::vector<std::vector<int>>& data) {
	std::stack<Pair> path;
	int y = dst.first;
	int x = dst.second;

	path.push({ y, x });
	// cellDetails의 x, y의 부모좌표가 모두 현재좌표와 동일할때까지 반복
	while (!(cellDetails[y][x].parent_x == x && cellDetails[y][x].parent_y == y)) {
		int tempy = cellDetails[y][x].parent_y;
		int tempx = cellDetails[y][x].parent_x;
		y = tempy;
		x = tempx;
		path.push({ y, x });
	}


	return path;
;
}

std::pair<bool, std::stack<AS::Pair>> AS::aStarSearch(std::vector<std::vector<int>>& map, Pair src, Pair dst) {

	std::stack<AS::Pair> path;

	if (!isInRange(src.first, src.second) || !isInRange(dst.first, dst.second)) return std::pair<bool, std::stack<AS::Pair>>(false, path);
	if (!isUnBlocked(map, src.first, src.second) || !isUnBlocked(map, dst.first, dst.second)) return std::pair<bool, std::stack<AS::Pair>>(false, path);
	if (isDestination(src.first, src.second, dst)) return std::pair<bool, std::stack<AS::Pair>>(false, path);

	bool closedList[MAX][MAX];	// 방문 노드 리스트
	std::memset(closedList, false, sizeof(closedList));

	Cell cellDetails[MAX][MAX];
	// 내용초기화
	// 계산해야할 값부분은 INF로하고, 계산할 경로는 -1로 초기화
	for (int i = 0; i < ROW; ++i) {
		for (int j = 0; j < COL; ++j) {
			cellDetails[i][j].f = cellDetails[i][j].g = cellDetails[i][j].h = INF;
			cellDetails[i][j].parent_x = cellDetails[i][j].parent_y = -1;
		}
	}

	// 출발지 초기화
	int sy = src.first;
	int sx = src.second;
	cellDetails[sy][sx].f = cellDetails[sy][sx].g = cellDetails[sy][sx].h = 0.0;
	cellDetails[sy][sx].parent_x = sx;
	cellDetails[sy][sx].parent_y = sy;

	std::set<pPair> openList;	// 균형이진탐색트리로 구현되어 있기 때문에 우선순위 큐 역할을 함.
	openList.insert({ 0.0, { sy, sx } });

	while (!openList.empty()) {

	    const pPair& p = *openList.begin();
		openList.erase(openList.begin());

		int y = p.second.first;
		int x = p.second.second;
		closedList[y][x] = true;

		double ng, nf, nh;

		// 인접 노드 탐색
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
				else if (!closedList[ny][nx] && isUnBlocked(map, ny, nx)) {

					ng = cellDetails[y][x].g + 1.0;
					nh = GethValue(ny, nx, dst);
					nf = ng + nh;

					if (cellDetails[ny][nx].f > nf) {
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