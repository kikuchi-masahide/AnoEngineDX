//================================================================================
//Copyright <2022> ano3. All rights reserved.
//This source code and a part of it must not be reproduced or used in any case.
//================================================================================
#pragma once
#include <string>

//与えられたファイル名(.json、ftm/込み)から、同名の.ftmファイルを作成する(既に存在するならば無視する)
void GenerateFTMFile(std::string filename);