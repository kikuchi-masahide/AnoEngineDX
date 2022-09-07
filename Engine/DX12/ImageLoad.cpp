//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "ImageLoad.h"

#include "Buffer.h"

void DX12::LoadImageToScratchImage(LPCWSTR filename, DirectX::ScratchImage& img)
{
	DirectX::TexMetadata meta = {};
	auto result = DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, &meta, img);
	if (FAILED(result)) {
		throw 0;
	}
}

UINT64 DX12::GetAlignmentedRowPitch(int row_pitch)
{
	return row_pitch + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - row_pitch % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT;
}

void DX12::MapImageToBuffer(DirectX::ScratchImage& scratchimg, std::shared_ptr<Buffer> buffer)
{
	uint8_t* mapforimg = static_cast<uint8_t*>(buffer->Map());
	auto img = scratchimg.GetImage(0, 0, 0);
	auto srcaddress = img->pixels;
	auto dstrowpitch = GetAlignmentedRowPitch(img->rowPitch);
	//bufferのサイズの確認
	//HACK:例外飛ばす形にするか?
	assert(buffer->size_ >= dstrowpitch * img->height);
	for (int y = 0; y < img->height; y++) {
		std::copy_n(srcaddress, img->rowPitch, mapforimg);
		srcaddress += img->rowPitch;
		mapforimg += dstrowpitch;
	}
	buffer->Unmap();
}
