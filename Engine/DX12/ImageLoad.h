//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	class Buffer;
	/// <summary>
	/// �摜�f�[�^��ǂݍ��݁Aimg�ɕۑ�����
	/// </summary>
	void LoadImageToScratchImage(LPCWSTR filename, DirectX::ScratchImage& img);
	/// <summary>
	/// �摜��1�����������[����ہA��s������̃T�C�Y�����o�C�g�ɃA���C�������g����Buffer���쐬����ׂ�����Ԃ�
	/// </summary>
	/// <param name="row_pitch">�摜�̈�s������̃f�[�^�T�C�Y</param>
	UINT64 GetAlignmentedRowPitch(int row_pitch);
	/// <summary>
	///  img�̃f�[�^���ꎟ������buffer�Ƀ}�b�v����
	/// </summary>
	void MapImageToBuffer(DirectX::ScratchImage& scratchimg, std::shared_ptr<Buffer> buffer);
}