# pragma once
# include "algorithmVisualizerDefs.h"
# include "algorithmIntroduction.h"
# include "explainAlgorithm.h"
# include "graphSearchDef.h"

using AppScene = SceneManager<String, algorithmIntroduction>;
class breadthFirstSearchGraph : public AppScene::Scene, graphSearchDef {
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
	std::vector<Rect>searchCandidateSquares;
	std::vector<Rect>distSquares;
	const String detailStr1Key = U"breadthFirstSearchGraph.detailStr";
	bool flag1 = true;
	bool flagEnd = false;
	int nowStep = 0;
	TextEditState textBoxStrA{ U"1" };
	TextEditState textBoxStrB{ U"2" };
public:
	breadthFirstSearchGraph(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"breadthFirstSearchGraph.str");
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
			searchCandidate.push_back(-1);
			searchCandidateSquares.push_back({ 1000,70 + i * 50,100,53 });
		}
		for (int i = 0; i < verticesNum; i++) {  // 数値
			distSquares.push_back({ 1260,70 + i * 50,60,53 });
		}
		for (int i = 0; i < verticesNum; i++) {  // インデックス
			distSquares.push_back({ 1200,70 + i * 50,63,53 });
		}
		setSample(0);

	}
	void update() override {
		if (flag1) {
			if (SimpleGUI::Button(U"サンプル", Vec2{ 10, 30 }, 150)) {
				setSample(0);
			}
		}
		else {
			(void)SimpleGUI::Button(U"サンプル", Vec2{ 10, 30 }, 150, false);
		}

		if (flag1) {
			(void)SimpleGUI::TextBox(textBoxStrA, Vec2{ 10,  130 }, 50, 2);
		}
		else {
			(void)SimpleGUI::TextBox(textBoxStrA, Vec2{ 10,  130 }, 50, 2, false);
		}

		if (flag1) {
			(void)SimpleGUI::TextBox(textBoxStrB, Vec2{ 80,  130 }, 50, 2);
		}
		else {
			(void)SimpleGUI::TextBox(textBoxStrB, Vec2{ 80,  130 }, 50, 2, false);
		}

		if (flag1) {
			if (SimpleGUI::Button(U"辺を追加", Vec2{ 140, 130 }, 120)) {
				addEdge();
			}
		}
		else {
			(void)SimpleGUI::Button(U"辺を追加", Vec2{ 140, 130 }, 120, false);
		}

		if (flag1) {
			if (SimpleGUI::Button(U"ランダムな辺を追加", Vec2{ 10, 190 }, 200)) {
				randomAddEge();
			}
		}
		else {
			(void)SimpleGUI::Button(U"ランダムな辺を追加", Vec2{ 10, 190 }, 200, false);
		}

		if (flag1) {
			if (SimpleGUI::Button(U"初期化", Vec2{ 10, 250 }, 100)) {
				resetEdges();
			}
		}
		else {
			(void)SimpleGUI::Button(U"初期化", Vec2{ 10, 250 }, 100, false);
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

		SimpleGUI::Headline(U"A   to   B", Vec2{ 10, 90 }, 120);

		// 辺を描画
		for (int i = 0; i < verticesNum; i++) {
			for (int j = i + 1; j < verticesNum; j++) {
				if (edges[i][j]) {
					Line(vertices[i], vertices[j]).draw(3, Palette::Black);
				}
			}
		}

		for (int i = 0; i < verticesNum; i++) {
			if (i == startVertex) {
				Circle(vertices[i], 35).draw(Palette::Cyan);
			}
			else if (i == goalVertex) {
				Circle(vertices[i], 35).draw(Palette::Crimson);
			}
			else if (dist[i] != -1) {
				Circle(vertices[i], 35).draw(Palette::Lightgreen);
			}
			else {
				Circle(vertices[i], 35).draw(Palette::White);
			}
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
			if (i == verticesNum - 1) {
				distSquares[i].draw(Palette::Crimson);
			}
			else {
				distSquares[i].draw(Palette::White);
			}

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

		exampleFont(U"距離").draw(distSquares[0].x - 32, distSquares[0].y - 50, Palette::Black);

		algorithmDetailBoxFont(detailStr).draw(algorithmDetailBox2, Palette::Black);
	}
	void algo() {
		String ret;
		if (nowStep == 0) {
			ret = U"スタート地点である頂点1を探索候補としてキューに入れ、頂点1の距離を0とします";
			dist[startVertex] = 0;
			flag1 = false;
			que.push(startVertex);
			updateSearchCandidate();
		}
		else if (que.empty()) {
			ret += U"探索可能な全ての頂点を探索しましたが、ゴール地点に到達出来なかったので探索を終了します";
			flagEnd = true;
		}
		else if (nowStep % 2 == 1) {
			int nowVertex = que.front();
			ret += U"キューの先頭の要素は(" + Format(nowVertex + 1) + U")なのでこの頂点から移動可能な近傍の頂点を調べます、この頂点から見て未探索な近傍の頂点は";
			for (int i = 0; i < verticesNum; i++) {
				if (edges[nowVertex][i] && dist[i] == -1) {
					queWillAdd.push(i);
				}
			}

			if (queWillAdd.size() == 0) {
				ret += U"存在しないので距離の更新も、キューに対する要素の追加もありません";
			}
			else {
				bool end = false;
				for (int i = 0; i < verticesNum; i++) {
					if (edges[nowVertex][i] && dist[i] == -1) {
						ret += U"(" + Format(i + 1) + U") ";
						dist[i] = dist[nowVertex] + 1;
						if (i == goalVertex) {
							end = true;
						}
					}
				}
				ret.pop_back(); // 最後のスペースを削除
				ret += U"です、これらの頂点は元の頂点から距離1の移動でたどり着けるので、距離は元の(" + Format(nowVertex + 1) + U")の距離に+1した値です。\n";
				if (end) {
					ret += U"近傍の頂点にゴール地点が含まれていたので、探索を終了します。";
					flagEnd = true;
				}
			}
		}
		else {
			ret += U"1ステップ前に探索した近傍の頂点をキューに追加し、探索済みの先頭の要素を消します(近傍頂点がない場合はキューの先頭を削除するだけになります)";
			que.pop();
			int tmp = (int)queWillAdd.size();
			for (int i = 0; i < tmp; i++) {
				que.push(queWillAdd.front());
				queWillAdd.pop();
			}
			updateSearchCandidate();
		}
		nowStep++;
		detailStr = ret;
	}
	void addEdge() {
		int a = 1, b = 2;
		String input = textBoxStrA.text;
		if (std::all_of(input.begin(), input.end(), isdigit)) {
			if (0 < Parse<int>(input) && Parse<int>(input) <= verticesNum) {
				a = Parse<int>(input);
			}
			else {
				textBoxStrA.text = U"1";
			}
		}
		else {
			textBoxStrA.text = U"1";
		}
		input = textBoxStrB.text;
		if (std::all_of(input.begin(), input.end(), isdigit)) {
			if (0 < Parse<int>(input) && Parse<int>(input) <= verticesNum) {
				b = Parse<int>(input);
			}
			else {
				textBoxStrB.text = U"2";
			}
		}
		else {
			textBoxStrB.text = U"2";
		}
		edges[a - 1][b - 1] = true;
		edges[b - 1][a - 1] = true;
	}
	void randomAddEge() {
		std::vector<std::pair<int, int>>addableEdges;
		for (int i = 0; i < verticesNum; i++) {
			for (int j = i + 1; j < verticesNum; j++) {
				if (!edges[i][j]) {
					addableEdges.push_back({ i,j });
				}
			}
		}
		if (addableEdges.size() == 0) {
			return;
		}
		int idx = Random<int>(0, (int)addableEdges.size() - 1);
		edges[addableEdges[idx].first][addableEdges[idx].second] = true;
		edges[addableEdges[idx].second][addableEdges[idx].first] = true;
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
