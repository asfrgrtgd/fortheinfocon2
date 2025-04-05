#pragma once
#include "algorithmVisualizerDefs.h"
#include "algorithmIntroduction.h"
#include "explainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;

class simulatedAnnealing : public AppScene::Scene {
private:
	const RoundRect detailButton = RoundRect(1480, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect backButton = RoundRect(15, 805, algo_vis::MIDDLE_BUTTON_SIZE, 5);
	const RoundRect stepButton = RoundRect(1380, 25, algo_vis::STEP_BUTTON_SIZE, 5);
	const Font detailBackButtonFont{ 40, Typeface::Bold };
	const Font stepButtonFont{ 45, Typeface::Bold };

	const RoundRect algorithmDetailBox1 = RoundRect(150, 600, 1300, 250, 10);
	const Rect algorithmDetailBox2 = Rect(160, 610, 1280, 230);
	const Font algorithmDetailBoxFont{ 21 };
	String detailStr = U"";

	bool flagSliderActive = true;
	bool flagEnd = false;
	int nowStep = 0;
	const String detailStrKey = U"simulatedAnnealing.detailStr";

	double a = 0.11, b = 0.09, c = -3.30, d = 0.00;
	double startnum = 0.0;
	double currentnum = 0.0;
	double dotX = 0.0;
	double maxStep = 70.0;
	double temperatureStart = 15.0;
	double temperature = 15.0;
	double alpha = 0.90;

	RectF graphArea = { 450, 60, 700, 500 };
	const double xMin = -10.0, xMax = 10.0;
	const double yMin = -10.0, yMax = 10.0;

public:
	simulatedAnnealing(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"simulatedAnnealing.str");
		detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStrKey);
	}

	void update() override {
		if (stepButton.leftClicked() && !flagEnd) {
			if (nowStep == 0) {
				currentnum = startnum;
				temperature = temperatureStart;
			}
			algo();
		}

		if (backButton.leftClicked()) {
			changeScene(U"selectAndExplainAlgorithm", 500);
		}

		if (detailButton.leftClicked()) {
			changeScene(U"explainAlgorithm", 500);
		}

		if (flagSliderActive) {
			SimpleGUI::Slider(U"a (x³): {:.2f}"_fmt(a), a, -1.0, 1.0, Vec2(10, 30), 180, 170);
			SimpleGUI::Slider(U"b (x²): {:.2f}"_fmt(b), b, -1.0, 1.0, Vec2(10, 80), 180, 170);
			SimpleGUI::Slider(U"c (x): {:.2f}"_fmt(c), c, -5.0, 5.0, Vec2(10, 130), 180, 170);
			SimpleGUI::Slider(U"d (定数): {:.2f}"_fmt(d), d, -10.0, 10.0, Vec2(10, 180), 180, 170);
			SimpleGUI::Slider(U"初期値: {:.2f}"_fmt(startnum), startnum, -10.0, 10.0, Vec2(10, 230), 180, 170);
			SimpleGUI::Slider(U"最大探索回数: {:.0f}"_fmt(maxStep), maxStep, 1.0, 100.0, Vec2(10, 280), 180, 170);
			SimpleGUI::Slider(U"開始温度: {:.2f}"_fmt(temperatureStart), temperatureStart, 0.1, 50.0, Vec2(10, 330), 180, 170);
			SimpleGUI::Slider(U"減衰率 α: {:.2f}"_fmt(alpha), alpha, 0.80, 0.9999, Vec2(10, 380), 180, 170);
		}
		else {
			(void)SimpleGUI::Slider(U"a (x³): {:.2f}"_fmt(a), a, -1.0, 1.0, Vec2(10, 30), 180, 170, false);
			(void)SimpleGUI::Slider(U"b (x²): {:.2f}"_fmt(b), b, -1.0, 1.0, Vec2(10, 80), 180, 170, false);
			(void)SimpleGUI::Slider(U"c (x): {:.2f}"_fmt(c), c, -5.0, 5.0, Vec2(10, 130), 180, 170, false);
			(void)SimpleGUI::Slider(U"d (定数): {:.2f}"_fmt(d), d, -10.0, 10.0, Vec2(10, 180), 180, 170, false);
			(void)SimpleGUI::Slider(U"初期値: {:.2f}"_fmt(startnum), startnum, -10.0, 10.0, Vec2(10, 230), 180, 170, false);
			(void)SimpleGUI::Slider(U"最大探索回数: {:.0f}"_fmt(maxStep), maxStep, 1.0, 100.0, Vec2(10, 280), 180, 170, false);
			(void)SimpleGUI::Slider(U"開始温度: {:.2f}"_fmt(temperatureStart), temperatureStart, 0.1, 50.0, Vec2(10, 330), 180, 170, false);
			(void)SimpleGUI::Slider(U"減衰率 α: {:.2f}"_fmt(alpha), alpha, 0.80, 0.9999, Vec2(10, 380), 180, 170, false);
		}

		if (flagSliderActive) {
			dotX = startnum;
		}
		else {
			dotX = currentnum;
		}
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

		graphArea.drawFrame(2.0, Palette::Black);
		graphArea.stretched(-1).draw(Palette::White);

		Line(graphArea.x + graphArea.w / 2, graphArea.y,
			 graphArea.x + graphArea.w / 2, graphArea.y + graphArea.h)
			.draw(1, Palette::Black);
		Line(graphArea.x,
			 graphArea.y + graphArea.h / 2,
			 graphArea.x + graphArea.w,
			 graphArea.y + graphArea.h / 2)
			.draw(1, Palette::Black);

		for (double x = xMin; x < xMax; x += 0.01) {
			double y = f(x);
			double nx = x + 0.01;
			double ny = f(nx);

			if (x >= xMin && x <= xMax && nx >= xMin && nx <= xMax &&
				y >= yMin && y <= yMax && ny >= yMin && ny <= yMax) {
				double sx1 = graphArea.x + (x - xMin) / (xMax - xMin) * graphArea.w;
				double sy1 = graphArea.y + graphArea.h - (y - yMin) / (yMax - yMin) * graphArea.h;
				double sx2 = graphArea.x + (nx - xMin) / (xMax - xMin) * graphArea.w;
				double sy2 = graphArea.y + graphArea.h - (ny - yMin) / (yMax - yMin) * graphArea.h;
				Line(sx1, sy1, sx2, sy2).draw(2, Palette::Blue);
			}
		}

		double val = f(dotX);
		double sx = graphArea.x + (dotX - xMin) / (xMax - xMin) * graphArea.w;
		double sy = graphArea.y + graphArea.h - (val - yMin) / (yMax - yMin) * graphArea.h;
		Circle(sx, sy, 5).draw(Palette::Red);

		FontAsset(U"LabelFont")(U"Simulated Annealing 係数調整").drawAt(Vec2(300, 0), Palette::Black);
	}

	double f(double x) const {
		return (a * Math::Pow(x, 3) + b * Math::Pow(x, 2) + c * x + d);
	}

	double getRandomNeighbor(double current) const {
		double delta = Random(-1.5, 1.5);
		double candidate = current + delta;
		if (candidate < xMin)
		{
			candidate = xMin;
		}
		else if (candidate > xMax)
		{
			candidate = xMax;
		}
		return candidate;
	}

	void algo() {
		if (flagEnd) return;

		double currentVal = f(currentnum);
		double neighbor = getRandomNeighbor(currentnum);
		double neighborVal = f(neighbor);
		double deltaE = neighborVal - currentVal;

		detailStr.clear();
		detailStr += U"ステップ " + ToString(nowStep + 1) + U"\n";
		detailStr += U" 現在の x = {:.2f}, f(x) = {:.2f}\n"_fmt(currentnum, currentVal);
		detailStr += U" 候補の x' = {:.2f}, f(x') = {:.2f}\n"_fmt(neighbor, neighborVal);
		detailStr += U" 温度 T = {:.3f}\n"_fmt(temperature);

		bool accepted = false;
		if (deltaE > 0) {
			accepted = true;
			detailStr += U" → 改善解なので受け入れます。\n";
		}
		else {
			double prob = std::exp(deltaE / temperature);
			double r = Random();
			detailStr += U" → 悪化解。受け入れ確率 = {:.3f}, 抽選 = {:.3f}\n"_fmt(prob, r);

			if (r < prob) {
				accepted = true;
				detailStr += U" → 悪化解を受け入れました。\n";
			}
			else {
				detailStr += U" → 悪化解を棄却します。\n";
			}
		}

		if (accepted) {
			currentnum = neighbor;
		}

		temperature *= alpha;
		nowStep++;

		if (nowStep >= static_cast<int>(maxStep)) {
			flagEnd = true;
			detailStr += U"\n最大ステップ数に到達しました。探索を終了します。";
		}

		flagSliderActive = false;
	}
};
