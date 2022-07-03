#pragma once

#include "DX12ConfigEnums.h"

struct DX12VertexLayoutUnit {
public:
	LPCSTR mSemanticName;
	DX12Config::VertexLayoutFormat mFormat;   //フォーマット
	UINT mInputSlot;   //入力スロットインデックス
	DX12Config::VertexLayoutInputClassification mInputClassification;
	UINT mInstanceDataStepRate;   //一度に描画するインスタンスの数
	DX12VertexLayoutUnit(LPCSTR _sem, DX12Config::VertexLayoutFormat _format, UINT _slot,
		DX12Config::VertexLayoutInputClassification _classification, UINT _inst)
		:mSemanticName(_sem), mFormat(_format), mInputSlot(_slot),
		mInputClassification(_classification), mInstanceDataStepRate(_slot)
	{}
};

using DX12VertexLayout = std::vector<DX12VertexLayoutUnit>;