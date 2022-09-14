//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include <string>

class CryptoMaster {
public:
	/// <summary>
	/// �f�[�^���Í������o�C�i���t�@�C���ɏ�������
	/// </summary>
	/// <param name="src">�Í�������t�@�C����(�v�g���q)</param>
	/// <param name="dst">�o�̓t�@�C����(�v�g���q)</param>
	static void Encrypt(std::string src, std::string dst);
private:
	//�f�[�^�����̒����ɋ�؂��ĕϊ�����
	static constexpr size_t process_section_size = 256;
	static constexpr unsigned char key0[16] = "AnoEngineDXkey";
	static constexpr unsigned char iv0[8] = "AnoEgIV";
};