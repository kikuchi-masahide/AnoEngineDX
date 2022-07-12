//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#include <cmath>
#include <string>
#include <map>
#include <Windows.h>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <queue>
#include <typeinfo>
#include <random>

#include "boost/noncopyable.hpp"

#include "boost/shared_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include "boost/make_shared.hpp"
#include "boost/scoped_ptr.hpp"

//正直ここに置きたくないが，シェーダとの情報の受け渡しのために仕方なく
#include <DirectXMath.h>
//一部構造体のみ名前空間を外し使う構造体を分かりやすくする
using DirectX::XMMATRIX;
using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;