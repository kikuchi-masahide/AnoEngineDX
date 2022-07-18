//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class Game;

/// <summary>
/// profiller�̂��߂̏����N���X(�Ȃ񂩂�����Ǝv���Ă��̂ƈႤ)
/// </summary>
//HACK:�������g�p�ʁA�ߑ������C�ӂ̏��̕\���Ȃǂ��ł���Ɗ�������?
//���������̂��߂ɂ�Window�N���X���AWindowsAPI�܂��̈��������l���Ȃ���GUI�ʂł�����
class Profiller
{
public:
	Profiller(Game* game);
	void Init();
	~Profiller();
	//�\����������X�V���ĕ`��
	void UpdateInfo(DWORD update_time, DWORD output_time, int obj, int update_comp, int output_comp);
private:
	//profiller���g�p����ꍇ��WindowID
	static int constexpr kProfillerWindowID = 10000;
	Game* game_;
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	//WM_PAINT�̎��Ăяo��
	static void ReactWMPaint(HWND hwnd);
	void SendWMPaint();
	//�O���t�̘g�p�̃y��
	static HPEN graph_edge_pen_;
	HWND hwnd_;
	//UpdateInfo���Ăяo���ꂽ�� ���߂Ă̎�0
	static int counter_;
	//�ۑ����Ă�����
	static std::array<std::array<int, 60>, 2> update_times_;
	static std::array<std::array<int, 60>, 2> output_times_;
	static std::array<std::array<int, 60>, 2> obj_nums_;
	static std::array<std::array<int, 60>, 2> update_comp_nums_;
	static std::array<std::array<int, 60>, 2> output_comp_nums_;
	//�\������ۂ̃O���t�̉���/���
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
	//���ꂼ��̏���\������ۂɎg���y��
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

