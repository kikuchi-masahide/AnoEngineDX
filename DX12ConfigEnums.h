#pragma once
/// <summary>
/// クラスDX12が持つインターフェースで用いるenum
/// </summary>

namespace DX12Config {
	//ディスクリプタヒープの種類
	enum class DescriptorHeapType :unsigned char {
		RTV,
		CBV,
		SRV,
		UAV,
		DSV,
		size
	};
	//シェーダからの可視範囲に関する定数
	enum class ShaderVisibility :unsigned char {
		NONE,
		size
	};
	//リソースのヒープのCPUからのアクセスの可不可の設定
	enum class ResourceHeapType {
		DEFAULT,//CPUからアクセスできない
		UPLOAD,//CPUからアクセスできる
		READBACK,//CPUから読み取れる
		size
	};
	//シェーダのタイプ
	enum class ShaderType {
		VERTEX,
		PIXEL,
		size
	};
	//頂点レイアウトの情報フォーマット
	enum class VertexLayoutFormat {
		R32G32B32_FLOAT,
		R8G8B8A8_UNORM,
		size
	};
	//頂点レイアウトのinput_classificastion
	enum class VertexLayoutInputClassification {
		INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		INPUT_CLASSIFICATION_PER_INSTANCE_DATA,
		size
	};
	//プリミティブトポロジ
	enum class PrimitiveTopologyType {
		POINT,
		LINE,
		TRIANGLE,
		size
	};
}
