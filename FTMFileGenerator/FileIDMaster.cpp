//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "FileIDMaster.h"

#include <iostream>
#include <fstream>
#include "rapidjson/istreamwrapper.h"
#include "unicode/ustring.h"

void FileIDMaster::InitiateFontID()
{
	std::cout << "start InitiateFontID()\n";
	std::ifstream ifs("fontID.json");
	rapidjson::IStreamWrapper isw(ifs);
	doc_.ParseStream(isw);
	for (size_t i = 0; i < doc_["fonts"].Size(); i++) {
		auto& obj = doc_["fonts"][i];
		int fontid = obj["fontID"].GetInt();
		auto itr = fontidmap_.find(fontid);
		if (itr != fontidmap_.end()) {
			std::cout << "fileid duplicate\n";
			throw 0;
		}
		//フォント文字列(UTF-8)
		auto fontname = obj["font"].GetString();
		//UTF-8 => UTF-16
		int32_t utf16capacity = 0;
		UErrorCode errorcode = U_ZERO_ERROR;
		//errorcodeを渡さないと止まるのでとりあえず渡す
		u_strFromUTF8(NULL, 0, &utf16capacity, fontname, -1, &errorcode);
		//帰ってくるutf16capacityは文字分ギリギリのサイズで、末尾NULLが入っていない
		utf16capacity++;
		UChar* utf16 = new UChar[utf16capacity];
		//前のu_strFromUTF8で絶対U_BUFFER_OVERFLOWが返るけど、これをU_ZERO_ERRORにしないと何故か変換してくれない
		errorcode = U_ZERO_ERROR;
		u_strFromUTF8(utf16, utf16capacity, NULL, fontname, -1, &errorcode);
		//UTF-16 => wchar_t
		int32_t wchartcapacity = 0;
		u_strToWCS(NULL, 0, &wchartcapacity, utf16, utf16capacity, &errorcode);
		wchartcapacity++;
		wchar_t* wchart = new wchar_t[wchartcapacity];
		errorcode = U_ZERO_ERROR;
		u_strToWCS(wchart, wchartcapacity, NULL, utf16, utf16capacity, &errorcode);
		fontidmap_.emplace(fontid, std::wstring(wchart));
		delete[] utf16;
		delete[] wchart;
	}
	std::cout << "finished InitiateFontID()\n";
}

std::wstring FileIDMaster::GetFontName(int fontid)
{
	auto itr = fontidmap_.find(fontid);
	if (itr == fontidmap_.end()) {
		std::cout << "fontid " << fontid << " is not registered\n";
		return L"";
	}
	return itr->second;
}

rapidjson::Document FileIDMaster::doc_;
std::map<int, std::wstring> FileIDMaster::fontidmap_;