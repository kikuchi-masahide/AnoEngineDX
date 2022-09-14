//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "rapidjson/document.h"
#include <string>
#include <map>

class FileIDMaster {
public:
	static void InitiateFontID();
	static std::wstring GetFontName(int fontid);
private:
	static rapidjson::Document doc_;
	static std::map<int, std::wstring> fontidmap_;
};