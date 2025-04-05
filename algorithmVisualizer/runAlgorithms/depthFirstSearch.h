# pragma once
# include "algorithmVisualizerDefs.h"
# include "algorithmIntroduction.h"
# include "explainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;
class depthFirstSearch : public AppScene::Scene, public mazeSearchDef {
private:
	const RoundRect detailButton = RoundRect(1480, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect stepButton = RoundRect(1380, 25, algo_vis::STEP_BUTTON_SIZE, 5);
	const Font mazeSquareFont{ 40 };
	const Font detailBackButtonFont{ 40,Typeface::Bold };
	const Font gridExampleFont{ 34,Typeface::Bold };
	const Font stepButtonFont{ 45,Typeface::Bold };
	const RoundRect algorithmDetailBox1 = RoundRect(150, 600, 1300, 250, 10);
	const Rect algorithmDetailBox2 = Rect(160, 610, 1280, 230);
	const Font algorithmDetailBoxFont{ 30 };
	const static int maze_y = 6, maze_x = 6;
	const std::vector<String>gridExampleStr = { U"空間",U"壁",U"スタート地点",U"ゴール地点" };
	const int dx[4] = { 1,0,-1,0 }, dy[4] = { 0,1,0,-1 };
	std::vector<std::vector<Rect>>mazeSquares;
	std::vector<Rect>sampleSquares;
	std::vector<std::pair<int, int>>searchCandidate;
	std::stack<std::pair<int, int>>sta;
	std::stack<std::pair<int, int>>staWillAdd;
	std::vector<Rect>searchCandidateSquares;
	String detailStr = U"";
	const String detailStr1Key = U"depthFirstSearch.detailStr1";
	const String detailStr2Key = U"depthFirstSearch.detailStr2";
	bool flag1 = true;
	bool flag2 = false;
	bool flagEnd = false;
	int nowStep = 0;
	int sx = -1, sy = -1, gx = -1, gy = -1;
	TextEditState textBoxStr{ U"5" };
public:
	depthFirstSearch(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"depthFirstSearch.str");
		detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStr1Key);
		for (int i = 0; i < 4; i++) {
			sampleSquares.push_back({ 10,140 + i * 110,90,90 });
		}
		for (int i = 0; i < maze_y; i++) {
			std::vector<Rect>tmp, tmpShadow;
			for (int j = 0; j < maze_x; j++) {
				int x = 460 + j * 85;
				int y = 60 + i * 85;
				tmp.push_back({ x,y,90,90 });
			}
			mazeSquares.push_back(tmp);
		}
		for (int i = 0; i < maze_y; i++) {
			for (int j = 0; j < maze_x; j++) {
				dist[i][j] = -1;
			}
		}
		for (int i = 0; i < 10; i++) {
			searchCandidate.push_back({ -1,-1 });
			searchCandidateSquares.push_back({ 1100,70 + i * 50,180,53 });
		}
		setSample();
	}
	void update() override {

		if (flag1) {
			if (SimpleGUI::Button(U"手動生成", Vec2{ 10, 30 }, 150)) {
				flag2 = true;
				for (int i = 0; i < maze_y; i++) {
					for (int j = 0; j < maze_x; j++) {
						maze[i][j] = 0;
					}
				}
				detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStr2Key);
			}
		}
		else {
			(void)SimpleGUI::Button(U"手動生成", Vec2{ 10, 30 }, 150, false);
		}

		if (flag1) {
			if (SimpleGUI::Button(U"サンプル", Vec2{ 10, 90 }, 150)) {
				setSample();
			}
		}
		else {
			(void)SimpleGUI::Button(U"サンプル", Vec2{ 10, 90 }, 150, false);
		}

		if (stepButton.leftClicked() && (!flagEnd)) {
			if (nowStep == 0) {
				flag1 = false;
				flag2 = false;
				if (!checkMaze()) {
					setSample();
					checkMaze();
				}
			}
			algo();
		}

		if (backButton.leftClicked()) {
			changeScene(U"selectAndExplainAlgorithm", 500);
		}

		if (detailButton.leftClicked()) {
			changeScene(U"explainAlgorithm", 500);
		}

		if (flag2) {
			for (int i = 0; i < maze_y; i++) {
				for (int j = 0; j < maze_x; j++) {
					if (mazeSquares[i][j].rightClicked()) {
						maze[i][j] = 0;
					}
					if (mazeSquares[i][j].leftClicked()) {
						maze[i][j] = 1;
					}
					if (KeyS.down()) {
						if (mazeSquares[i][j].mouseOver()) {
							maze[i][j] = 2;
						}
					}
					if (KeyG.down()) {
						if (mazeSquares[i][j].mouseOver()) {
							maze[i][j] = 3;
						}
					}
				}
			}
		}

	}
	void draw() const override {
		algorithmDetailBox1.draw(Palette::White);

		backButton.draw(Palette::Dodgerblue);
		detailBackButtonFont(U"戻る").draw(Arg::center(backButton.x + 50, backButton.y + 35), ColorF{ 0.1 });

		detailButton.draw(Palette::Orange);
		detailBackButtonFont(U"詳細").draw(Arg::center(detailButton.x + 50, detailButton.y + 35), ColorF{ 0.1 });

		stepButton.draw(Palette::Bisque);
		stepButtonFont(U"ステップ").draw(Arg::center(stepButton.x + 100, stepButton.y + 45), Palette::Black);

		for (int i = 0; i < maze_y; i++) {
			for (int j = 0; j < maze_x; j++) {
				if (maze[i][j] == 0) {
					mazeSquares[i][j].draw(Palette::White);
				}
				if (maze[i][j] == 1) {
					mazeSquares[i][j].draw(Palette::Dimgray);
				}
				if (maze[i][j] == 2) {
					mazeSquares[i][j].draw(Palette::Cyan);
				}
				if (maze[i][j] == 3) {
					mazeSquares[i][j].draw(Palette::Crimson);
				}
				mazeSquares[i][j].drawFrame(5, 0, Palette::Black);
			}
		}

		for (int i = 0; i < maze_y; i++) {
			for (int j = 0; j < maze_x; j++) {
				if (dist[i][j] != -1) {
					mazeSquareFont(dist[i][j]).drawAt(mazeSquares[i][j].center(), Palette::Black);
				}
			}
		}

		for (int i = 0; i < 4; i++) {
			if (i == 0) {
				sampleSquares[i].draw(Palette::White);
			}
			if (i == 1) {
				sampleSquares[i].draw(Palette::Dimgray);
			}
			if (i == 2) {
				sampleSquares[i].draw(Palette::Cyan);
			}
			if (i == 3) {
				sampleSquares[i].draw(Palette::Crimson);
			}
			sampleSquares[i].drawFrame(5, 0, Palette::Black);
			gridExampleFont(gridExampleStr[i]).draw(sampleSquares[i].x + 100, sampleSquares[i].y + 45, Palette::Black);
		}

		for (int i = 0; i < 10; i++) {
			if (i == 0) {
				searchCandidateSquares[i].draw(Palette::Gold);
			}
			else {
				searchCandidateSquares[i].draw(Palette::White);
			}
			searchCandidateSquares[i].drawFrame(3, 0, Palette::Black);
			if (searchCandidate[i].first != -1 && searchCandidate[i].second != -1) {
				mazeSquareFont(U"(" + Format(searchCandidate[i].first + 1) + U"," + Format(searchCandidate[i].second + 1) + U")").drawAt(searchCandidateSquares[i].center(), Palette::Black);
			}
		}

		for (int i = 0; i < 6; i++) {
			gridExampleFont(i + 1).draw(mazeSquares[i][0].x - 35, mazeSquares[i][0].y + 20, Palette::Black);
		}

		for (int i = 0; i < 6; i++) {
			gridExampleFont(i + 1).draw(mazeSquares[0][i].x + 30, mazeSquares[0][i].y - 50, Palette::Black);
		}

		gridExampleFont(U"探索候補").draw(searchCandidateSquares[0].x + 18, searchCandidateSquares[0].y - 50, Palette::Black);
		algorithmDetailBoxFont(detailStr).draw(algorithmDetailBox2, Palette::Black);
	}
	void algo() {
		String ret;
		if (nowStep == 0) {
			ret += U"スタート地点である(" + Format(sx + 1) + U"," + Format(sy + 1) + U")を探索候補としてスタックに入れ、スタート地点からの距離を0とします";
			dist[sy][sx] = 0;
			sta.push({ sx,sy });
			updateSearchCandidate();
			nowStep++;
		}
		else if (sta.empty()) {
			ret += U"全てのマス目を探索しましたが、ゴール地点に到達出来なかったので探索を終了します";
			flagEnd = true;
		}
		else if (nowStep % 2 == 1) {
			int nowX = sta.top().first, nowY = sta.top().second;
			bool end = false;
			ret += U"スタックの一番上の要素は(" + Format(nowX + 1) + U"," + Format(nowY + 1) + U")なのでこのマスについて近傍を調べます、このマスから見て未探索な近傍のマスは";
			int tmp = 0;
			for (int i = 0; i < 4; i++) {
				int nextX = nowX + dx[i], nextY = nowY + dy[i];
				if (isOk(nextX, nextY)) {
					if (maze[nextY][nextX] != 1)tmp++;
				}
			}
			if (tmp == 0) {
				ret += U"存在しないので距離の更新も、スタックに対する要素の追加もありません";
			}
			else {
				for (int i = 0; i < 4; i++) {
					int nextX = nowX + dx[i], nextY = nowY + dy[i];
					if (isOk(nextX, nextY)) {
						if (maze[nextY][nextX] != 1) {
							ret += U"(" + Format(nextX + 1) + U"," + Format(nextY + 1) + U") ";
						}
					}
				}
				ret.pop_back();
				ret += U"です、これらのマスは元のマスから一マス移動する事でたどり着けるので、距離は元の(" + Format(nowX + 1) + U", " + Format(nowY + 1) + U")の距離に+1した値です。\n";
				for (int i = 0; i < 4; i++) {
					int nextX = nowX + dx[i], nextY = nowY + dy[i];
					if (isOk(nextX, nextY)) {
						if (maze[nextY][nextX] != 1) {
							dist[nextY][nextX] = dist[nowY][nowX] + 1;
							staWillAdd.push({ nextX,nextY });
						}
						if (maze[nextY][nextX] == 3) {
							end = true;
						}
					}
				}
				if (end) {
					ret += U"近傍のマスにゴールが含まれていたので探索を終了します。";
					flagEnd = true;
				}
			}
			nowStep++;
		}
		else {
			ret += U"一ステップ前に探索した近傍のマスをスタックに追加し、探索済みの一番上の要素を消します(近傍マスがない場合はスタックの一番上を削除するだけになります)";
			sta.pop();
			const int tmp = static_cast<int>(staWillAdd.size());
			for (int i = 0; i < tmp; i++) {
				sta.push(staWillAdd.top());
				staWillAdd.pop();
			}
			updateSearchCandidate();
			nowStep++;
		}
		detailStr = ret;
	}
	bool checkMaze() {
		int scnt = 0, gcnt = 0;
		for (int i = 0; i < maze_y; i++) {
			for (int j = 0; j < maze_x; j++) {
				if (maze[i][j] == 2) {
					scnt++, sx = j, sy = i;
				}
				if (maze[i][j] == 3) {
					gcnt++, gx = j, gy = i;
				}
			}
		}
		if (scnt == 1 && gcnt == 1) {
			return true;
		}
		else {
			return false;
		}
	}
	void updateSearchCandidate() {
		auto tmp = sta;
		int i = 0;
		std::vector<std::pair<int, int>>ret(10, { -1,-1 });
		while (!tmp.empty() && i < 10) {
			ret[i] = tmp.top();
			tmp.pop();
			i++;
		}
		searchCandidate = ret;
	}
};
