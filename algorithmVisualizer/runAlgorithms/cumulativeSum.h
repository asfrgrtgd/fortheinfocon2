# pragma once
# include "algorithmVisualizerDefs.h"
# include "algorithmIntroduction.h"
# include "explainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;
class cumulativeSum : public AppScene::Scene {
private:
	const RoundRect detailButton = RoundRect(1480, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect stepButton = RoundRect(1380, 25, algo_vis::STEP_BUTTON_SIZE, 5);
	const Font detailBackButtonFont{ 40,Typeface::Bold };
	const Font stepButtonFont{ 45,Typeface::Bold };
	const RoundRect algorithmDetailBox1 = RoundRect(150, 600, 1300, 250, 10);
	const Rect algorithmDetailBox2 = Rect(160, 610, 1280, 230);
	const Font algorithmDetailBoxFont{ 25 };
	const String detailStrKey = U"cumulativeSum.detailStr";
	const Font vecIndexExampleFont{ 34,Typeface::Bold };
	const Font vecExampleFont{ 60,Typeface::Bold };
	String detailStr = U"";
	std::vector<int> vec{ 3, 7, 1, 9, 4, 2, 6, 10, 8, 5 };
	std::vector<int> vecColor;
	std::vector<int> vecSum;
	std::vector<int> vecSumColor;
	std::vector<Rect>vecIndSquares;
	std::vector<Rect>vecSquares;
	std::vector<Rect>vecSumSquares;
	bool flag1 = true;
	bool flagFound = false;
	int nowStep = 0;
	int N = 10;
	int L = 1, R = 5;
	TextEditState textBoxStrL{ U"1" };
	TextEditState textBoxStrR{ U"5" };
public:
	cumulativeSum(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"cumulativeSum.str");
		detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStrKey);
		vecSum.resize(N);
		vecSumColor.resize(N);
		vecColor.resize(N);
		for (int i = 0; i < N; i++) {
			vecIndSquares.push_back({ 175 + i * 125,150,130,50 });
		}
		for (int i = 0; i < N; i++) {
			vecSquares.push_back({ 175 + i * 125,203,130,130 });
		}
		for (int i = 0; i < N; i++) {
			vecSumSquares.push_back({ 175 + i * 125, 328, 130, 130 });
		}
	}
	void update() override {
		if (flag1) {
			if (SimpleGUI::Button(U"ランダム生成", Vec2{ 10, 30 }, 150)) {
				for(int i = 0; i < N; i++) {
					vec[i] = Random<int>(1, 99);
				}
			}
		}
		else {
			(void)SimpleGUI::Button(U"ランダム生成", Vec2{ 10, 30 }, 150, false);
		}

		(void)SimpleGUI::TextBox(textBoxStrL, Vec2{ 10,  130 }, 100, 3);

		(void)SimpleGUI::TextBox(textBoxStrR, Vec2{ 10,  230 }, 100, 3);

		if (stepButton.leftClicked()) {
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
		SimpleGUI::Headline(U"L", Vec2{ 10, 90 }, 100);

		SimpleGUI::Headline(U"R", Vec2{ 10, 190 }, 100);

		algorithmDetailBox1.draw(Palette::White);

		backButton.draw(Palette::Dodgerblue);
		detailBackButtonFont(U"戻る").draw(Arg::center(backButton.x + 50, backButton.y + 35), ColorF{ 0.1 });

		detailButton.draw(Palette::Orange);
		detailBackButtonFont(U"詳細").draw(Arg::center(detailButton.x + 50, detailButton.y + 35), ColorF{ 0.1 });

		stepButton.draw(Palette::Bisque);
		stepButtonFont(U"ステップ").draw(Arg::center(stepButton.x + 100, stepButton.y + 45), Palette::Black);

		algorithmDetailBoxFont(detailStr).draw(algorithmDetailBox2, Palette::Black);

		for (int i = 0; i < N; i++) {
			vecIndSquares[i].draw(Palette::White);
			vecIndSquares[i].drawFrame(3, 0, Palette::Black);

			if(vecColor[i] == 0) {
				vecSquares[i].draw(Palette::White);
			}
			else if(vecColor[i] == 1) {
				vecSquares[i].draw(Palette::Cyan);
			}

			vecSquares[i].drawFrame(5, 0, Palette::Black);

			vecIndexExampleFont(Format(i + 1)).drawAt(vecIndSquares[i].center(), Palette::Black);
			vecExampleFont(Format(vec[i])).drawAt(vecSquares[i].center(), Palette::Black);
		}

		if (!flag1) {
			for(int i = 0; i < N; i++) {
				if (vecSumColor[i] == 0) {
					vecSumSquares[i].draw(Palette::White);
				}
				else if (vecSumColor[i] == 1) {
					vecSumSquares[i].draw(Palette::Orange);
				}
				else if(vecSumColor[i] == 2) {
					vecSumSquares[i].draw(Palette::Royalblue);
				}
				vecSumSquares[i].drawFrame(5, 0, Palette::Black);
				vecExampleFont(Format(vecSum[i])).drawAt(vecSumSquares[i].center(), Palette::Black);
			}
		}

	}
	void algo() {
		String ret;
		if(nowStep == 0) {
			flag1 = false;
			vecSum = vec;
			for (int i = 1; i < N; i++) {
				vecSum[i] += vecSum[i - 1];
			}
			ret = U"与えられた数列を元に累積和を構築します";	
		}
		else {
			String input = textBoxStrL.text;
			for(int i = 0; i < N; i++) {
				vecSumColor[i] = 0;
				vecColor[i] = 0;
			}
			if (std::all_of(input.begin(), input.end(), isdigit)) {
				if (0 < Parse<int>(input) && Parse<int>(input) <= N) {
					L = Parse<int>(input);
				}
				else {
					L = 1;
					textBoxStrL.text = U"1";
				}
			}
			else {
				L = 1;
				textBoxStrL.text = U"1";
			}
			input = textBoxStrR.text;
			if (std::all_of(input.begin(), input.end(), isdigit)) {
				if (0 < Parse<int>(input) && Parse<int>(input) <= N) {
					R = Parse<int>(input);
				}
				else {
					R = 5;
					textBoxStrR.text = U"5";
				}
			}
			else {
				R = 5;
				textBoxStrR.text = U"5";
			}
			if (L > R) {
				std::swap(L, R);
				textBoxStrL.text = Format(L);
				textBoxStrR.text = Format(R);
			}

			for(int i = L - 1; i <= R - 1; i++) {
				vecColor[i] = 1;
			}
			int sum = 0;

			if(L == 1) {
				sum = vecSum[R - 1];
				ret = U"L = 1のため引き算をせずに、累積和のR番目の値が1からRまでの総和になり、" + Format(sum) + U"です\n";
				vecSumColor[R - 1] = 1;
			}
			else {
				sum = vecSum[R - 1] - vecSum[L - 2];
				ret = U"累積和のR番目の値からL-1番目の値を引くことで、LからRまでの総和が求まり、";
				ret += Format(L) + U"番目から" + Format(R) + U"番目までの総和は";
				ret += Format(vecSum[R - 1]) + U" - " + Format(vecSum[L - 2]) + U" = " + Format(sum) + U"です\n";
				vecSumColor[R - 1] = 1;
				vecSumColor[L - 2] = 2;
			}
			ret += U"累積和の構築が既に出来ているので他のL Rに対しても同様に計算が可能です";
		}
		nowStep++;
		detailStr = ret;
	}
};
