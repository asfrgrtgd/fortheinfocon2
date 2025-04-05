# pragma once
# include "algorithmVisualizerDefs.h"
# include "algorithmIntroduction.h"
# include "explainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;

class selectionSort : public AppScene::Scene {
private:
	const RoundRect detailButton = RoundRect(1480, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect stepButton = RoundRect(1380, 25, algo_vis::STEP_BUTTON_SIZE, 5);
	const Font detailBackButtonFont{ 40, Typeface::Bold };
	const Font stepButtonFont{ 45, Typeface::Bold };
	const Font barValueFont{ 38 ,Typeface::Bold};
	const Font algorithmDetailBoxFont{ 28 };
	const RoundRect algorithmDetailBox1 = RoundRect(150, 600, 1300, 250, 10);
	const Rect algorithmDetailBox2 = Rect(160, 610, 1280, 230);
	const String detailStrKey = U"selectionSort.detailStr";
	String detailStr = U"";
	std::vector<int> vec{ 5, 1, 4, 2, 3, 7, 8, 10, 6, 9 };
	const int vecSize = 10;
	int currentIndex = 0;
	int minIndex = 0;
	bool isSwapping = false;
	const int barWidth = 100;
	const int barSpacing = 30;
	const int barBaseY = 580;
	const int totalBarWidth = vecSize * barWidth + (vecSize - 1) * barSpacing;
	const int startX = (1600 - totalBarWidth) / 2;
	bool flag1 = true;
	bool flagEnd = false;
public:
	selectionSort(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"selectionSort.str");
		detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStrKey);
	}

	void update() override {
		if (!flagEnd && stepButton.leftClicked()) {
			algo();
			flag1 = false;
		}

		if (flag1) {
			if (SimpleGUI::Button(U"ランダムシャッフル", Vec2{ 10, 30 }, 200)) {
				randomVec();
			}
		}
		else {
			(void)SimpleGUI::Button(U"ランダムシャッフル", Vec2{ 10, 30 }, 200, false);
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

		algorithmDetailBoxFont(detailStr).draw(algorithmDetailBox2, Palette::Black);

		// 棒グラフ描画
		for (int i = 0; i < vecSize; ++i) {
			int barHeight = vec[i] * 40 + 30;
			int barX = startX + i * (barWidth + barSpacing);
			Rect barRect{ barX, barBaseY - barHeight, barWidth, barHeight };
			if (i == currentIndex) {
				barRect.draw(Palette::Yellow);
			}
			else if (i == minIndex) {
				barRect.draw(Palette::Red);
			}
			else {
				barRect.draw(Palette::Skyblue);
			}

			barRect.drawFrame(3, 0, Palette::Black);

			int textY = barBaseY - 30; 
			barValueFont(vec[i]).draw(Arg::center(barRect.center().x, textY), Palette::Black);
		}

		algorithmDetailBoxFont(detailStr).draw(algorithmDetailBox2, Palette::Black);
	}

	void algo() {
		if (!isSwapping) {
			detailStr = U"配列の" + Format(currentIndex + 1) +U"番目に入れる数を探すために、" +Format(currentIndex + 1) + U"番目を含む右側から最小の値を探します。";

			minIndex = currentIndex;
			for (int j = currentIndex + 1; j < vecSize; ++j) {
				if (vec[j] < vec[minIndex]) {
					minIndex = j;
				}
			}

			isSwapping = true;
		}
		else {
			if (minIndex != currentIndex) {
				detailStr = U"範囲の中の最小の値は" + Format(vec[minIndex]) + U"だったので、" + Format(currentIndex + 1) + U"番目と交換します。";
			}
			else {
				detailStr = U"配列の" + Format(currentIndex + 1) + U"番目に入れる数は範囲の中の最小値、すでに正しい位置にあるため、交換は行いません。";
			}

			std::swap(vec[currentIndex], vec[minIndex]);


			currentIndex++;
			minIndex = -1;
			isSwapping = false;
		}

		if (currentIndex >= vecSize - 1) {
			detailStr = U"全ての配列のソートを完了したため、終了します";
			flagEnd = true;
		}
	}


	void randomVec() {
		for (int i = 0; i < vecSize; i++) {
			int j = Random(i, vecSize - 1);
			std::swap(vec[i], vec[j]);
		}
	}
};
