# pragma once
#include "stdafx.h"

class mazeSearchDef
{
	protected:
	const static int maze_y = 6, maze_x = 6;

	std::array<std::array<int, maze_x>, maze_y> dist{};
	std::array<std::array<int, maze_x>, maze_y> maze{};
	// int dist[maze_y][maze_x], maze[maze_y][maze_x];//maze 0-空間 1-壁 2-スタート 3-ゴール
	int mazeSample[maze_y][maze_x] = { {2, 0, 0, 0, 1, 1},{1, 1, 0, 1, 0, 1},{1, 1, 0, 1, 0, 1},{1, 0, 0, 0, 0, 0},{1, 0, 1, 1, 0, 1},{1, 3, 1, 1, 1, 1} };

	void setSample() {
		for (int i = 0; i < maze_y; i++) {
			for (int j = 0; j < maze_x; j++) {
				maze[i][j] = mazeSample[i][j];
			}
		}
	}
	bool isOk(int x, int y) {
		bool ret = false;
		if (0 <= x && x < maze_x && 0 <= y && y < maze_y) {
			if (dist[y][x] == -1)ret = true;
		}
		return ret;
	}

};
