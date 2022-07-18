//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "Profiller.h"

#include "Game.h"
#include "Log.h"
#include "window.h"

Profiller::Profiller(Game* game)
	:game_(game)
{
}

void Profiller::Init()
{
	game_->AddWindow(WindowProcedure, TEXT("Profiller"), 400, 600, TEXT("Profiller"), kProfillerWindowID, false);
	hwnd_ = game_->GetWindow(kProfillerWindowID)->GetWindowHandle();
	graph_edge_pen_ = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	counter_ = -1;
	update_time_pen_after_ = CreatePen(PS_SOLID, 2, RGB(31, 59, 259));
	update_time_pen_before_ = CreatePen(PS_DASH, 1, RGB(31, 59, 259));
	output_time_pen_after_ = CreatePen(PS_SOLID, 2, RGB(209, 49, 13));
	output_time_pen_before_ = CreatePen(PS_DASH, 1, RGB(209, 49, 13));
	obj_num_pen_after_ = CreatePen(PS_SOLID, 2, RGB(30, 191, 27));
	obj_num_pen_before_ = CreatePen(PS_DASH, 1, RGB(30, 191, 27));
	update_comp_num_pen_after_ = CreatePen(PS_SOLID, 2, RGB(27, 180, 191));
	update_comp_num_pen_before_ = CreatePen(PS_DASH, 1, RGB(27, 180, 191));
	output_comp_num_pen_after_ = CreatePen(PS_SOLID, 2, RGB(251, 255, 20));
	output_comp_num_pen_before_ = CreatePen(PS_DASH, 1, RGB(251, 255, 20));
}

Profiller::~Profiller()
{
	DeleteObject(graph_edge_pen_);
	DeleteObject(update_time_pen_after_);
	DeleteObject(update_time_pen_before_);
	DeleteObject(output_time_pen_after_);
	DeleteObject(output_time_pen_before_);
	DeleteObject(obj_num_pen_after_);
	DeleteObject(obj_num_pen_before_);
	DeleteObject(update_comp_num_pen_after_);
	DeleteObject(update_comp_num_pen_before_);
	DeleteObject(output_comp_num_pen_after_);
	DeleteObject(output_comp_num_pen_before_);
}

void Profiller::UpdateInfo(DWORD update_time, DWORD output_time, int obj, int update_comp, int output_comp)
{
	counter_++;
	update_times_[(counter_ / 60) % 2][counter_ % 60] = update_time;
	output_times_[(counter_ / 60) % 2][counter_ % 60] = output_time;
	obj_nums_[(counter_ / 60) % 2][counter_ % 60] = obj;
	update_comp_nums_[(counter_ / 60) % 2][counter_ % 60] = update_comp;
	output_comp_nums_[(counter_ / 60) % 2][counter_ % 60] = output_comp;
	SendWMPaint();
}

void Profiller::SendWMPaint()
{
	RECT rect = { 0,0,400,600 };
	InvalidateRect(hwnd_, &rect, TRUE);
}

LRESULT Profiller::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_PAINT) {
		ReactWMPaint(hwnd);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Profiller::ReactWMPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);
	//グラフ表示用枠
	SelectObject(hdc, graph_edge_pen_);
	Rectangle(hdc, 25, 25, 400-25, 400-25);
	//表示する座標
	auto getx = [](int value) {
		return 50 + value * 5;
	};
	auto gety = [](int showmin, int showmax, int value) {
		if (value < showmin) {
			return 350;
		}
		if (value > showmax) {
			return 50;
		}
		return (350 * (showmax - value) + 50 * (value - showmin)) / (showmax - showmin);
	};
	//現在の情報を持つ配列
	int cur_arr_index = (counter_ / 60) % 2;
	//過去の情報を持つ配列
	int bef_arr_index = 1 - cur_arr_index;
	//各配列を表示する
	auto show = [&](std::array<std::array<int, 60>, 2> show_arr, int showmin, int showmax, 
		HPEN pen_bef,HPEN pen_cur) {
		SelectObject(hdc, pen_bef);
		MoveToEx(hdc, getx(0), gety(showmin, showmax, show_arr[bef_arr_index][0]), NULL);
		for (int t = 1; t < 60; t++) {
			LineTo(hdc, getx(t), gety(showmin, showmax, show_arr[bef_arr_index][t]));
		}
		SelectObject(hdc, pen_cur);
		MoveToEx(hdc, getx(0), gety(showmin, showmax, show_arr[cur_arr_index][0]), NULL);
		for (int t = 1; t <= counter_ % 60; t++) {
			LineTo(hdc, getx(t), gety(showmin, showmax, show_arr[cur_arr_index][t]));
		}
	};
	show(update_times_, update_time_showmin_, update_time_showmax_, update_time_pen_before_, update_time_pen_after_);
	show(output_times_, output_time_showmin_, output_time_showmax_, output_time_pen_before_, output_time_pen_after_);
	show(obj_nums_, obj_num_showmin_, obj_num_showmax_, obj_num_pen_before_, obj_num_pen_after_);
	show(update_comp_nums_, update_comp_num_showmin_, update_comp_num_showmax_, 
		update_comp_num_pen_before_, update_comp_num_pen_after_);
	show(output_comp_nums_, output_comp_num_showmin_, output_comp_num_showmax_,
		output_comp_num_pen_before_, output_comp_num_pen_after_);
	//どのペンが何を表すかの説明を追加
	auto desc = [&](LPCTSTR text, int textlen, int yind, HPEN pen) {
		SelectObject(hdc, pen);
		MoveToEx(hdc, 50, 400 + 20 + 40 * yind, NULL);
		LineTo(hdc, 150, 400 + 20 + 40 * yind);
		RECT rect = { 200,400 + 40 * yind,400,400 + 40 * (yind + 1) };
		DrawText(hdc, text, textlen, &rect, 0);
	};
	desc(TEXT("update_time"), 11, 0, update_time_pen_after_);
	desc(TEXT("output_time"), 11, 1, output_time_pen_after_);
	desc(TEXT("obj_num"), 7, 2, obj_num_pen_after_);
	desc(TEXT("update_comp_num"), 15, 3, update_comp_num_pen_after_);
	desc(TEXT("output_comp_num"), 15, 4, output_comp_num_pen_after_);
	EndPaint(hwnd, &ps);
}

std::array<std::array<int, 60>, 2> Profiller::update_times_;
std::array<std::array<int, 60>, 2> Profiller::output_times_;
std::array<std::array<int, 60>, 2> Profiller::obj_nums_;
std::array<std::array<int, 60>, 2> Profiller::update_comp_nums_;
std::array<std::array<int, 60>, 2> Profiller::output_comp_nums_;
HPEN Profiller::graph_edge_pen_;
HPEN Profiller::update_time_pen_before_;
HPEN Profiller::update_time_pen_after_;
HPEN Profiller::output_time_pen_before_;
HPEN Profiller::output_time_pen_after_;
HPEN Profiller::obj_num_pen_before_;
HPEN Profiller::obj_num_pen_after_;
HPEN Profiller::update_comp_num_pen_before_;
HPEN Profiller::update_comp_num_pen_after_;
HPEN Profiller::output_comp_num_pen_before_;
HPEN Profiller::output_comp_num_pen_after_;
int Profiller::counter_;
