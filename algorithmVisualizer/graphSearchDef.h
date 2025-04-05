# pragma once
#include "stdafx.h"

class graphSearchDef
{
protected:
	const static int verticesNum = 10;

	std::vector<int>dist;
	std::vector<std::vector<bool>>edges;
	std::vector<std::vector<bool>>realEdges;
	void setSample(int ver) {
		resetEdges();
		if (ver == 0) {
			edges[0][1] = true;
			edges[1][0] = true;
			edges[0][2] = true;
			edges[2][0] = true;
			edges[1][7] = true;
			edges[7][1] = true;
			edges[1][3] = true;
			edges[3][1] = true;
			edges[1][4] = true;
			edges[4][1] = true;
			edges[3][7] = true;
			edges[7][3] = true;
			edges[2][5] = true;
			edges[5][2] = true;
			edges[2][6] = true;
			edges[6][2] = true;
			edges[4][7] = true;
			edges[7][4] = true;
			edges[5][8] = true;
			edges[8][5] = true;
			edges[5][9] = true;
			edges[9][5] = true;
		}
		else if (ver == 1) {
			edges[0][1] = true;
			edges[0][2] = true;
			edges[1][3] = true;
			edges[1][4] = true;
			edges[1][7] = true;
			edges[2][5] = true;
			edges[3][6] = true;
			edges[4][6] = true;
			edges[5][7] = true;
			edges[6][8] = true;
			edges[7][9] = true;
		}
		else if (ver == 2) {
			edges[1][9] = true;
			edges[1][5] = true;
			edges[1][8] = true;
			edges[4][6] = true;
			edges[2][3] = true;
			edges[9][0] = true;
			edges[5][3] = true;
			edges[5][8] = true;
			edges[7][3] = true;
			edges[7][8] = true;
			edges[6][8] = true;
		}
		else if (ver == 3) {
			edges[1][0] = true;
			edges[1][6] = true;
			edges[3][0] = true;
			edges[3][5] = true;
			edges[3][4] = true;
			edges[9][2] = true;
			edges[9][4] = true;
			edges[0][4] = true;
			edges[2][4] = true;
			edges[2][5] = true;
			edges[6][5] = true;
			edges[0][6] = true;
		}
		else if (ver == 4) {
			edges[6][2] = true;
			edges[6][3] = true;
			edges[6][1] = true;
			edges[2][4] = true;
			edges[9][8] = true;
			edges[9][5] = true;
			edges[9][1] = true;
			edges[8][7] = true;
			edges[8][0] = true;
			edges[5][7] = true;
			edges[7][1] = true;
			edges[3][4] = true;
		}
		else if (ver == 5) {
			edges[9][5] = true;
			edges[9][1] = true;
			edges[9][0] = true;
			edges[4][6] = true;
			edges[4][2] = true;
			edges[6][3] = true;
			edges[6][2] = true;
			edges[5][1] = true;
			edges[5][2] = true;
			edges[8][0] = true;
			edges[8][7] = true;
			edges[1][0] = true;
			edges[0][2] = true;
			edges[3][2] = true;
		}
		else if (ver == 6) {
			edges[3][6] = true;
			edges[2][6] = true;
			edges[2][1] = true;
			edges[2][0] = true;
			edges[7][8] = true;
			edges[7][0] = true;
			edges[6][1] = true;
			edges[9][1] = true;
			edges[1][5] = true;
		}
		else if (ver == 7) {
			edges[3][6] = true;
			edges[3][0] = true;
			edges[8][1] = true;
			edges[8][9] = true;
			edges[0][2] = true;
			edges[0][4] = true;
			edges[1][7] = true;
			edges[1][9] = true;
			edges[1][4] = true;
			edges[2][7] = true;
			edges[2][5] = true;
			edges[5][4] = true;
		}
		realEdges = edges;
	}

	void resetEdges() {
		edges = std::vector<std::vector<bool>>(verticesNum, std::vector<bool>(verticesNum, false));
		realEdges = std::vector<std::vector<bool>>(verticesNum, std::vector<bool>(verticesNum, false));
	}
};
