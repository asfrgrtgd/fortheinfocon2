# pragma once
# include "algorithmVisualizerDefs.h"
# include "algorithmIntroduction.h"
# include "explainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;
class sieveOfEratosthenes : public AppScene::Scene {
private:
	const RoundRect detailButton = RoundRect(1480, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect stepButton = RoundRect(1380, 25, algo_vis::STEP_BUTTON_SIZE, 5);
	const Font detailBackButtonFont{ 40,Typeface::Bold };
	const Font stepButtonFont{ 45,Typeface::Bold };
	const Font primeSquaresFont{ 40 };
	const RoundRect algorithmDetailBox1 = RoundRect(150, 600, 1300, 250, 10);
	const Rect algorithmDetailBox2 = Rect(160, 610, 1280, 230);
	const Font algorithmDetailBoxFont{ 28 };
	const String detailStrKey = U"sieveOfEratosthenes.detailStr";
	String detailStr = U"";
	bool flag1 = true;
	bool flagFound = false;
	std::vector<bool>isPrime;
	std::vector<Rect>primeSquares;
	int nowStep = 0;
	int nowI = 1;
	int N = 20;
	TextEditState textBoxStr{ U"20" };
public:
	sieveOfEratosthenes(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"sieveOfEratosthenes.str");
		detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStrKey);
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 20; j++) {
				primeSquares.push_back({ 160 + j * 64, 180 + i * 64, 67, 67 });
			}
		}
		isPrime = std::vector<bool>(101, true);
	}
	void update() override {
		if (flag1) {
			if (SimpleGUI::Button(U"ランダム生成", Vec2{ 10, 30 }, 150)) {
				N = Random(20, 99);
				textBoxStr.text = Format(N);
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
				flag1 = false;
				String input = textBoxStr.text;
				if (std::all_of(input.begin(), input.end(), isdigit)) {
					if (Parse<int>(input) < 2) {
						N = 20;
						textBoxStr.text = U"20";
					}
					else N = Parse<int>(input);
				}
				else {
					N = 20;
					textBoxStr.text = U"20";
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
		SimpleGUI::Headline(U"n", Vec2{ 10, 90 }, 100);

		algorithmDetailBox1.draw(Palette::White);

		backButton.draw(Palette::Dodgerblue);
		detailBackButtonFont(U"戻る").draw(Arg::center(backButton.x + 50, backButton.y + 35), ColorF{ 0.1 });

		detailButton.draw(Palette::Orange);
		detailBackButtonFont(U"詳細").draw(Arg::center(detailButton.x + 50, detailButton.y + 35), ColorF{ 0.1 });

		stepButton.draw(Palette::Bisque);
		stepButtonFont(U"ステップ").draw(Arg::center(stepButton.x + 100, stepButton.y + 45), Palette::Black);

		algorithmDetailBoxFont(detailStr).draw(algorithmDetailBox2, Palette::Black);

		if (nowStep != 0) {
			for (int i = 1; i < N; i++) {
				if (isPrime[i]) {
					primeSquares[i].draw(Palette::Dodgerblue);
				}
				else {
					primeSquares[i].draw(Palette::White);
				}
				primeSquares[i].drawFrame(3, 0, Palette::Black);
				primeSquaresFont(Format(i + 1)).drawAt(primeSquares[i].center(), Palette::Black);
			}
		}
	}
	void algo() {
		String ret;
		if (nowStep == 0) {
			ret = U"2から" + Format(N) + U"までの数を昇順に列挙します、この段階では" + Format(N) + U"を除く全ての数が素数の可能性があるので" + Format(N) + U"以外の全ての数を青色にします";
			isPrime[N - 1] = false;
		}
		else {
			if (nowI * nowI > N) {
				flagFound = true;
				std::vector<int>primes;
				for (int i = 1; i < N; i++) {
					if (isPrime[i]) {
						primes.push_back(i + 1);
					}
				}
				ret = Format(nowI) + U"は" + Format(N) + U"の平方根より大きいのでステップは中止し、残った青色の部分が素数になります\n";
				ret += U"素数は" + Format(primes.size()) + U"個あり、それぞれ" + Format(primes) + U"です";
			}
			else {
				if (isPrime[nowI]) {
					ret = Format(nowI + 1) + U"は青色でマークされているので素数です、" + Format(nowI + 1) + U"を除く全ての" + Format(nowI + 1) + U"の倍数をリストから除外します";
					for (int i = nowI + 1; i < N; i++) {
						if ((i + 1) % (nowI + 1) == 0) {
							isPrime[i] = false;
						}
					}
				}
				else {
					ret = Format(nowI + 1) + U"は青色でマークされていないので素数では無いです、除外する操作をしないで次のステップに行きます";
				}
				nowI++;
			}
			
		}
		nowStep++;
		detailStr = ret;
	}
};
