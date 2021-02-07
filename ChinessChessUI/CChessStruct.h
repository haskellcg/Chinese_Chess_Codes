//FENStr指针由于只是使用一次，不需要使用智能指针，在判断是否合法时过滤掉即可
#ifndef CCHESSSTRUCT_H
#define CCHESSSTRUCT_H
#include "SmartPtr.h"
enum ChessMan
{
	redKing=16,
	redAdvisorOne,redAdvisorTwo,redBishopOne,redBishopTwo,redKnightOne,redKnightTwo,redRookOne,redRookTwo,redCannonOne,redCannonTwo,
	redPawnOne,redPawnTwo,redPawnThree,redPawnFour,redPawnFive,

	
	blackKing,
	blackAdvisorOne,blackAdvisorTwo,blackBishopOne,blackBishopTwo,blackKnightOne,blackKnightTwo,blackRookOne,blackRookTwo,
	blackCannonOne,blackCannonTwo,
	blackPawnOne,blackPawnTwo,blackPawnThree,blackPawnFour,blackPawnFive
};
class CChessStruct
{
    friend class MoveMaker;
    friend class MainWindow;
	private:
		bool isRed;
		int  captureStepNumber;
		int  roundsNumber;
		int cChessArray[256];
		int cChessManPosition[32];				//记录棋子的位置，0表示被吃掉
	private:	
		bool isLegalFENStr(const char* FENStr) const;
		bool isLegalInteger(int* index,const char* str) const;
		int  getIntegerFromStr(int* index,const char* str) const;
		void integerToStr(int integer,int* index,char* str) const;
		int  getChessManSubScript(char manCh) const;
		int  getSameNumber(int i,int j,const int* buffer) const;
	public:
		CChessStruct();
		CChessStruct(const char* FENStr);
		bool initializeFromFEN(const char* FENStr);
		SmartPtr<char> toFENStr() const;
		void reverseStruct();
		void reverseAll();
		bool isTurnRed() const;
		int stepNumberFromLastCapture() const;
		int getRoundsNumber() const;

		//For Debug
		void report() const;
};
#endif
