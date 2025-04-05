#pragma once
#include "algorithmVisualizerDefs.h"
#include "algorithmIntroduction.h"
#include "explainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;

class quickSort : public AppScene::Scene {
private:
	const RoundRect detailButton = RoundRect(1480, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect stepButton = RoundRect(1380, 25, algo_vis::STEP_BUTTON_SIZE, 5);
	const Font detailBackButtonFont{ 40, Typeface::Bold };
	const Font stepButtonFont{ 45, Typeface::Bold };
	const Font barValueFont{ 38, Typeface::Bold };
	const Font algorithmDetailBoxFont{ 28 };
	const RoundRect algorithmDetailBox1 = RoundRect(150, 600, 1300, 250, 10);
	const Rect algorithmDetailBox2 = Rect(160, 610, 1280, 230);
	const String detailStrKey = U"quickSort.detailStr";
	String detailStr = U"";
	std::vector<int> vec{ 5, 1, 4, 2, 3, 7, 8, 10, 6, 9 };
	const int vecSize = 10;
	Array<std::pair<int, int>> subranges;
	bool isSelectPivotStep = true;
	int currentLeft = -1;
	int currentRight = -1;
	int pivotIndex = -1;
	int pivotValue = -1;
	bool flagEnd = false;
	bool flagShuffleActive = true;
	const int barWidth = 100;
	const int barSpacing = 30;
	const int barBaseY = 580;
	const int totalBarWidth = 10 * barWidth + (10 - 1) * barSpacing;
	const int startX = (1600 - totalBarWidth) / 2;

public:
	quickSort(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"quickSort.str");
		detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStrKey);
		subranges.emplace_back(0, vecSize - 1);
	}

	void update() override {
		if (!flagEnd && stepButton.leftClicked()) {
			algo();
			flagShuffleActive = false;
		}
		if (flagShuffleActive) {
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

			if (currentLeft <= i && i <= currentRight) {
				if (i == pivotIndex) {
					barRect.draw(Palette::Red);
				}
				else {
					barRect.draw(Palette::Yellow);
				}
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
		if (isSelectPivotStep) {
			if (!subranges.isEmpty()) {
				auto [left, right] = subranges.back();
				subranges.pop_back();

				currentLeft = left;
				currentRight = right;
				pivotIndex = (left + right) / 2;
				pivotValue = vec[pivotIndex];

				detailStr = U"区間[" + Format(left + 1) + U"," + Format(right + 1) + U"]の中央をピボットに選びました。ピボットは位置"
					+ Format(pivotIndex + 1) + U"の値" + Format(pivotValue) + U"です。次のステップでこの値を基準に分割します。";

				isSelectPivotStep = false;
			}
			else {
				detailStr = U"ソートが既に完了しています。";
				flagEnd = true;
			}
		}
		else {
			if (currentLeft < currentRight) {
				if ((currentRight - currentLeft + 1) == 2) {
					if (vec[currentLeft] > vec[currentRight]) {
						std::swap(vec[currentLeft], vec[currentRight]);
					}
					detailStr = U"区間[" + Format(currentLeft + 1) + U"," + Format(currentRight + 1)
						+ U"]は要素が2つだけなので単純に比較し、必要なら交換しました。";
				}
				else {
					auto [i, j] = partitionByMidPivot(vec, currentLeft, currentRight);
					detailStr = U"区間[" + Format(currentLeft + 1) + U"," + Format(currentRight + 1)
						+ U"]をピボット" + Format(pivotValue) + U"で分割しました。"
						+ U"[" + Format(currentLeft + 1) + U"," + Format(j + 1)
						+ U"]と[" + Format(i + 1) + U"," + Format(currentRight + 1) + U"]を次に処理します。";

					if (i < currentRight) {
						subranges.emplace_back(i, currentRight);
					}
					if (currentLeft < j) {
						subranges.emplace_back(currentLeft, j);
					}
				}
			}
			else {
				detailStr = U"区間[" + Format(currentLeft + 1) + U"," + Format(currentRight + 1)
					+ U"]は要素数が1以下です。";
			}
			isSelectPivotStep = true;
			if (subranges.isEmpty() && isSelectPivotStep) {
				detailStr = U"クイックソートが完了しました。";
				flagEnd = true;
			}
		}
	}

	std::pair<int, int> partitionByMidPivot(std::vector<int>& arr, int left, int right) {
		int pivotVal = arr[(left + right) / 2];
		int i = left;
		int j = right;
		while (true) {
			while (i <= j && arr[i] < pivotVal) {
				i++;
			}
			while (i <= j && arr[j] > pivotVal) {
				j--;
			}
			if (i >= j) {
				break;
			}
			std::swap(arr[i], arr[j]);
			i++;
			j--;
		}
		return { i, j };
	}

	void randomVec() {
		for (int i = 0; i < vecSize; i++) {
			int j = Random(i, vecSize - 1);
			std::swap(vec[i], vec[j]);
		}
		detailStr = U"配列をランダムにシャッフルしました。";
	}
};
