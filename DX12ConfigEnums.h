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
}
