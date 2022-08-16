//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once

namespace DX12 {
	enum class ResourceBarrierState :int {
		PRESENT = 0,
		RENDER_TARGET = 0x4,
		COPY_DEST = 0x400,
		PIXEL_SHADER_RESOURCE = 0x80,
		UPLOAD_GENERIC_READ = 2755
	};
	//シェーダのタイプ
	enum class ShaderType :unsigned char {
		VERTEX,
		PIXEL,
		size
	};
	//頂点レイアウトの情報フォーマット
	enum class VertexLayoutFormat :unsigned char {
		R32G32B32_FLOAT,
		R8G8B8A8_UNORM,
		R32G32_FLOAT,
		R32_UINT,
		size
	};
	//GraphicsPipelineで指定するプリミティブトポロジ(構成要素の指定)
	enum class PrimitiveTopology :unsigned char {
		POINT,
		LINELIST,//独立した線分の集合
		LINESTRIP,//前の頂点とつながる線
		TRIANGLELIST,//独立した三角形の集合
		TRIANGLESTRIP,//前の三角形とつながった三角形の集合体
		size
	};
	//リソースのヒープのCPUからのアクセスの可不可の設定
	enum class ResourceHeapType :int {
		DEFAULT = 1,//CPUからアクセスできない
		UPLOAD = 2,//CPUからアクセスできる
		READBACK = 3,//CPUから読み取れる
	};
	//ディスクリプタヒープの種類
	enum class DescriptorHeapType :int {
		CBV_SRV_UAV = 0,
		RTV = 2,
		DSV = 3
	};
	//シェーダからの可視範囲に関する定数
	enum class DescriptorHeapShaderVisibility :unsigned char {
		NONE = 0,
		SHADER_VISIBLE = 1
	};
	//ディスクリプタレンジのタイプ
	enum class DescriptorRangeType :unsigned char {
		SRV = 0,
		UAV = 1,
		CBV = 2
	};
	//ルートパラメータのシェーダーからの見え具合
	enum class RootParameterShaderVisibility :unsigned char {
		ALL = 0,
		VERTEX = 1,
		PIXEL = 5
	};
}
