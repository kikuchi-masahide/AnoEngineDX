//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	class Buffer;
	/// <summary>
	/// 画像データを読み込み、imgに保存する
	/// </summary>
	void LoadImageToScratchImage(LPCWSTR filename, DirectX::ScratchImage& img);
	/// <summary>
	/// データから画像を読み込み、imgを保存する
	/// </summary>
	/// <param name="size">データのサイズ</param>
	void LoadImageToScratchImage(std::shared_ptr<unsigned char[]> data, size_t size,
		DirectX::ScratchImage& img);
	/// <summary>
	/// 画像を1次元化し収納する際、一行当たりのサイズを何バイトにアラインメントしてBufferを作成するべきかを返す
	/// </summary>
	/// <param name="row_pitch">画像の一行当たりのデータサイズ</param>
	UINT64 GetAlignmentedRowPitch(int row_pitch);
	/// <summary>
	///  imgのデータを一次元化しbufferにマップする
	/// </summary>
	void MapImageToBuffer(DirectX::ScratchImage& scratchimg, Buffer buffer);
}