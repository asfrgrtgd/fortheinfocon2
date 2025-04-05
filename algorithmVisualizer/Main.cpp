# include "stdafx.h"

# include "algorithmVisualizerDefs.h"
# include "selectAlgorithmCategory.h"
# include "selectAndExplainAlgorithm.h"
# include "explainAlgorithm.h"
# include "runAlgorithms/linearSearch.h"
# include "runAlgorithms/binarySearch.h"
# include "runAlgorithms/breadthFirstSearch.h"
# include "runAlgorithms/depthFirstSearch.h"
# include "runAlgorithms/euclideanAlgorithm.h"
# include "runAlgorithms/sieveOfEratosthenes.h"
# include "runAlgorithms/cumulativeSum.h"
# include "runAlgorithms/breadthFirstSearchGraph.h"
# include "runAlgorithms/depthFirstSearchGraph.h"
# include "runAlgorithms/dijkstra.h"
# include "runAlgorithms/topologicalSort.h"
# include "runAlgorithms/selectionSort.h"
# include "runAlgorithms/bubbleSort.h"
# include "runAlgorithms/quickSort.h"
# include "runAlgorithms/hillClimbing.h"
# include "runAlgorithms/simulatedAnnealing.h"
# include "runAlgorithms/monteCarlo.h"

constexpr int WINDOW_W = 1600;
constexpr int WINDOW_H = 900;

bool LoadINIFile(INI& ini_data) {
	// すでにデータ読み込み済みならリロードする
	if (ini_data) {
		ini_data.clear();
		ini_data.load(U"config.ini");
		return true;
	}

	ini_data.load(U"config.ini");

	if (not ini_data) {
		throw Error{ U"Failed to load `config.ini`" };
	}

	return true;
}

void Main() {
	Window::Resize(WINDOW_W, WINDOW_H);
	Window::SetStyle(WindowStyle::Fixed);
	SceneManager<String, algorithmIntroduction>manager;

	if (manager.get()) {
		LoadINIFile(manager.get()->data_ini);
	}

	manager.add<selectAlgorithmCategory>(U"selectAlgorithmCategory");
	manager.add<selectAndExplainAlgorithm>(U"selectAndExplainAlgorithm");
	manager.add<explainAlgorithm>(U"explainAlgorithm");
	manager.add<linearSearch>(U"linearSearch");
	manager.add<binarySearch>(U"binarySearch");
	manager.add<breadthFirstSearch>(U"breadthFirstSearch");
	manager.add<depthFirstSearch>(U"depthFirstSearch");
	manager.add<euclideanAlgorithm>(U"euclideanAlgorithm");
	manager.add<sieveOfEratosthenes>(U"sieveOfEratosthenes");
	manager.add<cumulativeSum>(U"cumulativeSum");
	manager.add<breadthFirstSearchGraph>(U"breadthFirstSearchGraph");
	manager.add<depthFirstSearchGraph>(U"depthFirstSearchGraph");
	manager.add<dijkstra>(U"dijkstra");
	manager.add<topologicalSort>(U"topologicalSort");
	manager.add<selectionSort>(U"selectionSort");
	manager.add<bubbleSort>(U"bubbleSort");
	manager.add<quickSort>(U"quickSort");
	manager.add<hillClimbing>(U"hillClimbing");
	manager.add<simulatedAnnealing>(U"simulatedAnnealing");
	manager.add<monteCarlo>(U"monteCarlo");


	while ((System::Update())) {
		if (!manager.update())break;
#if defined(ENABLE_DEVELOP_MODE)
		if (KeyR.down() && manager.get()) {
			if (LoadINIFile(manager.get()->data_ini)) {
				Print << U"文字列データのリロード";
			}
		}
#endif
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
