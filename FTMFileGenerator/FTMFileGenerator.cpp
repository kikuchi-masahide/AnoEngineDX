//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "FTMFileGenerator.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <set>
#include <chrono>
#include <filesystem>
#include <Windows.h>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "unicode/ustring.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/salsa.h"
#include "FileIDMaster.h"

namespace Helper{
	//フォント名とサイズを受け取り、hdcに対応フォントをセットする
	void SetFontToHDC(std::wstring font, int size, HDC& hdc) {
		LOGFONT lf = {};
		lf.lfHeight = size;
		lf.lfWidth = 0;
		lf.lfEscapement = 0;
		lf.lfOrientation = 0;
		//TODO:文字の太さを変えられるようにする
		lf.lfWeight = FW_NORMAL;
		lf.lfItalic = 0;
		lf.lfUnderline = 0;
		lf.lfStrikeOut = 0;
		//TODO:Unicodeの文字なのにこれぐらいしか使えそうなのが無い?
		lf.lfCharSet = SHIFTJIS_CHARSET;
		lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		lf.lfQuality = PROOF_QUALITY;
		lf.lfPitchAndFamily = DEFAULT_PITCH | FF_MODERN;
		//フォント名は無理やりコピー
		std::copy_n(font.c_str(), font.size(), lf.lfFaceName);
		//lf.lfFaceName[font.size()] = NULL;
		HFONT hfont = CreateFontIndirectW(&lf);
		SelectObject(hdc, hfont);
	}
	struct TextMet {
		TextMet(int a, int d)
			:tmAscent(a), tmDescent(d) {};
		int tmAscent;
		int tmDescent;
	};
	//フォント名とサイズから、TextMetrics情報を得る
	TextMet GetTextMet(HDC& hdc) {
		TEXTMETRIC tm = {};
		::GetTextMetrics(hdc, &tm);
		return TextMet(tm.tmAscent, tm.tmDescent);
	}
	template<class T>
	void WriteAsBinary(std::ofstream& ofs, T val) {
		ofs.write((char*)&val, sizeof(T));
	}
	template<class T>
	void WriteAsBinary(std::ofstream& ofs, T* val, UINT size) {
		ofs.write((char*)val, sizeof(T) * size);
	}
	//utf8文字列から、その文字列に含まれるコードポイントをUChar32でsetに保存する
	void GetCodepointsFromUTF8(const char8_t* utf8, std::set<UChar32>& codepoints) {
		int32_t pos8 = 0;
		UChar32 out8;
		while (true) {
			if (utf8[pos8] == NULL) {
				break;
			}
			U8_NEXT(utf8, pos8, -1, out8);
			codepoints.insert(out8);
		}
	}
	//与えられたUnicodeコードポイントから、
	//-gmBlackBoxX,gmBlackBoxY
	//-bmptGlyphOriginX,bmptGlyphOriginY
	//-gmCellIncX
	//をintで、マッピングデータを各ピクセルcharで、ofsへ出力する。
	void OutputMappingFromCodepoint(UChar32 code, HDC& hdc, std::ofstream& ofs) {
		const MAT2 mat = { {0,1},{0,0},{0,0},{0,1} };
		GLYPHMETRICS gm = {};
		DWORD size = GetGlyphOutlineW(hdc, static_cast<UINT>(code), GGO_GRAY4_BITMAP, &gm, 0, NULL, &mat);
		BYTE* bmp = new BYTE[size];
		GetGlyphOutlineW(hdc, static_cast<UINT>(code), GGO_GRAY4_BITMAP, &gm, size, bmp, &mat);
		WriteAsBinary(ofs, (int)code);
		WriteAsBinary(ofs, (int)gm.gmBlackBoxX);
		WriteAsBinary(ofs, (int)gm.gmBlackBoxY);
		WriteAsBinary(ofs, (int)gm.gmptGlyphOrigin.x);
		WriteAsBinary(ofs, (int)gm.gmptGlyphOrigin.y);
		WriteAsBinary(ofs, (int)gm.gmCellIncX);
		int alignx = (gm.gmBlackBoxX + 3) / 4 * 4;
		for (int y = 0; y < gm.gmBlackBoxY; y++) {
			WriteAsBinary(ofs, bmp + alignx * y, gm.gmBlackBoxX);
		}
		delete[] bmp;
	}
	//ファイルbeforeがafterより前に更新されていればtrueを返す
	bool IsUpdatedEarlier(std::string before, std::string after) {
		namespace chrono = std::chrono;
		namespace fs = std::filesystem;
		auto sec_before = chrono::duration_cast<chrono::seconds>(fs::last_write_time(before).time_since_epoch());
		auto sec_after = chrono::duration_cast<chrono::seconds>(fs::last_write_time(after).time_since_epoch());
		return sec_before < sec_after;
	}
	//データをこのサイズずつに区切って変換する(ファイル全体を一気に変換しようとすると死ぬことがあるので)
	const size_t process_section_size = 256;
	//.oftmを読み込み.ftmを生成
	//(filename_bodyは、拡張子を除いたファイル名)
	void GenerateFtmFileFromOftm(std::string filename_body) {
		using namespace CryptoPP;
		unsigned char key0[16] = "AnoEngineDXkey";
		unsigned char iv0[8] = "AnoEgIV";
		unsigned char key1[16];
		unsigned char iv1[8];
		Salsa20::Encryption enc0;
		enc0.SetKeyWithIV(key0, 16, iv0, 8);
		enc0.ProcessData(key1, key0, 16);
		enc0.ProcessData(iv1, iv0, 8);
		std::ifstream ifs(filename_body + ".oftm", std::ios::binary);
		std::ofstream ftm;
		ftm.open(filename_body + ".ftm", std::ios::binary | std::ios::trunc);
		if (!ftm) {
			assert(false);
		}
		ifs.seekg(0, std::ios_base::end);
		size_t filesize = ifs.tellg();
		ifs.seekg(0, std::ios_base::beg);
		std::shared_ptr<unsigned char[]> original = std::make_shared<unsigned char[]>(filesize);
		ifs.read((char*)original.get(), filesize);
		std::shared_ptr<unsigned char[]> cipher = std::make_shared<unsigned char[]>(filesize);
		//今から変換するブロック
		unsigned char* cur = original.get();
		//今から書き込むブロック
		unsigned char* dst_pos = cipher.get();
		size_t processed = 0;
		while (processed < filesize) {
			size_t size = min(process_section_size, filesize - processed);
			Salsa20::Encryption enc1;
			enc1.SetKeyWithIV(key1, 16, iv1, 8);
			enc1.ProcessData(dst_pos, cur, size);
			cur += size;
			dst_pos += size;
			processed += size;
		}
		ftm.write((char*)cipher.get(), filesize);
		ftm.close();

		//逆変換してみる
		std::shared_ptr<unsigned char[]> rev = std::make_shared<unsigned char[]>(filesize);
		dst_pos = cipher.get();
		unsigned char* rev_pos = rev.get();
		processed = 0;
		while (processed < filesize) {
			size_t size = filesize - processed;
			if (size > process_section_size) {
				size = process_section_size;
			}
			Salsa20::Decryption dec;
			dec.SetKeyWithIV(key1, 16, iv1, 8);
			dec.ProcessData(rev_pos, dst_pos, size);
			rev_pos += size;
			dst_pos += size;
			processed += size;
		}
		for (int n = 0; n < filesize; n++) {
			if (rev[n] != original[n]) {
				std::cout << " " << "\n";
			}
		}

	}
}

void GenerateFTMFile(std::string filename)
{
	//HACK:フォーマットが違うなどの場合の例外処理を行う
	//.oftm.ftmを除外
	if (filename.substr(filename.size() - 5, 5) != ".json") {
		return;
	}
	//出力ファイル名
	std::string oftmname = filename.substr(0, filename.size() - 5);
	oftmname += ".oftm";
	std::string ftmname = filename.substr(0, filename.size() - 5);
	ftmname += ".ftm";
	//同名.ftmの検索
	if (std::filesystem::exists(ftmname)) {
		if (Helper::IsUpdatedEarlier(filename, ftmname)) {
			std::cout << "skipped " << filename << "\n";
			return;
		}
	}
	std::cout << "start generating " << ftmname << "\n";
	std::ofstream oftm;
	oftm.open(oftmname, std::ios::binary | std::ios::trunc);
	//.jsonの読み込み
	rapidjson::Document json;
	std::ifstream ifs(filename);
	rapidjson::IStreamWrapper isw(ifs);
	json.ParseStream(isw);
	auto& fonts = json["fonts"];
	Helper::WriteAsBinary(oftm, static_cast<int>(fonts.Size()));
	for (size_t i = 0; i < fonts.Size(); i++) {
		HDC hdc = GetDC(NULL);
		auto& font = fonts[i];
		auto fontid = font["fontid"].GetInt();
		auto size = font["size"].GetInt();
		Helper::SetFontToHDC(FileIDMaster::GetFontName(fontid), size, hdc);
		//TextMetrics情報
		auto textmetrics = Helper::GetTextMet(hdc);
		Helper::WriteAsBinary(oftm, fontid);
		Helper::WriteAsBinary(oftm, size);
		Helper::WriteAsBinary(oftm, textmetrics.tmAscent);
		Helper::WriteAsBinary(oftm, textmetrics.tmDescent);
		auto allcharsutf8 = (const char8_t*)font["chars"].GetString();
		//全codepointの重複なしのset
		std::set<UChar32> codepoints;
		Helper::GetCodepointsFromUTF8(allcharsutf8, codepoints);
		Helper::WriteAsBinary(oftm, static_cast<int>(codepoints.size()));
		//各文字のデータを出力
		for (UChar32 code : codepoints) {
			Helper::OutputMappingFromCodepoint(code, hdc, oftm);
		}
		ReleaseDC(NULL, hdc);
	}
	oftm.close();
	Helper::GenerateFtmFileFromOftm(filename.substr(0, filename.size() - 5));
	std::cout << "finished generating " << ftmname << "\n";
}
