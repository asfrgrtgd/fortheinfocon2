# pragma once
#include "stdafx.h"

class weightedEdgeGraphSearchDef
{
protected:
	const static int verticesNum = 10;

	std::vector<int> dist;
	std::vector<std::vector<int>>dist2x2;
	std::vector<std::vector<int>> edges;

	void setSample(int num) {
		resetEdges();
		if (num == 0) {
			edges[0][1] = 2;
			edges[0][2] = 4;
			edges[1][3] = 3;
			edges[2][4] = 2;
			edges[3][5] = 4;
			edges[4][5] = 1;
			edges[5][6] = 3;
			edges[4][7] = 5;
			edges[6][8] = 2;
			edges[7][8] = 1;
			edges[8][9] = 3;
			edges[3][7] = 2;
			edges[1][4] = 6;
		}
		else if (num == 1) {
			edges[0][1] = 1;
			edges[0][3] = 5;
			edges[1][2] = 2;
			edges[1][4] = 2;
			edges[2][5] = 3;
			edges[3][4] = 3;
			edges[4][5] = 1;
			edges[4][6] = 4;
			edges[5][7] = 2;
			edges[6][7] = 1;
			edges[7][8] = 2;
			edges[8][9] = 4;
			edges[5][9] = 7;
		}
		else if (num == 2) {
			edges[0][2] = 3;
			edges[0][3] = 2;
			edges[2][4] = 4;
			edges[3][4] = 1;
			edges[3][5] = 5;
			edges[4][6] = 2;
			edges[5][6] = 2;
			edges[6][7] = 3;
			edges[7][8] = 2;
			edges[8][9] = 1;
			edges[5][9] = 6;
			edges[4][7] = 3;
		}
		else if (num == 3) {
			edges[0][1] = 4;
			edges[0][2] = 1;
			edges[1][3] = 5;
			edges[2][3] = 2;
			edges[2][4] = 7;
			edges[3][5] = 3;
			edges[4][5] = 1;
			edges[5][6] = 4;
			edges[6][7] = 2;
			edges[7][9] = 3;
			edges[5][8] = 2;
			edges[8][9] = 2;
		}
		else if (num == 4) {
			edges[0][1] = 3;
			edges[0][4] = 2;
			edges[1][2] = 4;
			edges[1][5] = 6;
			edges[2][3] = 2;
			edges[3][5] = 1;
			edges[4][5] = 3;
			edges[4][7] = 5;
			edges[5][6] = 2;
			edges[6][8] = 3;
			edges[7][8] = 2;
			edges[8][9] = 4;
			edges[5][9] = 5;
		}
	}

	void resetEdges() {
		edges = std::vector<std::vector<int>>(verticesNum, std::vector<int>(verticesNum, 10000));
	}
};
