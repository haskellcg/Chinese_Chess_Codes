#ifndef ZOBRIST_H
#define ZOBRIST_H
#include "CChessStruct.h"
#include "ChessManMove.h"
#include "UBRandom.h"
#include "UBTypeDef.h"
class Zobrist
{
	private:
		SmartPtr<CChessStruct> cChess;
		UBRandom	ubRandom;
		uint32_ub	zobristKey;
		uint64_ub	zobristCheck;
		uint32_ub	playerZobrist;
		uint64_ub	playerZobristCheck;
		uint32_ub	chessManZobrist[14][256];
		uint64_ub 	chessManZobristCheck[14][256];
	private:
		uint32_ub generateRandUInt32();
		uint64_ub generateRandUInt64();
		void initialMember();
		int getChessManSub(int chessManValue) const;
		uint32_ub getChessManZobrist(int chessManValue,int position) const;
		uint64_ub getChessManZobristCheck(int chessManValue,int position) const;
	public:
		Zobrist();
		Zobrist(SmartPtr<CChessStruct> _cChess);
		void bindStruct(SmartPtr<CChessStruct> _cChess);
		void generate();
		void generateForMove(const Move& _move);
		uint32_ub getZobristKey() const;
		uint64_ub getZobristCheck() const;
};
#endif
