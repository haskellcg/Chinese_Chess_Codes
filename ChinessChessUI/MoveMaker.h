#ifndef MOVE_MAKER_H
#define MOVE_MAKER_H
//For class CChessStruct
#include "CChessStruct.h"
#include "UBString.h"
//实现移动在棋盘上的执行、以及各种移动的转换
//存储走棋历史，从而实现撤销功能、撤销策略(这里是考虑是否把撤销动作作为历史)
//设置isRed、captureNumber、roundsNumber
struct Move
{
    int from;
    int to;
    int capture;
};
struct ICCSMove
{
	char fromX;
	char fromY;
	char toX;
	char toY;
};
struct ChinessMove
{
	UBString chessMan;
	UBString from;
	UBString act;
	UBString to;
};
struct WXFMove
{
	char chessMan;
	char source;
	char signal;
	char dest;
};
struct RoundsCapture
{
	int movedCaptureStepNumber;
	int movedRoundsNumber;
};
class MoveMaker
{
	private:
		SmartPtr<CChessStruct> 	cChess;
		Move*		       	moveHistory;
		//移动数组的辅助数据
		RoundsCapture*		counterHistory;
		int		       	moveHistoryLen;
		int		       	moveHistoryLimit;
		bool 		       	unMakeIsHistory;
		bool			chessIsNewForMe;
		//用于记录makeMove的节点，在reDO中需要，及封顶界面
		int			lastNewIndex;
		//在撤销以及回溯中都用到，主要用于在0～lastNewIndex中间游荡
		int			lastVisitIndex;
		//在reDo的开始出需要用到，如果之前不是，则无法回溯
		bool			lastIsUnMake;
		bool			lastIsReMake;
	private:
		bool isRedChessMan(int position) const;
		bool isLegalMove(const Move& _move) const;
	public:
		MoveMaker();
		MoveMaker(SmartPtr<CChessStruct> _cChess);
		MoveMaker(const MoveMaker& _moveMaker);
		MoveMaker& operator=(const MoveMaker& _moveMaker);
		~MoveMaker();
		void bindStruct(SmartPtr<CChessStruct> _cChess);
		bool makeMove(const Move& _move);
		bool unMakeMove();
		bool reMakeMove();
		int getMoveHistoryLen() const;
		bool getHistoryMove(int index,Move* _move) const;
		void setHistoryDecision(bool _unMakeIsHistory);
		void report() const;
		//************************************************************************************
		//各种形式的转换
		//转换只是转换参数的移动，不会去判别该移动对当前棋盘的是否合法
		//************************************************************************************
		//ICCS表示-Internet Chiness Chess Server
		//  a b c d e f g h i
		//  9 8 7 6 5 4 3 2 1 0
		static ICCSMove 	translateToICCS(const Move& _move);
		static Move		translateToMove(SmartPtr<CChessStruct> _cChess,const ICCSMove& _iccsMove);
		//中文纵线格式
		//由于表示需要用到棋盘，所以需要一个棋盘参数
		static ChinessMove	translateToChinessZX(SmartPtr<CChessStruct> _cChess,const Move& _move);
		static Move 		translateToMove(SmartPtr<CChessStruct> _cChess,const ChinessMove& _chinessMove);
		//WXF纵线格式
		//WXF-World XiangQi Federation
		static WXFMove	 	translateToWXF(SmartPtr<CChessStruct> _cChess,const Move& _move);
		static Move		translateToMove(SmartPtr<CChessStruct> _cChess,const WXFMove& _wxfMove);
	private:
		static UBString		getChessManChinessName(int chessManValue);
		static char		getChessManWXFName(const UBString& _chinessName);
		static bool		isChessManChinessName(const UBString& _chessManName);
		static bool		isChessManWXFName(char _wxfName);
		static UBString		getNumberStr(int number);
		static UBString		getNumberStr(int number,int forDiff);
		static int		getNumber(const UBString& _number);
		static int		getNumber(const UBString& _number,int forDiff);
};
const int moveHistoryInitialCapacity=256;
#endif
