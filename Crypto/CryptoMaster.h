//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include <string>

class CryptoMaster {
public:
	/// <summary>
	/// データを暗号化しバイナリファイルに書き込む
	/// </summary>
	/// <param name="src">暗号化するファイル名(要拡張子)</param>
	/// <param name="dst">出力ファイル名(要拡張子)</param>
	static void Encrypt(std::string src, std::string dst);
private:
	//データをこの長さに区切って変換する
	static constexpr size_t process_section_size = 256;
	static constexpr unsigned char key0[16] = "AnoEngineDXkey";
	static constexpr unsigned char iv0[8] = "AnoEgIV";
};