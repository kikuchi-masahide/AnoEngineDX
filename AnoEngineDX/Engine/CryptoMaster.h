//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class CryptoMaster {
public:
	/// <summary>
	/// 復号化
	/// </summary>
	/// <param name="src">データ</param>
	/// <param name="length">データの長さ(複合化したデータも同じ長さを持つ)</param>
	/// <returns>複合化したデータ</returns>
	static std::shared_ptr<unsigned char[]> Decrypt(std::shared_ptr<unsigned char[]> src, size_t length);
	/// <summary>
	/// ファイルの内容を復号化する
	/// </summary>
	/// <returns>読み込んだデータとデータ長のペア</returns>
	static std::pair<std::shared_ptr<unsigned char[]>, size_t> Decrypt(std::string filename);
	/// <summary>
	/// 暗号化
	/// </summary>
	/// <param name="src">データ</param>
	/// <param name="length">データの長さ(暗号化したデータも同じ長さを持つ)</param>
	/// <returns>暗号化したデータ</returns>
	static std::shared_ptr<unsigned char[]> Encrypt(std::shared_ptr<unsigned char[]> src, size_t length);
	/// <summary>
	/// データを暗号化しバイナリファイルに書き込む
	/// </summary>
	/// <param name="src">暗号化するデータ</param>
	/// <param name="length">データのサイズ</param>
	/// <param name="filename">出力ファイル名(要拡張子)</param>
	static void Encrypt(std::shared_ptr<unsigned char[]> src, size_t length, std::string filename);
private:
	//データをこの長さに区切って変換する
	static constexpr size_t process_section_size = 256;
	static constexpr unsigned char key0[16] = "AnoEngineDXkey";
	static constexpr unsigned char iv0[8] = "AnoEgIV";
};