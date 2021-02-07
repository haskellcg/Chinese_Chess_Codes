#include "Zobrist.h"
#include "UBException.h"
#include "UBRandom.h"
#include <iostream>
using std::cout;
using std::endl;
uint32_ub Zobrist::generateRandUInt32()
{
	return this->ubRandom.randomLinear();
}
uint64_ub Zobrist::generateRandUInt64()
{
	return this->ubRandom.randomLinear64();
}
void Zobrist::initialMember()
{
	this->zobristKey=0;
	this->zobristCheck=0;
	this->playerZobrist=generateRandUInt32();
	this->playerZobristCheck=generateRandUInt64();
	for(int i=0;i<14;i++)
	{
		for(int j=0;j<256;j++)
		{
			this->chessManZobrist[i][j]=generateRandUInt32();
			this->chessManZobristCheck[i][j]=generateRandUInt64();
		}
	}
}
int Zobrist::getChessManSub(int chessManValue) const
{
	switch(chessManValue)
	{
		case redKing:		return 0;
		case redAdvisorOne:
		case redAdvisorTwo:	return 1;
		case redBishopOne:
		case redBishopTwo:	return 2;
		case redKnightOne:
		case redKnightTwo:	return 3;
		case redRookOne:
		case redRookTwo:	return 4;
		case redCannonOne:
		case redCannonTwo:	return 5;
		case redPawnOne:
		case redPawnTwo:
		case redPawnThree:
		case redPawnFour:
		case redPawnFive:	return 6;
		case blackKing:		return 7;
		case blackAdvisorOne:
		case blackAdvisorTwo:	return 8;
		case blackBishopOne:
		case blackBishopTwo:	return 9;
		case blackKnightOne:
		case blackKnightTwo:	return 10;
		case blackRookOne:
		case blackRookTwo:	return 11;
		case blackCannonOne:
		case blackCannonTwo:	return 12;
		case blackPawnOne:
		case blackPawnTwo:
		case blackPawnThree:
		case blackPawnFour:
		case blackPawnFive:	return 13;
	}
	return -1;
}
uint32_ub Zobrist::getChessManZobrist(int chessManValue,int position) const
{
	if(position < 0 || position >255 || chessManValue < 16 || chessManValue > 47)
		throw UBException(-1,"parameter is inlegal,please check..");
	return chessManZobrist[getChessManSub(chessManValue)][position];
}
uint64_ub Zobrist::getChessManZobristCheck(int chessManValue,int position) const
{
	if(position < 0 || position >255 || chessManValue < 16 || chessManValue > 47)
		throw UBException(-1,"parameter is inlegal,please check..");
	return chessManZobristCheck[getChessManSub(chessManValue)][position];
}
Zobrist::Zobrist():cChess(NULL)
{
	CChessStruct *temp=new CChessStruct;
	this->cChess=makeSmartPtr(temp);
	initialMember();
}
Zobrist::Zobrist(SmartPtr<CChessStruct> _cChess):cChess(_cChess)
{
	if(_cChess.getPtr() == NULL)
	{
		CChessStruct *temp=new CChessStruct;
		this->cChess=makeSmartPtr(temp);
	}
	initialMember();
}
void Zobrist::bindStruct(SmartPtr<CChessStruct> _cChess)
{
	if(_cChess.getPtr() != NULL)
	{
		this->cChess=_cChess;
		initialMember();
	}
}
void Zobrist::generate()
{
	int _chessManValue,_chessManPosition;
	zobristKey=zobristKey ^ playerZobrist;
	zobristCheck=zobristCheck ^ playerZobristCheck;
	for(int i=0;i<32;i++)
	{
		_chessManValue=i+16;
		_chessManPosition=cChess->cChessManPosition[i];
		if(_chessManPosition != 0)
		{
			zobristKey=zobristKey ^ getChessManZobrist(_chessManValue,_chessManPosition);
			zobristCheck=zobristCheck ^ getChessManZobristCheck(_chessManValue,_chessManPosition);
		}
	}
}
//在使用一种走法后，界面的Zobrist键值不用重新计算，使用异或
//应为棋盘走完后才会调该高函数，所以无法验证_move是否合法
//由于会有悔棋操作，但是其中的过程不需要去产生zobrist，且需要在MoveMaker中插入该类，不易调理
//我们之需要到时候直接调用generate即可
void Zobrist::generateForMove(const Move& _move)
{
	zobristKey = zobristKey ^ playerZobrist;
	zobristCheck = zobristCheck ^ playerZobristCheck;
	int chessManFromValue=cChess->cChessArray[_move.from];
	zobristKey = zobristKey ^ getChessManZobrist(chessManFromValue,_move.from);
	zobristCheck=zobristCheck ^ getChessManZobrist(chessManFromValue,_move.from);
	int chessManToValue=_move.capture;
	zobristKey = zobristKey ^ getChessManZobrist(chessManToValue,_move.to);
	zobristCheck=zobristCheck ^ getChessManZobrist(chessManToValue,_move.to);
}
uint32_ub Zobrist::getZobristKey() const
{
	return this->zobristKey;
}
uint64_ub Zobrist::getZobristCheck() const
{
	return this->zobristCheck;
}
