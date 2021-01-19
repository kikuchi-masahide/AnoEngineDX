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
		size
	};
	//頂点レイアウトのinput_classificastion
	enum class VertexLayoutInputClassification :unsigned char {
		INPUT_CLASSIFICATION_PER_VERTEX_DATA,
		INPUT_CLASSIFICATION_PER_INSTANCE_DATA,
		size
	};
	//コマンドリストで指定するプリミティブトポロジ
	enum class PrimitiveTopology :unsigned char {
		POINT,
		LINE,
		TRIANGLE,
		TRIANGLESTRIP,
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
}
