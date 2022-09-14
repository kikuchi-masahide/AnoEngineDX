//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

class CryptoMaster {
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="src">�f�[�^</param>
	/// <param name="length">�f�[�^�̒���(�����������f�[�^����������������)</param>
	/// <returns>�����������f�[�^</returns>
	static std::shared_ptr<unsigned char[]> Decrypt(std::shared_ptr<unsigned char[]> src, size_t length);
	/// <summary>
	/// �t�@�C���̓��e�𕜍�������
	/// </summary>
	/// <returns>�ǂݍ��񂾃f�[�^�ƃf�[�^���̃y�A</returns>
	static std::pair<std::shared_ptr<unsigned char[]>, size_t> Decrypt(std::string filename);
	/// <summary>
	/// �Í���
	/// </summary>
	/// <param name="src">�f�[�^</param>
	/// <param name="length">�f�[�^�̒���(�Í��������f�[�^����������������)</param>
	/// <returns>�Í��������f�[�^</returns>
	static std::shared_ptr<unsigned char[]> Encrypt(std::shared_ptr<unsigned char[]> src, size_t length);
	/// <summary>
	/// �f�[�^���Í������o�C�i���t�@�C���ɏ�������
	/// </summary>
	/// <param name="src">�Í�������f�[�^</param>
	/// <param name="length">�f�[�^�̃T�C�Y</param>
	/// <param name="filename">�o�̓t�@�C����(�v�g���q)</param>
	static void Encrypt(std::shared_ptr<unsigned char[]> src, size_t length, std::string filename);
private:
	//�f�[�^�����̒����ɋ�؂��ĕϊ�����
	static constexpr size_t process_section_size = 256;
	static constexpr unsigned char key0[16] = "AnoEngineDXkey";
	static constexpr unsigned char iv0[8] = "AnoEgIV";
};