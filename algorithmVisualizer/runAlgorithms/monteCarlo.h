#pragma once
#include "algorithmVisualizerDefs.h"
#include "algorithmIntroduction.h"
#include "explainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;

class monteCarlo : public AppScene::Scene {
private:
	const RoundRect detailButton = RoundRect(1480, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect stepButton = RoundRect(1380, 25, algo_vis::STEP_BUTTON_SIZE, 5);
	const Font detailBackButtonFont{ 40, Typeface::Bold };
	const Font stepButtonFont{ 45, Typeface::Bold };
	const RoundRect algorithmDetailBox1 = RoundRect(150, 600, 1300, 250, 10);
	const Rect algorithmDetailBox2 = Rect(160, 610, 1280, 230);
	const Font algorithmDetailBoxFont{ 32 };
	String detailStr = U"";
	bool flag1 = true;
	bool flagEnd = false;
	int nowStep = 0;
	const String detailStrKey = U"monteCarlo.detailStr";
	RectF area = RectF(550, 40, 500, 500);
	struct MonteCarloPoint {
		Vec2 pos;
		bool inside;
	};
	Array<MonteCarloPoint> points;
	int insideCount = 0;
	int totalCount = 0;
	double pointsPerStep = 100.0;

public:
	monteCarlo(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"monteCarlo.str");
		detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStrKey);
	}

	void update() override {
		if (stepButton.leftClicked() && !flagEnd) {
			algo();
		}
		if (backButton.leftClicked()) {
			changeScene(U"selectAndExplainAlgorithm", 500);
		}
		if (detailButton.leftClicked()) {
			changeScene(U"explainAlgorithm", 500);
		}

		SimpleGUI::Slider(U"点/ステップ: {:.0f}"_fmt(pointsPerStep), pointsPerStep, 1.0, 200.0, Vec2(10, 30), 180, 170);
	}

	void draw() const override {
		algorithmDetailBox1.draw(Palette::White);
		algorithmDetailBoxFont(detailStr).draw(algorithmDetailBox2, Palette::Black);

		backButton.draw(Palette::Dodgerblue);
		detailBackButtonFont(U"戻る").draw(Arg::center(backButton.center()), Palette::White);

		detailButton.draw(Palette::Orange);
		detailBackButtonFont(U"詳細").draw(Arg::center(detailButton.center()), Palette::White);

		stepButton.draw(Palette::Bisque);
		stepButtonFont(U"ステップ").draw(Arg::center(stepButton.center()), Palette::Black);

		area.drawFrame(2, Palette::Black);
		area.stretched(-1).draw(Palette::White);
		Circle(area.center(), area.w / 2).drawFrame(2, Palette::Black);


		for (auto& p : points) {
			if(p.inside) {
				Circle(p.pos, 2).draw(Palette::Skyblue);
			}
			else {
				Circle(p.pos, 2).draw(Palette::Red);
			}
		}
	}

	void algo() {
		for (int i = 0; i < static_cast<int>(pointsPerStep); i++) {
			double rx = Random(-1.0, 1.0);
			double ry = Random(-1.0, 1.0);
			bool inside = ((rx * rx) + (ry * ry) <= 1.0);

			if (inside) {
				insideCount++;
			}
			totalCount++;

			MonteCarloPoint pd;
			double sx = area.center().x + rx * (area.w / 2.0);
			double sy = area.center().y + ry * (area.h / 2.0);
			pd.pos = Vec2(sx, sy);
			pd.inside = inside;
			points << pd;
		}

		double piEst = 4.0 * insideCount / totalCount;
		nowStep++;
		detailStr = U"総点数: " + ToString(totalCount)
			+ U"\n円内の点数: " + ToString(insideCount)
			+ U"\n推定π: " + ToString(piEst, 6);

		flag1 = false;
	}
};
