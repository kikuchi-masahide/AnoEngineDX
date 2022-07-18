//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class Game;

/// <summary>
/// profillerのための処理クラス(なんかちょっと思ってたのと違う)
/// </summary>
//HACK:メモリ使用量、捕捉した任意の情報の表示などもできると嬉しいか?
//ただしそのためにはWindowクラス等、WindowsAPIまわりの扱い方も考えないとGUI面できつそう
class Profiller
{
public:
	Profiller(Game* game);
	void Init();
	~Profiller();
	//表示する情報を更新し再描画
	void UpdateInfo(DWORD update_time, DWORD output_time, int obj, int update_comp, int output_comp);
private:
	//profillerを使用する場合のWindowID
	static int constexpr kProfillerWindowID = 10000;
	Game* game_;
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	//WM_PAINTの時呼び出す
	static void ReactWMPaint(HWND hwnd);
	void SendWMPaint();
	//グラフの枠用のペン
	static HPEN graph_edge_pen_;
	HWND hwnd_;
	//UpdateInfoが呼び出された回数 初めての時0
	static int counter_;
	//保存している情報
	static std::array<std::array<int, 60>, 2> update_times_;
	static std::array<std::array<int, 60>, 2> output_times_;
	static std::array<std::array<int, 60>, 2> obj_nums_;
	static std::array<std::array<int, 60>, 2> update_comp_nums_;
	static std::array<std::array<int, 60>, 2> output_comp_nums_;
	//表示する際のグラフの下限/上限
	static int constexpr update_time_showmin_ = 0;
	static int constexpr update_time_showmax_ = 32;
	static int constexpr output_time_showmin_ = 0;
	static int constexpr output_time_showmax_ = 32;
	static int constexpr obj_num_showmin_ = 0;
	static int constexpr obj_num_showmax_ = 50;
	static int constexpr update_comp_num_showmin_ = 0;
	static int constexpr update_comp_num_showmax_ = 50;
	static int constexpr output_comp_num_showmin_ = 0;
	static int constexpr output_comp_num_showmax_ = 50;
	//それぞれの情報を表示する際に使うペン
	static HPEN update_time_pen_before_;
	static HPEN update_time_pen_after_;
	static HPEN output_time_pen_before_;
	static HPEN output_time_pen_after_;
	static HPEN obj_num_pen_before_;
	static HPEN obj_num_pen_after_;
	static HPEN update_comp_num_pen_before_;
	static HPEN update_comp_num_pen_after_;
	static HPEN output_comp_num_pen_before_;
	static HPEN output_comp_num_pen_after_;
};

