# pragma once
# include "algorithmVisualizerDefs.h"
# include "algorithmIntroduction.h"
# include "explainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;
class linearSearch : public AppScene::Scene {
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
	const String detailStrKey = U"linearSearch.detailStr";
	std::vector<int>vec{ 3, 6, 4, 2, 1, 5, 7 };
	bool flag1 = true;
	bool flagFound = false;
	int nowStep = 0;
	int searchValue = 0;
	TextEditState textBoxStr{ U"5" };
public:
	linearSearch(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"linearSearch.str");
		detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStrKey);
		for (int i = 0; i < 7; i++) {
			vecSquaresShadow.push_back({ 230 + i * 165,350,150,150 });
			vecSquares.push_back({ 235 + i * 165,355,140,140 });
		}
	}
	void update() override {
		if (flag1) {
			if (SimpleGUI::Button(U"ランダム生成", Vec2{ 10, 30 }, 150)) {
				for (int i = 0; i < 7; i++) {
					vec[i] = Random(1, 99);
				}
			}
		}
		else {
			(void)SimpleGUI::Button(U"ランダム生成", Vec2{ 10, 30 }, 150, false);
		}

		if (flag1) {
			SimpleGUI::TextBox(textBoxStr, Vec2{ 10,  130 }, 100, 2);
		}
		else {
			SimpleGUI::TextBox(textBoxStr, Vec2{ 10,  130 }, 100, 2, false);
		}

		if (stepButton.leftClicked() && (!flagFound)) {
			if (nowStep == 0) {
				flag1 = false;
				String input = textBoxStr.text;
				if (std::all_of(input.begin(), input.end(), isdigit)) {
					searchValue = Parse<int>(input);

				}
				else {
					searchValue = 5;
					textBoxStr.text = U"5";
				}
				algo(nowStep);
			}
			else if (nowStep < 7) {
				algo(nowStep);
			}
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
			vecSquares[i].draw(Palette::White);
			vecSquareFont(vec[i]).drawAt(vecSquares[i].center(), Palette::Black);
		}

		algorithmDetailBoxFont(detailStr).draw(algorithmDetailBox2, Palette::Black);

		if (nowStep != 0) {
			Shape2D::Arrow(Line{ vecSquares[nowStep - 1].x + 70, vecSquares[nowStep - 1].y - 140, vecSquares[nowStep - 1].x + 70, vecSquares[nowStep - 1].y - 40 }, 40, Vec2{ 40,40 }).draw(Palette::Black);
		}
	}
	void algo(int num) {
		String ret = s3d::Fmt(algo_vis::SearchINIDataFromKey(getData().data_ini, U"linearSearch.algoHeadStr"))(num + 1, vec[num], searchValue);
		if (vec[num] == searchValue) {
			ret += U"検索値と等しい要素を見つけたので探索を終了します";
			flagFound = true;
			nowStep++;
		}
		else {
			nowStep++;
			if (nowStep == 7) {
				flagFound = true;
				ret += U"検索値とは一致しません、全ての要素を確認した上で一致する要素が無かった為この配列には検索値は含まれていません";
			}
			else {
				ret += U"検索値とは一致しないので探索を続けます";
			}
		}
		detailStr = ret;
	}
};
