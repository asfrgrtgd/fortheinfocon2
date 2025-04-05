#pragma once
#include "algorithmVisualizerDefs.h"
#include "algorithmIntroduction.h"
#include "explainAlgorithm.h"

using AppScene = SceneManager<String, algorithmIntroduction>;

class hillClimbing : public AppScene::Scene {
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
	bool flag1 = true;
	bool flagEnd = false;
	int nowStep = 0;
	const String detailStr1Key = U"hillClimbing.detailStr";
	double a = 0.11, b = 0.09, c = -3.30, d = 0.00;
	double startnum = 0.00;
	double currentnum = 0.00;
	double dotX = 0.00;
	double maxStep = 10.00;
	RectF graphArea = { 450, 60, 700, 500 }; 
	const double xMin = -10.00, xMax = 10.00;
	const double yMin = -10.00, yMax = 10.00;
	String textBoxStr = U"";

	Array<double> stepSizes = { 0.1, -0.1, 0.40, -0.40 };

	enum StepPhase {
		DisplayNeighbors,
		MoveToBestNeighbor
	};
	StepPhase stepPhase = DisplayNeighbors;

public:
	hillClimbing(const InitData& init) : IScene(init) {
		getData().str = algo_vis::SearchINIDataFromKey(getData().data_ini, U"hillClimbing.str");
		detailStr = algo_vis::SearchINIDataFromKey(getData().data_ini, detailStr1Key);
	}

	void update() override {
		if (stepButton.leftClicked() && !flagEnd) {
			if (nowStep == 0) {
				currentnum = startnum;
			}
			algo();
		}

		if (backButton.leftClicked()) {
			changeScene(U"selectAndExplainAlgorithm", 500);
		}

		if (detailButton.leftClicked()) {
			changeScene(U"explainAlgorithm", 500);
		}

		if (flag1) {
			SimpleGUI::Slider(U"a (x³): {:.2f}"_fmt(a), a, -1.00, 1.00, Vec2(10, 30), 180, 170);
			SimpleGUI::Slider(U"b (x²): {:.2f}"_fmt(b), b, -1.00, 1.00, Vec2(10, 80), 180, 170);
			SimpleGUI::Slider(U"c (x): {:.2f}"_fmt(c), c, -5.00, 5.00, Vec2(10, 130), 180, 170);
			SimpleGUI::Slider(U"d (定数): {:.2f}"_fmt(d), d, -10.00, 10.00, Vec2(10, 180), 180, 170);
			SimpleGUI::Slider(U"初期値: {:.2f}"_fmt(startnum), startnum, -10.00, 10.00, Vec2(10, 230), 180, 170);
			SimpleGUI::Slider(U"最大探索回数: {:.0f}"_fmt(maxStep), maxStep, 1.00, 30.00, Vec2(10, 280), 180, 170);
		}
		else {
			(void)SimpleGUI::Slider(U"a (x³): {:.2f}"_fmt(a), a, -1.00, 1.00, Vec2(10, 30), 180, 170, false);
			(void)SimpleGUI::Slider(U"b (x²): {:.2f}"_fmt(b), b, -1.00, 1.00, Vec2(10, 80), 180, 170, false);
			(void)SimpleGUI::Slider(U"c (x): {:.2f}"_fmt(c), c, -5.00, 5.00, Vec2(10, 130), 180, 170, false);
			(void)SimpleGUI::Slider(U"d (定数): {:.2f}"_fmt(d), d, -10.00, 10.00, Vec2(10, 180), 180, 170, false);
			(void)SimpleGUI::Slider(U"初期値: {:.2f}"_fmt(startnum), startnum, -10.00, 10.00, Vec2(10, 230), 180, 170, false);
			(void)SimpleGUI::Slider(U"最大探索回数: {:.0f}"_fmt(maxStep), maxStep, 1.00, 30.00, Vec2(10, 280), 180, 170, false);
		}

		if (flag1) {
			dotX = startnum;
		}
		else {
			dotX = currentnum;
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

		graphArea.drawFrame(2.0, Palette::Black);
		graphArea.stretched(-1).draw(Palette::White);

		Line(graphArea.x + graphArea.w / 2, graphArea.y,
			 graphArea.x + graphArea.w / 2, graphArea.y + graphArea.h).draw(1, Palette::Black);
		Line(graphArea.x, graphArea.y + graphArea.h / 2,
			 graphArea.x + graphArea.w, graphArea.y + graphArea.h / 2).draw(1, Palette::Black);

		for (double x = xMin; x < xMax; x += 0.01) {
			double y = a * Math::Pow(x, 3) + b * Math::Pow(x, 2) + c * x + d;
			double nextX = x + 0.01;
			double nextY = a * Math::Pow(nextX, 3) + b * Math::Pow(nextX, 2) + c * nextX + d;

			if (x >= xMin && x <= xMax && nextX >= xMin && nextX <= xMax &&
				y >= yMin && y <= yMax && nextY >= yMin && nextY <= yMax) {

				double screenX1 = graphArea.x + (x - xMin) / (xMax - xMin) * graphArea.w;
				double screenY1 = graphArea.y + graphArea.h - (y - yMin) / (yMax - yMin) * graphArea.h;

				double screenX2 = graphArea.x + (nextX - xMin) / (xMax - xMin) * graphArea.w;
				double screenY2 = graphArea.y + graphArea.h - (nextY - yMin) / (yMax - yMin) * graphArea.h;

				Line(screenX1, screenY1, screenX2, screenY2).draw(2, Palette::Blue);
			}
		}

		FontAsset(U"LabelFont")(U"係数調整スライダー").drawAt(Vec2(290, 0), Palette::Black);

		double y = a * Math::Pow(dotX, 3) + b * Math::Pow(dotX, 2) + c * dotX + d;
		double screenX = graphArea.x + (dotX - xMin) / (xMax - xMin) * graphArea.w;
		double screenY = graphArea.y + graphArea.h - (y - yMin) / (yMax - yMin) * graphArea.h;
		Circle(screenX, screenY, 5).draw(Palette::Red);
	}

	Array<double> getNeighbors(double current) {
		Array<double> neighbors;
		for (auto step : stepSizes) {
			double neighbor = current + step;
			if (neighbor >= xMin && neighbor <= xMax) {
				double neighborY = a * Math::Pow(neighbor, 3) + b * Math::Pow(neighbor, 2) + c * neighbor + d;
				if (neighborY >= yMin && neighborY <= yMax) {
					neighbors.push_back(neighbor);
				}
			}
		}
		return neighbors;
	}

	void algo() {
		if (flagEnd) return;

		double currentY = a * Math::Pow(currentnum, 3) + b * Math::Pow(currentnum, 2) + c * currentnum + d;

		if (stepPhase == DisplayNeighbors) {

			Array<double> neighbors = getNeighbors(currentnum);

			double bestY = currentY;
			double bestX = currentnum;
			bool hasBetterNeighbor = false;

			for (auto neighbor : neighbors) {
				double y = a * Math::Pow(neighbor, 3) + b * Math::Pow(neighbor, 2) + c * neighbor + d;
				if (y > bestY) {
					bestY = y;
					bestX = neighbor;
					hasBetterNeighbor = true;
				}
			}

			detailStr = U"ステップ " + ToString(nowStep + 1) + U": 現在の位置 x = " + U"{:.2f}"_fmt(currentnum) + U", y = " + U"{:.2f}"_fmt(currentY) + U"\n近傍の評価:\n";

			for (auto neighbor : neighbors) {
				double y = a * Math::Pow(neighbor, 3) + b * Math::Pow(neighbor, 2) + c * neighbor + d;
				detailStr += U"  x = " + U"{:.2f}"_fmt(neighbor) + U" => y = " + U"{:.2f}"_fmt(y) + U"\n";
			}

			if (hasBetterNeighbor) {
				detailStr += U"ステップボタンを押して最大の近傍に移動します。";
				stepPhase = MoveToBestNeighbor;
			}
			else {
				flagEnd = true;
				detailStr += U"これ以上改善できないため、アルゴリズムを終了します。";
			}
		}
		else if (stepPhase == MoveToBestNeighbor) {
			Array<double> neighbors = getNeighbors(currentnum);

			double bestY = currentY;
			double bestX = currentnum;

			for (auto neighbor : neighbors) {
				double y = a * Math::Pow(neighbor, 3) + b * Math::Pow(neighbor, 2) + c * neighbor + d;

				if (y > bestY) {
					bestY = y;
					bestX = neighbor;
				}
			}

			if (bestX != currentnum) {
				detailStr = U"最大の近傍は x = " + U"{:.2f}"_fmt(bestX) + U", y = " + U"{:.2f}"_fmt(bestY) + U" です。\n最大の近傍に移動しました。";
				currentnum = bestX;
				nowStep++;

				if (nowStep >= static_cast<int>(maxStep)) {
					flagEnd = true;
					detailStr += U"\n最大ステップ数に達しました。アルゴリズムを終了します。";
				}
			}
			else {
				flagEnd = true;
				detailStr = U"これ以上改善できないため、アルゴリズムを終了します。";
			}

			stepPhase = DisplayNeighbors;

			flag1 = false;
		}
	}

};
