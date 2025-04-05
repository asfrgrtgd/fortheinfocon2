# pragma once
# include "algorithmVisualizerDefs.h"
# include "algorithmIntroduction.h"
# include "explainAlgorithm.h"
# include "weightedEdgeGraphSearchDef.h"

using AppScene = SceneManager<String, algorithmIntroduction>;
class dijkstra : public AppScene::Scene, public weightedEdgeGraphSearchDef{
private:
	const RoundRect detailButton = RoundRect(1480, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect stepButton = RoundRect(1380, 25, algo_vis::STEP_BUTTON_SIZE, 5);
	const Font detailBackButtonFont{ 40,Typeface::Bold };
	const Font stepButtonFont{ 45,Typeface::Bold };
	const Font circleFont{ 40 };
	const Font weighgtedEdgeFont{ 28 , Typeface::Bold};
	const Font exampleFont{ 34,Typeface::Bold };
	const Font exampleFont2{ 26,Typeface::Bold };
	const RoundRect algorithmDetailBox1 = RoundRect(150, 600, 1300, 250, 10);
	const Rect algorithmDetailBox2 = Rect(160, 610, 1280, 230);
	const Font algorithmDetailBoxFont{ 22 };
	const Vec2 center = Vec2(610, 300);
	const double radius = 250;
	const int startVertex = 0;
	const int goalVertex = 9;
	String detailStr = U"";
	std::vector<Vec2>vertices;
	std::vector<String>searchCandidate;
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pque;
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pqueWillAdd;
	std::vector<Rect>searchCandidateSquares;
	std::vector<Rect>distSquares;
	const String detailStr1Key = U"dijkstra.detailStr";
	bool flag1 = true;
	bool flagEnd = false;
	int nowStep = 0;
	TextEditState textBoxStrA{ U"1" };
	TextEditState textBoxStrB{ U"2" };
	TextEditState textBoxStrWeight{ U"1" };
public:
	dijkstra(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"dijkstra.str");
		detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStr1Key);
		for (int i = 0; i < verticesNum; i++) {
			double angle = (Math::TwoPi / verticesNum) * i;  // 各頂点の角度
			double x = center.x + radius * std::cos(angle);  // x座標
			double y = center.y + radius * std::sin(angle);  // y座標
			vertices.push_back(Vec2(x, y));
		}
		edges = std::vector<std::vector<int>>(verticesNum, std::vector<int>(verticesNum, -1));
		for (int i = 0; i < verticesNum; i++) {
			dist.push_back(10000);
			searchCandidate.push_back(U"");
			searchCandidateSquares.push_back({ 950,70 + i * 50,230,53 });
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
			(void)SimpleGUI::TextBox(textBoxStrWeight, Vec2{ 150,  130 }, 50, 2);
		}
		else {
			(void)SimpleGUI::TextBox(textBoxStrWeight, Vec2{ 150,  130 }, 50, 2, false);
		}

		if (flag1) {
			if (SimpleGUI::Button(U"辺を追加", Vec2{ 10, 190 }, 120)) {
				addEdge();
			}
		}
		else {
			(void)SimpleGUI::Button(U"辺を追加", Vec2{ 10, 190 }, 120, false);
		}

		if (flag1) {
			if (SimpleGUI::Button(U"ランダム", Vec2{ 10, 250 }, 200)) {
				int num = Random(0, 4);
				setSample(num);
			}
		}
		else {
			(void)SimpleGUI::Button(U"ランダム", Vec2{ 10, 250 }, 200, false);
		}

		if (flag1) {
			if (SimpleGUI::Button(U"初期化", Vec2{ 10, 310 }, 100)) {
				resetEdges();
			}
		}
		else {
			(void)SimpleGUI::Button(U"初期化", Vec2{ 10, 310 }, 100, false);
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
		ClearPrint();
		algorithmDetailBox1.draw(Palette::White);

		backButton.draw(Palette::Dodgerblue);
		detailBackButtonFont(U"戻る").draw(Arg::center(backButton.x + 50, backButton.y + 35), ColorF{ 0.1 });

		detailButton.draw(Palette::Orange);
		detailBackButtonFont(U"詳細").draw(Arg::center(detailButton.x + 50, detailButton.y + 35), ColorF{ 0.1 });

		stepButton.draw(Palette::Bisque);
		stepButtonFont(U"ステップ").draw(Arg::center(stepButton.x + 100, stepButton.y + 45), Palette::Black);

		SimpleGUI::Headline(U"A   to   B   Weight", Vec2{ 10, 90 }, 190);

		// 辺を描画
		for (int i = 0; i < verticesNum; i++) {
			for (int j = i + 1; j < verticesNum; j++) {
				if (edges[i][j] != 10000) {
					Line(vertices[i], vertices[j]).draw(3, Palette::Black);
					weighgtedEdgeFont(Format(edges[i][j])).drawAt((vertices[i] + vertices[j]) / 2, Palette::Orangered);
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
			else if (dist[i] != 10000) {
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
			if (searchCandidate[i] != U"") {
				exampleFont2(searchCandidate[i]).drawAt(searchCandidateSquares[i].center(), Palette::Black);
			}
		}

		exampleFont(U"探索候補").draw(searchCandidateSquares[0].x + 43, searchCandidateSquares[0].y - 50, Palette::Black);

		for (int i = 0; i < verticesNum; i++) {  // 数値
			if (i == verticesNum - 1) {
				distSquares[i].draw(Palette::Crimson);
			}
			else {
				distSquares[i].draw(Palette::White);
			}

			distSquares[i].drawFrame(3, 0, Palette::Black);
			if (dist[i] != 10000) {
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
			ret = U"スタート地点である頂点1は距離0で最小の距離が確定しているので優先度付きキューに{距離:0,頂点:1}を追加します";
			flag1 = false;
			pque.push({ 0,startVertex });
			updateSearchCandidate();
		}
		else if (pque.empty()) {
			if (dist[goalVertex] != 10000) {
				ret = U"ゴール地点である頂点10の最小の距離が確定しました、最短経路の探索を終了します";
				flagEnd = true;
			}
			else {
				ret = U"キューが空になったので最短経路の探索を終了します";
				flagEnd = true;
			}
		}
		else if (nowStep % 2 == 1) {
			int nowVertex = pque.top().second;
			int nowDist = pque.top().first;
			
			ret += U"キューの先頭の要素は{頂点:" + Format(nowVertex + 1) + U",距離:" + Format(nowDist) + U"}で";
			if (dist[nowVertex] <= nowDist) {
				ret += U"この頂点の距離はこのキューの先端の要素よりも小さいのでキューから削除します";
				pque.pop();
				updateSearchCandidate();
				nowStep += 2;
				detailStr = ret;
				return;
			}
			else {
				ret += U"今まで距離を確定してきた頂点からの最小の距離を示しているので頂点" + Format(nowVertex + 1) + U"の距離を" + Format(nowDist) + U"に更新します\n";
				ret += U"この頂点から移動可能な近傍の頂点を調べます、この頂点から見て未探索または距離を更新可能な近傍の頂点は";
			}
			dist[nowVertex] = nowDist;
			for (int i = 0; i < verticesNum; i++) {
				if (edges[nowVertex][i] != 10000 && (dist[i] > nowDist + edges[nowVertex][i])) {
					pqueWillAdd.push({ nowDist + edges[nowVertex][i],i });
				}
			}

			if (pqueWillAdd.size() == 0) {
				ret += U"存在しないので距離の更新も、キューに対する要素の追加もありません";
			}
			else {
				for (int i = 0; i < verticesNum; i++) {
					if (edges[nowVertex][i] != 10000 && (dist[i] > nowDist + edges[nowVertex][i])) {
						ret += U"{頂点:" + Format(i + 1) + U",距離:" + Format(nowDist + edges[nowVertex][i]) + U"} ";
					}
				}
				ret.pop_back(); // 最後のスペースを削除
				ret += U"です";
			}
		}
		else {
			pque.pop();
			if (pqueWillAdd.size() != 0) {
				ret = U"1ステップ前に探索した近傍の頂点をキューに追加し、探索済みの先頭の要素を消します";
			}
			else {
				ret = U"探索済みの先頭の要素を消します";
			}
			int tmp = (int)pqueWillAdd.size();
			for (int i = 0; i < tmp; i++) {
				pque.push(pqueWillAdd.top());
				pqueWillAdd.pop();
			}
			updateSearchCandidate();
		}
		nowStep++;
		detailStr = ret;
	}
	void addEdge() {
		int a = 1, b = 2,c = 1;
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
		input = textBoxStrWeight.text;
		if (std::all_of(input.begin(), input.end(), isdigit)) {
			if (0 < Parse<int>(input)) {
				c = Parse<int>(input);
			}
			else {
				textBoxStrWeight.text = U"1";
			}
		}
		else {
			textBoxStrWeight.text = U"1";
		}
		edges[a - 1][b - 1] = c;
		edges[b - 1][a - 1] = c;
	}
	void randomAddEge() {
		std::vector<std::pair<int, int>>addableEdges;
		for (int i = 0; i < verticesNum; i++) {
			for (int j = i + 1; j < verticesNum; j++) {
				if (edges[i][j] == 10000) {
					addableEdges.push_back({ i,j });
				}
			}
		}
		if (addableEdges.size() == 0) {
			return;
		}
		int idx = Random<int>(0, (int)addableEdges.size() - 1);
		int weight = Random<int>(1, 10);
		edges[addableEdges[idx].first][addableEdges[idx].second] = weight;
		edges[addableEdges[idx].second][addableEdges[idx].first] = weight;
	}
	void updateSearchCandidate() {
		auto tmp = pque;
		int i = 0;
		std::vector<String>ret(10, U"");
		while (!tmp.empty()) {
			ret[i] = U"{頂点:" + Format(tmp.top().second + 1) + U",距離:" + Format(tmp.top().first) + U"} ";
			tmp.pop();
			i++;
		}
		searchCandidate = ret;
	}
};
