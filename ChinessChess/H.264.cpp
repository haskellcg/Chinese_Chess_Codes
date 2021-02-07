/*
	@Product:H.264
	@Describe:采用UCCI协议的中国象棋引擎,支持最少的协议命令,通信部分采用Linux命名管道
	@Author:Mr.WDGB(王彬)
	@Date:2014-4-8 ----- 2014-6-2
	@Where:西安交通大学东2-130宿舍
*/
#include "CChessStruct.h"
#include "ChessManMove.h"
#include "ChessSearch.h"
#include "EvaluateSituation.h"
#include "UBStack.h"
#include "UBRandom.h"
#include "UBHashTable.h"
#include "MoveMaker.h"
#include "UBUCCI.h"
#include "UCCIEngine.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdio>
using std::cout;
using std::endl;
using std::setw;
int main()
{
	{
		UCCIEngine engine;
		engine.handleIOFunction();
	}
	return 0;
}
