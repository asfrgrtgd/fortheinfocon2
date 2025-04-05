# pragma once
# include "algorithmVisualizerDefs.h"
# include "algorithmIntroduction.h"
# include "explainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;
class euclideanAlgorithm : public AppScene::Scene {
private:
	const RoundRect detailButton = RoundRect(1480, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect stepButton = RoundRect(1380, 25, algo_vis::STEP_BUTTON_SIZE, 5);
	const Font nSquareFont{ 58 };
	const Font explainXYFont{ 38 };
	const Font detailBackButtonFont{ 40,Typeface::Bold };
	const Font stepButtonFont{ 45,Typeface::Bold };
	const RoundRect algorithmDetailBox1 = RoundRect(150, 600, 1300, 250, 10);
	const Rect algorithmDetailBox2 = Rect(160, 610, 1280, 230);
	const Font algorithmDetailBoxFont{ 28 };
	const String detailStrKey = U"euclideanAlgorithm.detailStr";
	std::vector<Rect>xSquareList;
	std::vector<Rect>ySquareList;
	std::vector<Rect>dSquareList;
	std::vector<Rect>rSquareList;
	std::vector<int>xNumList;
	std::vector<int>yNumList;
	std::vector<int>dNumList;
	std::vector<int>rNumList;
	String detailStr = U"";
	bool flag1 = true;
	bool flagFound = false;
	int nowStep = 0;
	int X = 90;
	int Y = 54;
	TextEditState textBoxStrX{ U"90" };
	TextEditState textBoxStrY{ U"54" };
public:
	euclideanAlgorithm(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"euclideanAlgorithm.str");
		detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStrKey);
		for (int i = 0; i < 8; i++) {
			xSquareList.push_back({ 300, 40 + i * 63, 200, 65 });
			ySquareList.push_back({ 570, 40 + i * 63, 200, 65 });
			dSquareList.push_back({ 840, 40 + i * 63, 200, 65 });
			rSquareList.push_back({ 1110, 40 + i * 63, 200, 65 });

		}
	}
	void update() override {
		if (flag1) {
			if (SimpleGUI::Button(U"ランダム生成", Vec2{ 10, 30 }, 150)) {
				X = Random(2, 999);
				Y = Random(2, 999);
				while (X == Y) {
					Y = Random(2, 999);
				}
				if (X < Y)std::swap(X, Y);
				textBoxStrX.text = Format(X);
				textBoxStrY.text = Format(Y);
			}
		}
		else {
			(void)SimpleGUI::Button(U"ランダム生成", Vec2{ 10, 30 }, 150, false);
		}

		if (flag1) {
			(void)SimpleGUI::TextBox(textBoxStrX, Vec2{ 10,  130 }, 100, 3);
		}
		else {
			(void)SimpleGUI::TextBox(textBoxStrX, Vec2{ 10,  130 }, 100, 3, false);
		}

		if (flag1) {
			(void)SimpleGUI::TextBox(textBoxStrY, Vec2{ 10,  230 }, 100, 3);
		}
		else {
			(void)SimpleGUI::TextBox(textBoxStrY, Vec2{ 10,  230 }, 100, 3, false);
		}


		if (stepButton.leftClicked() && (!flagFound)) {
			if (nowStep == 0) {
				flag1 = false;
				String input = textBoxStrX.text;
				if (std::all_of(input.begin(), input.end(), isdigit)) {
					if (Parse<int>(input) == 0) {
						X = 90;
						textBoxStrX.text = U"90";
					}
					else X = Parse<int>(input);
				}
				else {
					X = 90;
					textBoxStrX.text = U"90";
				}
				input = textBoxStrY.text;
				if (std::all_of(input.begin(), input.end(), isdigit)) {
					if (Parse<int>(input) == 0) {
						Y = 54;
						textBoxStrY.text = U"54";
					}
					else Y = Parse<int>(input);
				}
				else {
					Y = 54;
					textBoxStrY.text = U"54";
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
		SimpleGUI::Headline(U"x", Vec2{ 10, 90 }, 100);
		SimpleGUI::Headline(U"y", Vec2{ 10, 190 }, 100);

		algorithmDetailBox1.draw(Palette::White);

		backButton.draw(Palette::Dodgerblue);
		detailBackButtonFont(U"戻る").draw(Arg::center(backButton.x + 50, backButton.y + 35), ColorF{ 0.1 });

		detailButton.draw(Palette::Orange);
		detailBackButtonFont(U"詳細").draw(Arg::center(detailButton.x + 50, detailButton.y + 35), ColorF{ 0.1 });

		stepButton.draw(Palette::Bisque);
		stepButtonFont(U"ステップ").draw(Arg::center(stepButton.x + 100, stepButton.y + 45), Palette::Black);

		for (int i = 0; i < Max(xNumList.size(), dNumList.size()); i++) {
			xSquareList[i].draw(Palette::White);
			xSquareList[i].drawFrame(2, 0, Palette::Black);

			ySquareList[i].draw(Palette::White);
			ySquareList[i].drawFrame(2, 0, Palette::Black);

			explainXYFont(U"x").drawAt(xSquareList[0].x + 100, xSquareList[0].y - 24, Palette::Black);
			explainXYFont(U"y").drawAt(ySquareList[0].x + 100, ySquareList[0].y - 27, Palette::Black);

			if (dNumList.size() > i) {
				dSquareList[i].draw(Palette::White);
				dSquareList[i].drawFrame(2, 0, Palette::Black);

				rSquareList[i].draw(Palette::White);
				rSquareList[i].drawFrame(2, 0, Palette::Black);
			}

			if (xNumList.size() > i) {
				nSquareFont(xNumList[i]).drawAt(xSquareList[i].center(), Palette::Black);
				nSquareFont(yNumList[i]).drawAt(ySquareList[i].center(), Palette::Black);
				nSquareFont(U"/").drawAt((ySquareList[i].center() - xSquareList[i].center()) / 2 + xSquareList[i].center(), Palette::Black);
			}
			if (dNumList.size() > i) {
				nSquareFont(dNumList[i]).drawAt(dSquareList[i].center(), Palette::Black);
				nSquareFont(rNumList[i]).drawAt(rSquareList[i].center(), Palette::Black);
				nSquareFont(U"=").drawAt((dSquareList[i].center() - ySquareList[i].center()) / 2 + ySquareList[i].center(), Palette::Black);
				nSquareFont(U"...").drawAt((rSquareList[i].center() - dSquareList[i].center()) / 2 + dSquareList[i].center(), Palette::Black);
			}
		}

		algorithmDetailBoxFont(detailStr).draw(algorithmDetailBox2, Palette::Black);

	}
	void algo() {
		String ret;
		if (nowStep == 0) {
			xNumList.push_back(X);
			yNumList.push_back(Y);
		}
		if (nowStep % 2 == 0) {
			dNumList.push_back(X / Y);
			rNumList.push_back(X % Y);
			if (X % Y == 0) {
				ret = U"yで割り切れたので" + Format(Y) + U"が最大公約数となり、アルゴリズムを終了します";
				flagFound = true;
			}
			else {
				ret = U"xをyで割った商は" + Format(X / Y) + U"で余りは" + Format(X % Y) + U"です";
			}
		}
		else {
			X = yNumList[(int)yNumList.size() - 1];
			Y = rNumList[(int)rNumList.size() - 1];
			xNumList.push_back(X);
			yNumList.push_back(Y);
			ret = U"一つ前のステップのyをxに、余りをyに代入します";
		}
		nowStep++;
		detailStr = ret;
	}
};
