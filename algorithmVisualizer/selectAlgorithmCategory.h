# pragma once
# include "algorithmIntroduction.h"
# include "selectAndExplainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;
class selectAlgorithmCategory : public AppScene::Scene {
private:
	std::vector<RoundRect>buttons;
	const Font buttonTitleFont{ 115,Typeface::Bold };
	const Font titleFont{ 85,Typeface::Bold };
	std::vector<String>titles = { U"探索" ,U"数学" ,U"グラフ",U"ソート",U"その他" };

public:
	selectAlgorithmCategory(const InitData& init) : IScene(init) {
		for (int i = 0; i < 5; i++) {
			buttons.push_back({ 100 + 500 * (i % 3),220 + (280 * (i / 3)) ,400,180,10 });
		}

	}
	// 更新関数
	void update() override {
		for (int i = 0; i < 5; i++) {
			if (buttons[i].leftClicked()) {
				selectAndExplainAlgorithm::changeCategory(i);
				changeScene(U"selectAndExplainAlgorithm", 500);
			}
		}
	}
	void draw() const override {
		Scene::SetBackground(ColorF{ 0.7, 0.9, 0.8 });
		for (int i = 0; i < 5; i++) {
			if (buttons[i].mouseOver()) {
				buttons[i].draw(Palette::Royalblue);
				buttons[i].drawFrame(5.0f, Palette::Slategrey);
			}
			else {
				buttons[i].draw(Palette::Steelblue);
			}
			buttonTitleFont(titles[i]).draw(Arg::center(buttons[i].x + 200, buttons[i].y + 90), ColorF{ 0.1 });
		}
		titleFont(U"アルゴリズムの種類を選択してください").draw(Arg::center(800, 100), Palette::Forestgreen);
	}
};
