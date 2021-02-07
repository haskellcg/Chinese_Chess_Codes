#ifndef CHESSMANMOVE_H
#define CHESSMANMOVE_H
#include "CChessStruct.h"
//注意:在生成走法时考虑源数组chessArray、chessManPostion数组
//	以及moveArray数组的整理
//1、记录直线走法的最大不吃子位置有利于简化走法数组
//2、如果每次走法生成都能依据之前生成的走法数组以及回合之间的变化，则生成算法更加省时间,所以makeMove函数应该在该函数中实现
//3、建议建立一个movetransform类，实现move的各种格式的转换，并且有助于2、的实现
struct Move
{
	int from;
	int to;
	int capture;
};
class ChessManMove
{
	private:
		SmartPtr<CChessStruct> cChess;
		Move moveArray[256];
		int  moveArrayLen;
		//为了记录最优走法，以及双手走法，之后排序
		Move bestMove;
		Move killerMove[5];
		int  killerMoveLen;
		int checkingChessManPosition[16];
		int checkingChessManPositionLen;
		const int kingLegalPosVal;
		const int knightLegalPosVal;
		const int cannonLegalPosVal;
		const int rookLegalPosVal;
		const int advisorLegalPosVal;
		const int bishopLegalPosVal;
		const int pawnLegalPosVal;
		int legalPosition[2][256];
	private:
		void initializeLegalPosition();
		bool isInLegalPosition(int source,int chessManPosVal,int position) const;
		void saveLegalMove(int _from,int _to,int _capture);
		bool isSelfSideChessMan(int source,int position) const;
		bool isGeneral(int position) const;
		bool genKnightMove(bool capture,int position,bool* retureCheck);
		bool genBishopMove(bool capture,int position,bool* retureCheck);
		bool genKingMove(bool capture,int position,bool* retureCheck);
		bool genAdvisorMove(bool capture,int position,bool* retureCheck);
		bool genRookMove(bool capture,int position,bool*  retureCheck);
		bool genCannonMove(bool capture,int position,bool*  retureCheck);
		bool genPawnMove(bool capture,int position,bool*  retureCheck);
		bool isSameMove(const Move& lmove,const Move& rmove);
		void moveArraySort();
	public:
		ChessManMove();
		ChessManMove(SmartPtr<CChessStruct> _cChess);
		void bindStruct(SmartPtr<CChessStruct> _cChess);
		//产生所有招法后自动排序
		void genAllMove();
		void genCaptureMove();
		bool check();
		bool generalOpposite() const;
		int  getCheckingChessManPosition(int index) const;
		int  getMoveArrayLen() const;
		bool getMove(int index,Move* _move) const;
		void clearMoveArray();
		void setBestMove(const Move& _move);
		void setKillerMove(const Move& _move);
		void clearKillerMove();
		//For Debug
		void reportMove() const;
};
typedef bool (ChessManMove::*genMoveFunPtr)(bool,int,bool*);
extern int kingMoveOff[4];
extern int advisorMoveOff[4];
extern int bishopMoveOff[4];
extern int bishopEye[4];
extern int knightMoveOff[8];
extern int knightLeg[8];
extern int cannonMoveOff[4];
extern int rookMoveOff[4];
extern int pawnMoveOff[2][3];
#endif
