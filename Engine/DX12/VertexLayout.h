//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include "ConfigEnums.h"

namespace DX12 {
	/// <summary>
	/// 頂点レイアウトの一つの要素を表す パイプラインにはこの配列を頂点レイアウト全体として渡す
	/// </summary>
	struct VertexLayoutUnit {
	public:
		//セマンティック名("POSITION"など)
		LPCSTR semantic_name_;
		//フォーマット
		VertexLayoutFormat format_;
		//入力スロットインデックス
		UINT input_slot_;
		VertexLayoutUnit(LPCSTR sem, VertexLayoutFormat format, UINT slot)
			:semantic_name_(sem), format_(format), input_slot_(slot)
		{}
	};
}
