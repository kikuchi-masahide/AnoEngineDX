#pragma once
/// <summary>
/// クラスDX12が持つインターフェースで用いるenum
/// </summary>

namespace DX12Config {
	//ディスクリプタヒープの種類
	enum class DescriptorHeapType :unsigned char {
		RTV,
		CBV_SRV_UAV,
		DSV,
		size
	};
	//シェーダからの可視範囲に関する定数
	enum class DescriptorHeapShaderVisibility :unsigned char {
		NONE,
		SHADER_VISIBLE,
		size
	};
	//リソースのヒープのCPUからのアクセスの可不可の設定
	enum class ResourceHeapType :unsigned char {
		DEFAULT,//CPUからアクセスできない
		UPLOAD,//CPUからアクセスできる
		READBACK,//CPUから読み取れる
		size
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
	//頂点レイアウトのinput_classificastion
	enum class VertexLayoutInputClassification :unsigned char {
		INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		INPUT_CLASSIFICATION_PER_INSTANCE_DATA,
		size
	};
	//コマンドリストで指定するプリミティブトポロジ(構成要素の指定)
	enum class PrimitiveTopology :unsigned char {
		POINT,
		LINELIST,//独立した線分の集合
		LINESTRIP,//前の頂点とつながる線
		TRIANGLELIST,//独立した三角形の集合
		TRIANGLESTRIP,//前の三角形とつながった三角形の集合体
		size
	};
	//グラフィックスパイプラインで指定するプリミティブトポロジタイプ
	enum class PrimitiveTopologyType :unsigned char {
		POINT,
		LINE,
		TRIANGLE,
		size
	};
	//ディスクリプタレンジのタイプ
	enum class DescriptorRangeType :unsigned char {
		CBV,
		SRV,
		UAV,
		size
	};
	//ルートパラメータのシェーダーからの見え具合
	enum class RootParameterShaderVisibility :unsigned char {
		ALL,
		VERTEX,
		PIXEL,
		size
	};
	//リソースバリアの遷移前後の定数
	enum class ResourceBarrierState :unsigned char {
		PRESENT,
		RENDER_TARGET,
		COPY_DEST,
		PIXEL_SHADER_RESOURCE,
		size
	};
}
