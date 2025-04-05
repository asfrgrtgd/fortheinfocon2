# pragma once
# include "algorithmVisualizerDefs.h"
# include "algorithmIntroduction.h"
# include "explainAlgorithm.h"
# include "graphSearchDef.h"

using AppScene = SceneManager<String, algorithmIntroduction>;
class topologicalSort : public AppScene::Scene, graphSearchDef {
private:
	const RoundRect detailButton = RoundRect(1480, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect stepButton = RoundRect(1380, 25, algo_vis::STEP_BUTTON_SIZE, 5);
	const Font detailBackButtonFont{ 40,Typeface::Bold };
	const Font stepButtonFont{ 45,Typeface::Bold };
	const Font circleFont{ 40 };
	const Font exampleFont{ 34,Typeface::Bold };
	const RoundRect algorithmDetailBox1 = RoundRect(150, 600, 1300, 250, 10);
	const Rect algorithmDetailBox2 = Rect(160, 610, 1280, 230);
	const Font algorithmDetailBoxFont{ 26 };
	const Vec2 center = Vec2(610, 300);
	const double radius = 250;
	const int startVertex = 0;
	const int goalVertex = 9;
	String detailStr = U"";
	std::vector<Vec2>vertices;
	std::vector<int>searchCandidate;
	std::queue<int>que;
	std::queue<int>queWillAdd;
	std::queue<std::pair<int, int>>willChangeDist;
	std::vector<int>inDegree;
	std::vector<Rect>searchCandidateSquares;
	std::vector<Rect>distSquares;
	const String detailStr1Key = U"topologicalSort.detailStr";
	bool flag1 = true;
	bool flagEnd = false;
	int nowStep = 0;
	TextEditState textBoxStrA{ U"1" };
	TextEditState textBoxStrB{ U"2" };
public:
	topologicalSort(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"topologicalSort.str");
		detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStr1Key);
		for (int i = 0; i < verticesNum; i++) {
			double angle = (Math::TwoPi / verticesNum) * i;  // 各頂点の角度
			double x = center.x + radius * std::cos(angle);  // x座標
			double y = center.y + radius * std::sin(angle);  // y座標
			vertices.push_back(Vec2(x, y));
		}
		edges = std::vector<std::vector<bool>>(verticesNum, std::vector<bool>(verticesNum, false));
		for (int i = 0; i < verticesNum; i++) {
			dist.push_back(-1);
			inDegree.push_back(0);
			searchCandidate.push_back(-1);
			searchCandidateSquares.push_back({ 1000,70 + i * 50,100,53 });
		}
		for (int i = 0; i < verticesNum; i++) {  // 数値
			distSquares.push_back({ 1260,70 + i * 50,60,53 });
		}
		for (int i = 0; i < verticesNum; i++) {  // インデックス
			distSquares.push_back({ 1200,70 + i * 50,63,53 });
		}
		setSample(1);

	}
	void update() override {
		if (flag1) {
			if (SimpleGUI::Button(U"ランダム", Vec2{ 10, 30 }, 150)) {
				int num = Random(1, 7);
				setSample(num);
			}
		}
		else {
			(void)SimpleGUI::Button(U"ランダム", Vec2{ 10, 30 }, 150, false);
		}

		if (stepButton.leftClicked() && (!flagEnd)) {
			algo();
		}

		if (backButton.leftClicked()) {
			changeScene(U"selectAndExplainAlgorithm", 500);
		}

		if (detailButton.leftClicked()) {
			changeScene(U"explainAlgorithm", 500);
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

		// 有向グラフの描画
		for (int i = 0; i < verticesNum; i++) {
			for (int j = 0; j < verticesNum; j++) {
				if (edges[i][j] && realEdges[i][j]) {
					Vec2 direction = (vertices[j] - vertices[i]).normalized();
					Vec2 start = vertices[i] + direction * 35;
					Vec2 end = vertices[j] - direction * 35;
					Shape2D::Arrow(Line{ start, end }, 3, Vec2{ 20, 20 }).draw(Palette::Black);
				}
				else if (edges[i][j] && !realEdges[i][j]) {
					Vec2 direction = (vertices[j] - vertices[i]).normalized();
					Vec2 start = vertices[i] + direction * 35;
					Vec2 end = vertices[j] - direction * 35;
					Shape2D::Arrow(Line{ start, end }, 3, Vec2{ 20, 20 }).draw(Palette::Red);
				}
			}
		}

		for (int i = 0; i < verticesNum; i++) {
			Circle(vertices[i], 35).draw(Palette::White);
			Circle(vertices[i], 35).drawFrame(3, Palette::Black);
			circleFont(i + 1).drawAt(vertices[i], Palette::Black);
		}

		for (int i = 0; i < verticesNum; i++) {
			if (i == 0) {
				searchCandidateSquares[i].draw(Palette::Gold);
			}
			else {
				searchCandidateSquares[i].draw(Palette::White);
			}

			searchCandidateSquares[i].drawFrame(3, 0, Palette::Black);
			if (searchCandidate[i] != -1) {
				exampleFont(Format(searchCandidate[i] + 1)).drawAt(searchCandidateSquares[i].center(), Palette::Black);
			}
		}

		exampleFont(U"探索候補").draw(searchCandidateSquares[0].x - 18, searchCandidateSquares[0].y - 50, Palette::Black);

		for (int i = 0; i < verticesNum; i++) {  // 数値
			distSquares[i].draw(Palette::White);

			distSquares[i].drawFrame(3, 0, Palette::Black);
			if (dist[i] != -1) {
				exampleFont(Format(dist[i])).drawAt(distSquares[i].center(), Palette::Black);
			}
		}

		for (int i = verticesNum; i < 2 * verticesNum; i++) {  // インデックス
			distSquares[i].draw(Palette::Lightskyblue);
			distSquares[i].drawFrame(3, 0, Palette::Black);
			exampleFont(Format(i - verticesNum + 1)).drawAt(distSquares[i].center(), Palette::Black);
		}

		exampleFont(U"順序").draw(distSquares[0].x - 32, distSquares[0].y - 50, Palette::Black);

		algorithmDetailBoxFont(detailStr).draw(algorithmDetailBox2, Palette::Black);
	}
	void algo() {
		String ret;
		if (nowStep == 0) {
			std::vector<int>tmp;
			for (int i = 0; i < verticesNum; i++) {
				for (int j = 0; j < verticesNum; j++) {
					if (edges[i][j]) {
						inDegree[j]++;
					}
				}
			}
			for (int i = 0; i < verticesNum; i++) {
				if (inDegree[i] == 0) {
					que.push(i);
					tmp.push_back(i + 1);
				}
			}
			ret = U"全てのノードを探索し、入次数が0のノード" + Format(tmp) + U"を探索候補に入れます\n";
			ret += U"この時点で入次数が0のノードはトポロジカル順序が最初である事は確定しているので、1番目とします";
			for (int i = 0; i < tmp.size(); i++) {
				dist[tmp[i] - 1] = 1;
			}
			flag1 = false;
			updateSearchCandidate();
		}
		else if (que.empty()) {
			ret = U"全ての辺を削除したため、探索を終了します";
			flagEnd = true;
		}
		else if (nowStep % 2 == 1) {
			ret = U"キューの先頭のノードは(" + Format(que.front() + 1) + U")なので、そのノードから接続している辺を削除します\n";
			for (int i = 0; i < verticesNum; i++) {
				if (realEdges[que.front()][i]) {
					inDegree[i]--;
					realEdges[que.front()][i] = false;
					if (inDegree[i] == 0) {
						queWillAdd.push(i);
						willChangeDist.push({ i, dist[que.front()] + 1 });
					}
				}
			}

		}
		else {
			if (queWillAdd.empty()) {
				ret = U"入次数が0になるノードが存在しないので、キューの先頭を削除します";
				que.pop();
				updateSearchCandidate();
			}
			else {
				std::vector<int>t;
				auto T = queWillAdd;
				while (!T.empty()) {
					t.push_back(T.front() + 1);
					T.pop();
				}
				ret += U"一ステップ前の操作により入次数が0になったノード" + Format(t) + U"をキューに追加し、探索済みの頂点をキューから削除します\n";
				ret += U"また入次数を0にした削除の辺を持っていたノードのトポロジカル順序から+1した数である" + Format(willChangeDist.front().second) + U"をそのノードのトポロジカル順序とします\n";
				que.pop();
				int tmp = (int)queWillAdd.size();
				for (int i = 0; i < tmp; i++) {
					que.push(queWillAdd.front());
					queWillAdd.pop();
				}
				while (!willChangeDist.empty()) {
					dist[willChangeDist.front().first] = willChangeDist.front().second;
					willChangeDist.pop();
				}
			}

			updateSearchCandidate();

		}
		nowStep++;
		detailStr = ret;
	}
	void updateSearchCandidate() {
		auto tmp = que;
		int i = 0;
		std::vector<int>ret(10, -1);
		while (!tmp.empty()) {
			ret[i] = tmp.front();
			tmp.pop();
			i++;
		}
		searchCandidate = ret;
	}
};
