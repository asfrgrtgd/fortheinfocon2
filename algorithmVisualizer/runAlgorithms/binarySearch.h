# pragma once
# include "algorithmVisualizerDefs.h"
# include "algorithmIntroduction.h"
# include "explainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;
class binarySearch : public AppScene::Scene {
private:
	const RoundRect detailButton = RoundRect(1480, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect stepButton = RoundRect(1380, 25, algo_vis::STEP_BUTTON_SIZE, 5);
	const Font vecSquareFont{ 80,Typeface::Bold };
	const Font detailBackButtonFont{ 40,Typeface::Bold };
	const Font stepButtonFont{ 45,Typeface::Bold };
	const RoundRect algorithmDetailBox1 = RoundRect(150, 600, 1300, 250, 10);
	const Rect algorithmDetailBox2 = Rect(160, 610, 1280, 230);
	const Font algorithmDetailBoxFont{ 30 };
	std::vector<Rect>vecSquaresShadow;
	std::vector<Rect>vecSquares;
	String detailStr = U"";
	const String detailStrKey = U"binarySearch.detailStr";
	std::vector<int>vec{ 1, 3, 5, 7, 9, 11, 13 }, vecColor{ 0,0,0,0,0,0,0 };
	bool flag1 = true;
	bool flagFound = false;
	int nowStep = 0;
	int searchValue{};
	int l = 0, r = 6, med = 3;
	TextEditState textBoxStr{ U"5" };
public:
	binarySearch(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"binarySearch.str");
		detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStrKey);
		for (int i = 0; i < 7; i++) {
			vecSquaresShadow.push_back({ 230 + i * 165,350,150,150 });
			vecSquares.push_back({ 235 + i * 165,355,140,140 });
		}
	}
	void update() override {
		if (flag1) {
			if (SimpleGUI::Button(U"ランダム生成", Vec2{ 10, 30 }, 150)) {
				while (1) { //重なり排除
					int tmp = 0;
					for (int i = 0; i < 7; i++) {
						vec[i] = Random(1, 99);
					}
					sort(vec.begin(), vec.end());
					for (int i = 0; i < 6; i++) {
						if (vec[i] != vec[i + 1])tmp++;
					}
					if (tmp == 6)break;
				}

			}
		}
		else {
			(void)SimpleGUI::Button(U"ランダム生成", Vec2{ 10, 30 }, 150, false);
		}

		if (flag1) {
			(void)SimpleGUI::TextBox(textBoxStr, Vec2{ 10,  130 }, 100, 2);
		}
		else {
			(void)SimpleGUI::TextBox(textBoxStr, Vec2{ 10,  130 }, 100, 2, false);
		}

		if (stepButton.leftClicked() && (!flagFound)) {
			if (nowStep == 0) {
				for (int i = 0; i < 7; i++) {
					vecColor[i] = 1;
				}
				flag1 = false;
				String input = textBoxStr.text;
				if (std::all_of(input.begin(), input.end(), isdigit)) {
					searchValue = Parse<int>(input);

				}
				else {
					searchValue = 5;
					textBoxStr.text = U"5";
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

	}
	void draw() const override {
		SimpleGUI::Headline(U"検索値", Vec2{ 10, 90 }, 100);
		algorithmDetailBox1.draw(Palette::White);

		backButton.draw(Palette::Dodgerblue);
		detailBackButtonFont(U"戻る").draw(Arg::center(backButton.x + 50, backButton.y + 35), ColorF{ 0.1 });

		detailButton.draw(Palette::Orange);
		detailBackButtonFont(U"詳細").draw(Arg::center(detailButton.x + 50, detailButton.y + 35), ColorF{ 0.1 });

		stepButton.draw(Palette::Bisque);
		stepButtonFont(U"ステップ").draw(Arg::center(stepButton.x + 100, stepButton.y + 45), Palette::Black);

		for (int i = 0; i < 7; i++) {
			vecSquaresShadow[i].draw(Palette::Black);
			if (vecColor[i] == 0) {
				vecSquares[i].draw(Palette::White);
			}
			if (vecColor[i] == 1) {
				vecSquares[i].draw(Palette::Royalblue);
			}

			vecSquareFont(vec[i]).drawAt(vecSquares[i].center(), Palette::Black);
		}

		algorithmDetailBoxFont(detailStr).draw(algorithmDetailBox2, Palette::Black);

		if (nowStep != 0 && med != -1) {
			Shape2D::Arrow(Line{ vecSquares[med].x + 70, vecSquares[med].y - 140, vecSquares[med].x + 70, vecSquares[med].y - 40 }, 40, Vec2{ 40,40 }).draw(Palette::Black);
		}

	}
	void algo() {
		String ret;
		int L = l + 1, R = r + 1;
		if (r - l == 0) {
			if (searchValue == vec[(l + r) / 2]) {
				ret += U"配列の" + Format((l + r) / 2 + 1) + U"番目の要素が検索値と等しい要素ので探索を終了します";
				flagFound = true;
			}
			else {
				ret += U"配列内に検索値と一致する要素が無かった為この配列には検索値は含まれていません";
				flagFound = true;
			}
		}
		else if (nowStep % 2 == 0) {
			ret += U"配列の" + Format(L) + U"番目から" + Format(R) + U"番目の中央は" + Format((L + R) / 2) + U"番目で配列の要素は" + Format(vec[(l + r) / 2]) + U"で検索値" + Format(searchValue) + U"と比較すると";
			med = (l + r) / 2;
			if (vec[(l + r) / 2] == searchValue) {
				ret += U"等しい値なので、探索を終了します";
				flagFound = true;
			}
			else if (vec[(l + r) / 2] < searchValue) {
				ret += U"中央の要素が検索値未満です";
			}
			else {
				ret += U"中央の要素が検索値より大きいです";
			}
		}
		else {
			if (vec[(l + r) / 2] == searchValue) {
				ret += U"等しい値なので、探索を終了します";
				flagFound = true;
			}
			else if (vec[(l + r) / 2] < searchValue) {
				ret += U"中央の要素が検索値未満なので検索範囲を右半分に絞ります";
				l = (l + r) / 2 + 1;
			}
			else {
				ret += U"中央の要素が検索値より大きいので検索範囲を左半分に絞ります";
				r = (l + r) / 2 - 1;
			}

			for (int i = 0; i < 7; i++) {
				if (l <= i && i <= r)vecColor[i] = 1;
				else vecColor[i] = 0;
			}
			med = -1;
		}
		nowStep++;
		detailStr = ret;
	}
};
