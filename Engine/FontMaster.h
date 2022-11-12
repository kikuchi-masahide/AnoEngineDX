//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "unicode/ustring.h"
#include "DX12/GraphicsCommandList.h"
#include "DX12/CommandQueue.h"
#include "DX12/Fence.h"
#include "DX12/Texture2D.h"
#include "Math/MatVec.h"

namespace DX12 {
	class Master;
}

/// <summary>
/// .ftmファイルを読み込み、フォント情報蓄積や描画を行う
/// </summary>
class FontMaster :public boost::noncopyable{
public:
	//Gameのコンストラクタで呼び出される
	void Init(DX12::Master* dx12);
	//指定ファイルからデータを同期的に読み込む
	void LoadFTMFile(std::string filename);
	//指定ファイルから読み込んだデータを解放する
	void ReleaseFTMFile(std::string filename);
	//GetDrawInfoForで出力される、文字出力の位置情報
	struct CharDrawInfo {
	public:
		CharDrawInfo(DX12::Texture2D bitmap, double leftx, double bottomy, double w, double h)
			:bitmap_(bitmap), left_x_(leftx), bottom_y_(bottomy), width_(w), height_(h) {};
		DX12::Texture2D bitmap_;
		//bitmap_を表示する領域の左端x座標(文字列全体の位置の左下を原点とする)
		double left_x_;
		//bitmap_を表示する領域の下端y座標(文字列全体の位置の左下を原点とする)
		double bottom_y_;
		//bitmap_を表示する領域の横幅
		double width_;
		//bitmap_を表示する領域の縦幅
		double height_;
	};
	/// <summary>
	/// 与えられたutf8文字列を縦幅heightで一行に出力するとき、どのようにbitmapを出力するかを表すvectorをセットする
	/// (n文字目はoutput[n]に従い表示する)
	/// </summary>
	/// <param name="output">元の内容はclearされることに注意</param>
	/// <returns>文字列全体の描画に必要な横幅</returns>
	double GetDrawInfoFor(const char8_t* utf8, int fontid, double height,std::vector<CharDrawInfo>& output);
private:
	//1つの文字に関する情報
	struct CharInfo {
	public:
		CharInfo(int gmbx,int gmby,int bmptx,int bmpty,int gmcx,DX12::Texture2D b)
			:gmBlackBoxX_(gmbx),gmBlackBoxY_(gmby),bmptGlyphOriginX_(bmptx),bmptGlyphOriginY_(bmpty),
			gmCellIncX_(gmcx),bitmap_(b){}
		int gmBlackBoxX_;
		int gmBlackBoxY_;
		int bmptGlyphOriginX_;
		int bmptGlyphOriginY_;
		int gmCellIncX_;
		DX12::Texture2D bitmap_;
	};
	//特定フォントの、一つのサイズについての情報
	struct SizeInfo {
	public:
		SizeInfo(int tma, int tmd)
			:tmAscent_(tma),tmDescent_(tmd){};
		int tmAscent_;
		int tmDescent_;
		std::map<UChar32, CharInfo> bitmaps_;
	};
	//1つのファイルに含まれる情報の管理
	struct FileInfo {
	public:
		//キーはフォントID
		//HACK:SizeInfoをサイズ降順で並べる
		std::map<int, std::vector<SizeInfo>> fonts_;
	};
	//キーはファイル名
	std::map<std::string, FileInfo> files_;
	boost::mutex files_mutex_;
	DX12::Master* dx12_;
	//リソース転送用のオブジェクト
	DX12::CommandQueue upload_cmd_queue_;
	DX12::GraphicsCommandList upload_cmd_list_;
	DX12::Fence upload_fence_;
};