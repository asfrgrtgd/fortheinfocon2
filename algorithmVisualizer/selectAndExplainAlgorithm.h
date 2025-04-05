# pragma once
# include "algorithmVisualizerDefs.h"
# include "algorithmIntroduction.h"

using AppScene = SceneManager<String, algorithmIntroduction>;
class selectAndExplainAlgorithm : public AppScene::Scene {
private:
	static inline int selectedAlgorithm = 0;
	static inline int selectedCategory = 0; // 0...探索 1...数学 2...グラフ 3...ソート 4...その他
	const std::vector<int>categorySize{ 4,3,4,3,3 };
	const std::vector<std::vector<String>>algorithmtitles{ {U"線形探索",U"二分探索",U"幅優先探索",U"深さ優先探索"},{U"ユークリッドの互除法",U"エラストテネスの篩",U"累積和"},{ U"幅優先探索", U"深さ優先探索", U"ダイクストラ法", U"トポロジカルソート"},{U"選択ソート", U"バブルソート", U"クイックソート"},{U"山登り法", U"焼きなまし法", U"モンテカルロ法"} };
	const std::vector<std::vector<String>> algorithmSceneNames{
		{ U"linearSearch", U"binarySearch", U"breadthFirstSearch", U"depthFirstSearch" },
		{ U"euclideanAlgorithm", U"sieveOfEratosthenes", U"cumulativeSum" },
		{ U"breadthFirstSearchGraph", U"depthFirstSearchGraph", U"dijkstra", U"topologicalSort" },
		{ U"selectionSort", U"bubbleSort", U"quickSort" },
		{ U"hillClimbing", U"simulatedAnnealing", U"monteCarlo" }
	};
	std::vector<std::vector<String>>algorirhtmDetails{};
	const std::vector<std::vector<String>>algorirhtmDetailsKey{ {U"selectAndExplainAlgorithm.algorithmDetails1",U"selectAndExplainAlgorithm.algorithmDetails2",U"selectAndExplainAlgorithm.algorithmDetails3",U"selectAndExplainAlgorithm.algorithmDetails4"},
		{U"selectAndExplainAlgorithm.algorithmDetails5", U"selectAndExplainAlgorithm.algorithmDetails6", U"selectAndExplainAlgorithm.algorithmDetails7"}, {U"selectAndExplainAlgorithm.algorithmDetails8", U"selectAndExplainAlgorithm.algorithmDetails9", U"selectAndExplainAlgorithm.algorithmDetails10", U"selectAndExplainAlgorithm.algorithmDetails11"}, {U"selectAndExplainAlgorithm.algorithmDetails12", U"selectAndExplainAlgorithm.algorithmDetails13", U"selectAndExplainAlgorithm.algorithmDetails14"}, {U"selectAndExplainAlgorithm.algorithmDetails15", U"selectAndExplainAlgorithm.algorithmDetails16", U"selectAndExplainAlgorithm.algorithmDetails17"} };
	const RoundRect runButton = RoundRect(1480, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect algorithmNameHeader = RoundRect(840, 10, 740, 103, 10);
	const RoundRect algorithmDetailBox1 = RoundRect(840, 120, 740, 675, 5);
	const Rect algorithmDetailBox2 = Rect(845, 125, 730, 665);
	const Font runBackButtonFont{ 40,Typeface::Bold };
	const Font algorithmTitleFont{ 65,Typeface::Bold };
	const Font algorithmNameHeaderFont{ 70,Typeface::Bold };
	const Font algorithmExplainFont{ 21 };
	std::vector<RoundRect>algorithmListButtons;
public:
	selectAndExplainAlgorithm(const InitData& init) : IScene(init) {
		// 文字列データの準備
		for (const auto& key_arr : algorirhtmDetailsKey) {
			algorirhtmDetails.emplace_back(std::vector<String>());
			for (const auto& key : key_arr) {
				algorirhtmDetails.back().emplace_back(algo_vis::SearchINIDataFromKey(getData().data_ini, key));
			}
		}

		for (int i = 0; i < categorySize[selectedCategory]; i++) {
			algorithmListButtons.push_back({ 115,40 + (130 * i),700,105,5 });
		}
	}
	// 更新関数
	void update() override {
		for (int i = 0; i < algorithmListButtons.size(); i++) {
			if (algorithmListButtons[i].mouseOver()) {
				selectedAlgorithm = i;
			}
			if (algorithmListButtons[i].leftClicked()) {
				// カテゴリーとアルゴリズムから特定してシーンを移動する、存在しない場合は無視する
				if ((selectedCategory < algorithmSceneNames.size()) && (selectedAlgorithm < algorithmSceneNames.at(selectedCategory).size())) {
					changeScene(algorithmSceneNames[selectedCategory][selectedAlgorithm], 500);
				}
			}
		}
		if (backButton.leftClicked()) {
			changeScene(U"selectAlgorithmCategory", 500);
			selectedAlgorithm = 0;
		}

	}
	void draw() const override {
		for (int i = 0; i < algorithmListButtons.size(); i++) {
			if (algorithmListButtons[i].mouseOver()) {
				algorithmListButtons[i].draw(Palette::Royalblue);
				algorithmListButtons[i].drawFrame(5.0f, Palette::Slategrey);
			}
			else {
				algorithmListButtons[i].draw(Palette::Steelblue);
			}
			algorithmTitleFont(algorithmtitles[selectedCategory][i]).draw(algorithmListButtons[i].x + 13, algorithmListButtons[i].y + 5, ColorF{ 0.1 });
		}

		algorithmNameHeader.draw(Palette::White);
		algorithmNameHeaderFont(algorithmtitles[selectedCategory][selectedAlgorithm]).drawAt({ algorithmNameHeader.x + 355, algorithmNameHeader.y + 50 }, ColorF{ 0.1 });

		algorithmDetailBox1.drawShadow({ 1,1 }, 10).draw(Palette::Lavender).drawFrame(2, ColorF{ 0.4 });
		algorithmDetailBox2.draw(Palette::Lavender);

		algorithmExplainFont(algorirhtmDetails[selectedCategory][selectedAlgorithm]).draw(algorithmDetailBox2, ColorF{ 0.25 });

		if (backButton.mouseOver()) {
			backButton.draw(Palette::Aqua);
		}
		else {
			backButton.draw(Palette::Dodgerblue);
		}
		runBackButtonFont(U"戻る").draw(Arg::center(backButton.x + 50, backButton.y + 35), ColorF{ 0.1 });

	}
	static void changeCategory(int num) {
		selectedCategory = num;
	}
};
