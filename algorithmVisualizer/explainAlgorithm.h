#pragma once
# include "algorithmVisualizerDefs.h"
# include "algorithmIntroduction.h"
# include "selectAndExplainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;
class explainAlgorithm : public AppScene::Scene {
private:
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const Font backButtonFont{ 40,Typeface::Bold };
	const RoundRect algorithmExplainBox1 = RoundRect(60, 40, 1480, 720, 10);
	const Rect algorithmExplainBox2 = Rect(70, 50, 1460, 700);
	const Font explainFont{ 27 };
	String explainstr;

public:
	explainAlgorithm(const InitData& init) : IScene(init) {
		explainstr = getData().str;
	}
	void update() override {
		if (backButton.leftClicked()) {
			changeScene(U"selectAndExplainAlgorithm", 500);
		}
	}
	void draw() const override {
		backButton.draw(Palette::Dodgerblue);
		algorithmExplainBox1.draw(Palette::White);
		algorithmExplainBox2.draw(Palette::White);
		explainFont(explainstr).draw(algorithmExplainBox2, Palette::Black);
		backButtonFont(U"戻る").draw(Arg::center(backButton.x + 50, backButton.y + 35), ColorF{ 0.1 });
	}
};
