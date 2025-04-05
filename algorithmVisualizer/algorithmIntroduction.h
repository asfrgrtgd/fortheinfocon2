#pragma once
#include "stdafx.h"
#include "algorithmIntroduction.h"

struct algorithmIntroduction {
	String str = U"";
	INI data_ini{};
};

namespace algo_vis {
	String SearchINIDataFromKey(INI& data_ini, String key);
	String ReplaceNewLineCode(String input_string);
}

String algo_vis::SearchINIDataFromKey(INI& data_ini, String key) {
#if defined(_DEBUG)
	return algo_vis::ReplaceNewLineCode(data_ini.getOr<String>(key, U"Not Found INI Data"));
#else
	return algo_vis::ReplaceNewLineCode(data_ini.getOr<String>(key, U""));
#endif
}

String algo_vis::ReplaceNewLineCode(String input_string) {
	input_string.replace(U"\\n", U"\n");

	return input_string;
}
