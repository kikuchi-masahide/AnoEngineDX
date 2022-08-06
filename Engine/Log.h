//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace quill {
	class Logger;
}

//�p�t�H�[�}���X�v���t�@�C���̍ۂ́A1frame�ɂ�����L�^�f�[�^
struct ProfileUnit;

/// <summary>
/// Log.txt�Ƀ��O�����
/// </summary>
class Log {
public:
	static void Init();
	static void CleanUp();
	static void OutputTrivial(const std::string& str, std::source_location loc = std::source_location::current());
	static void OutputCritical(const std::string& str, std::source_location loc = std::source_location::current());
	static void OutputTrivial(const char str[], std::source_location loc = std::source_location::current());
	static void OutputCritical(const char str[], std::source_location loc = std::source_location::current());
	/// <summary>
	/// ����frame���܂߂�duration frame�̃p�t�H�[�}���X�L�^���J�n����
	/// ���ɊJ�n����Ă����ꍇ�Aduration�������Ԃ����������
	/// </summary>
	/// <param name="duration"></param>
	static void StartPerformanceProfile(int duration);
	/// <summary>
	/// ���̏����L�^���ALog�����̃J�E���^��i�߂�(�v���t�@�C���̗L���ɂ�炸Game���疈frame�Ăяo��)
	/// </summary>
	static void UpdateDatas(int update, int output, int obj, int upcomp, int outcomp);
private:
	//JSON�����N�����X���b�h����Ă΂��֐�
	static void ThreadFunc(int filenum, std::vector<ProfileUnit> datas);
	static int counter_;
	static int profile_limit_;
	static std::vector<ProfileUnit> profile_units_;
	//�L�^�I�������v���t�@�C�����O�̐��AJSON�ŏ����N���������������v���t�@�C���̐�
	static int finished_profiling_;
	static int jsoned_profiling_;
	static std::vector<std::string> log_buffer_;
	static boost::mutex log_buffer_mutex_;
};
