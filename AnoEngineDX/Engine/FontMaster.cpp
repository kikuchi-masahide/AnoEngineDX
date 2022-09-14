//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#include "FontMaster.h"

#include <fstream>
#include "DX12/Texture2D.h"
#include "DX12/Buffer.h"
#include "DX12/Master.h"
#include "DX12/ImageLoad.h"
#include "DX12/CommandQueue.h"
#include "DX12/GraphicsCommandList.h"
#include "DX12/Fence.h"
#include "DX12/VertexBuffer.h"
#include "DX12/IndexBuffer.h"
#include "CryptoMaster.h"
#include "Log.h"

namespace {
	//頂点バッファの一頂点
	struct Vertex {
	public:
		XMFLOAT3 pos_;
		XMFLOAT2 uv_;
	};
}

//与えられたバッファ位置からT型のデータを読み出し、位置をずらす
template<class T>
T ReadBuffer(unsigned char*& buffer) {
	T val = *((T*)buffer);
	buffer += sizeof(T);
	return val;
}

void FontMaster::Init(DX12::Master* dx12)
{
	dx12_ = dx12;
	upload_cmd_queue_ = dx12->CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);
	upload_cmd_list_ = dx12->CreateGraphicsCommandList(D3D12_COMMAND_LIST_TYPE_DIRECT);
	upload_fence_ = dx12->CreateFence();
}

void FontMaster::LoadFTMFile(std::string filename)
{
	{
		boost::unique_lock<boost::mutex> lock(files_mutex_);
		auto files_itr = files_.find(filename);
		if (files_itr != files_.end()) {
			return;
		}
	}
	std::shared_ptr<unsigned char[]> buffer = CryptoMaster::Decrypt(filename).first;
	//読み込む位置
	unsigned char* bufferpos = buffer.get();
	std::map<int, std::vector<SizeInfo>> fontmap;
	//フォントidとサイズの組の名前
	int idsize_pair_num = ReadBuffer<int>(bufferpos);
	//フォントのビットマップアップロードに用いる中間バッファ(サイズが足りなくなったら適宜作り直す)
	std::shared_ptr<DX12::Buffer> texture_buffer;
	//現在のbufferのサイズ
	size_t texture_buffer_size = 0;
	auto fenceevent = CreateEvent(NULL, FALSE, FALSE, NULL);
	for (int idsize_pair_index = 0; idsize_pair_index < idsize_pair_num; idsize_pair_index++) {
		int fontid = ReadBuffer<int>(bufferpos);
		int fontsize = ReadBuffer<int>(bufferpos);
		auto itr = fontmap.find(fontid);
		if (itr == fontmap.end()) {
			fontmap.emplace(fontid, std::vector<SizeInfo>());
			itr = fontmap.find(fontid);
		}
		int tmascent = ReadBuffer<int>(bufferpos);
		int tmdescent = ReadBuffer<int>(bufferpos);
		itr->second.emplace_back(tmascent, tmdescent);
		SizeInfo& sizeinfo = itr->second.back();
		int charnum = ReadBuffer<int>(bufferpos);
		for (int charindex = 0; charindex < charnum; charindex++) {
			UChar32 codepoint = ReadBuffer<int>(bufferpos);
			int gmblackboxx = ReadBuffer<int>(bufferpos);
			int gmblackboxy = ReadBuffer<int>(bufferpos);
			int bmptglyphoriginx = ReadBuffer<int>(bufferpos);
			int bmptglyphoriginy = ReadBuffer<int>(bufferpos);
			int gmcellincx = ReadBuffer<int>(bufferpos);
			auto al_width = DX12::GetAlignmentedRowPitch(sizeof(float) * gmblackboxx);
			//以下ビットマップのコピー
			if (texture_buffer_size < al_width * gmblackboxy) {
				//bufferのサイズが足りないので作り直し
				texture_buffer_size = al_width * gmblackboxy;
				texture_buffer = dx12_->CreateBuffer(
					D3D12_HEAP_TYPE_UPLOAD, texture_buffer_size, D3D12_RESOURCE_STATE_GENERIC_READ
				);
			}
			float* texture_buffer_map = static_cast<float*>(texture_buffer->Map());
			for (int y = 0; y < gmblackboxy; y++) {
				for (int x = 0; x < gmblackboxx; x++) {
					char val = ReadBuffer<char>(bufferpos);
					texture_buffer_map[x] = static_cast<float>(val) / 16;
				}
				texture_buffer_map += al_width / sizeof(float);
				//bufferpos += gmblackboxx;
			}
			texture_buffer->Unmap();
			//最終的なビットマップのコピー先
			std::shared_ptr<DX12::Texture2D> final_bitmap = dx12_->CreateTexture2D(
				gmblackboxx, gmblackboxy, DXGI_FORMAT_R32_FLOAT, D3D12_HEAP_TYPE_DEFAULT,
				D3D12_TEXTURE_LAYOUT_UNKNOWN, D3D12_RESOURCE_STATE_COPY_DEST
			);
			upload_cmd_list_->CopyBufferToTexture2D(
				texture_buffer, gmblackboxx, gmblackboxy, DXGI_FORMAT_R32_FLOAT, sizeof(float)*gmblackboxx, final_bitmap
			);
			//描画するときのためにshader resourceに指定しておく
			upload_cmd_list_->SetResourceBarrier(DX12::ResourceBarrierUnit(
				final_bitmap, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
			));
			upload_cmd_list_->Close();
			std::vector<std::shared_ptr<DX12::GraphicsCommandList>> lists(1, upload_cmd_list_);
			upload_cmd_queue_->ExecuteCommandLists(lists);
			int fenceval = upload_fence_->GetCompletedValue();
			upload_cmd_queue_->Signal(upload_fence_, fenceval + 1);
			upload_fence_->SetEventOnCompletion(fenceval + 1, fenceevent);
			WaitForSingleObject(fenceevent, INFINITE);
			upload_cmd_list_->ResetCommandAllocator();
			upload_cmd_list_->ResetCommandList();
			sizeinfo.bitmaps_.emplace(std::piecewise_construct, std::forward_as_tuple(codepoint),
				std::forward_as_tuple(gmblackboxx,gmblackboxy,bmptglyphoriginx,bmptglyphoriginy,
					gmcellincx,final_bitmap));
		}
	}
	{
		boost::unique_lock<boost::mutex> lock(files_mutex_);
		files_.emplace(filename, FileInfo());
		FileInfo& fileinfo = files_.find(filename)->second;
		fileinfo.fonts_.swap(fontmap);
	}
}

void FontMaster::ReleaseFTMFile(std::string filename)
{
	boost::unique_lock<boost::mutex> lock(files_mutex_);
	auto itr = files_.find(filename);
	if (itr != files_.end()) {
		files_.erase(itr);
	}
}

double FontMaster::GetDrawInfoFor(const char8_t* utf8, int fontid, double height, std::vector<CharDrawInfo>& output)
{
	output.clear();
	//HACK:mutexの指定
	//file(または複数のファイルの組)は同時に1つしか保存できない設計でいいか?
	int utf8pos = 0;
	//(描画しようとしている文字の)原点x座標
	double originx = 0.0;
	while (true) {
		if (utf8[utf8pos] == NULL) {
			break;
		}
		UChar32 codepoint;
		U8_NEXT(utf8, utf8pos, -1, codepoint);
		bool found = false;
		//フォントIDとコードポイントで検索
		for (auto fileinfo : files_) {
			if (found) {
				break;
			}
			auto& fonts = fileinfo.second.fonts_;
			auto sizeinfos_itr = fonts.find(fontid);
			if (sizeinfos_itr == fonts.end()) {
				continue;
			}
			auto& sizeinfos = sizeinfos_itr->second;
			for (auto& sizeinfo : sizeinfos) {
				auto charinfo_itr = sizeinfo.bitmaps_.find(codepoint);
				if (charinfo_itr != sizeinfo.bitmaps_.end()) {
					auto& charinfo = charinfo_itr->second;
					//このフォントの各数値を要求縦幅に合わせるために何倍するか
					double r = height / (sizeinfo.tmAscent_ + sizeinfo.tmDescent_);
					if (originx == 0.0 && charinfo.bmptGlyphOriginX_ < 0) {
						originx = -r * charinfo.bmptGlyphOriginX_;
					}
					double leftx = originx + charinfo.bmptGlyphOriginX_ * r;
					double bottomy = (sizeinfo.tmDescent_ + charinfo.bmptGlyphOriginY_ - charinfo.gmBlackBoxY_) * r;
					double width = charinfo.gmBlackBoxX_ * r;
					double height = charinfo.gmBlackBoxY_ * r;
					output.emplace_back(charinfo.bitmap_, leftx, bottomy, width, height);
					originx += charinfo.gmCellIncX_ * r;
					found = true;
					break;
				}
			}
		}
	}
	return originx;
}
