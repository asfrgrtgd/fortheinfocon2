# pragma once
# include "algorithmVisualizerDefs.h"
# include "algorithmIntroduction.h"
# include "explainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;

class bubbleSort : public AppScene::Scene {
private:
	const RoundRect detailButton = RoundRect(1480, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect stepButton = RoundRect(1380, 25, algo_vis::STEP_BUTTON_SIZE, 5);
	const Font detailBackButtonFont{ 40, Typeface::Bold };
	const Font stepButtonFont{ 45, Typeface::Bold };
	const Font barValueFont{ 38 ,Typeface::Bold };
	const Font algorithmDetailBoxFont{ 28 };
	const RoundRect algorithmDetailBox1 = RoundRect(150, 600, 1300, 250, 10);
	const Rect algorithmDetailBox2 = Rect(160, 610, 1280, 230);
	const String detailStrKey = U"bubbleSort.detailStr";
	String detailStr = U"";
	std::vector<int> vec{ 6, 5, 1, 3, 2, 4, 10, 7, 8, 9 };
	static const int vecSize = 10;
	const int barWidth = 100;
	const int barSpacing = 30;
	const int barBaseY = 580;
	const int totalBarWidth = vecSize * barWidth + (vecSize - 1) * barSpacing;
	const int startX = (1600 - totalBarWidth) / 2;
	int outerLoopIndex = 0;
	int innerLoopIndex = 0;
	bool isSwapping = false;
	bool flag1 = true;
	bool flagEnd = false;

public:
	bubbleSort(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"bubbleSort.str");
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

		for (int i = 0; i < vecSize; ++i) {
			int barHeight = vec[i] * 40 + 30;
			int barX = startX + i * (barWidth + barSpacing);
			Rect barRect{ barX, barBaseY - barHeight, barWidth, barHeight };
			if (i == innerLoopIndex || i == innerLoopIndex + 1) {
				barRect.draw(Palette::Yellow);
			}
			else {
				barRect.draw(Palette::Skyblue);
			}
			barRect.drawFrame(3, 0, Palette::Black);
			int textY = barBaseY - 30;
			barValueFont(vec[i]).draw(Arg::center(barRect.center().x, textY), Palette::Black);
		}

	}

	void algo() {
		if (!flagEnd) {
			if (innerLoopIndex < vecSize - outerLoopIndex - 1) {
				detailStr = U"配列の" + Format(innerLoopIndex + 1) + U"番目("
					+ Format(vec[innerLoopIndex]) + U")と"
					+ Format(innerLoopIndex + 2) + U"番目("
					+ Format(vec[innerLoopIndex + 1]) + U")を比較します。\n";

				if (vec[innerLoopIndex] > vec[innerLoopIndex + 1]) {
					std::swap(vec[innerLoopIndex], vec[innerLoopIndex + 1]);
					detailStr += Format(vec[innerLoopIndex + 1]) + U"より"
						+ Format(vec[innerLoopIndex]) + U"の方が大きかったため、交換しました。\n";
				}
				else {
					detailStr += U"交換の必要はありませんでした。";
				}
				innerLoopIndex++;
			}
			else {
				outerLoopIndex++;
				innerLoopIndex = 0;
				detailStr = U"配列の末尾から" + Format(outerLoopIndex) + U"番目までの要素がソート完了です。\n";
			}
			if (outerLoopIndex >= vecSize - 1) {
				detailStr = U"全ての要素のソートが完了しました。";
				flagEnd = true;
			}
		}
	}

	void randomVec() {
		for (int i = 0; i < vecSize; ++i) {
			int j = Random(i, vecSize - 1);
			std::swap(vec[i], vec[j]);
		}
	}
};
