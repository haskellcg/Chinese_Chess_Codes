#ifndef EVALUATESITUATION_H
#define EVALUATESITUATION_H
#include "CChessStruct.h"
enum ThreatenLevel
{
	smallThreatenLevel,
	middleThreatenLevel,
	bigThreatenLevel
};
enum KnowledgeLevel
{
	staticLevel,
	dynamicFirstLevel,
	dynamicSecondLevel,
	dynamicThirdLevel,
	formFirstLevel,
	formSecondLevel,
	formThirdLevel,
	moreKnowledgeLevel
};
enum SubjectiveSituation
{
	otherPessimisticSubjective=-12,
	selfPessimisticSubjective=-6,
	normalSubjective=0,
	selfOptimisticSubjective=6,
	otherOptimisticSubjective=12
};
class EvaluateSituation
{
	private:
		SmartPtr<CChessStruct> cChess;
		//机器的考虑深度，可以叫做智力水平
		KnowledgeLevel knowledgeLevel;
		//棋手的主观状态，会影响到对局面的评估、在动态评估的选择项
		int subjectiveOfPlayer;
		const int subjectiveForSelfScope;
		const int subjectiveForOtherScope; 
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
		bool isEndGame() const;
		int  getChessManStaticPosVal(int position) const;
		//静态评估
		double  chessManStaticPosEvaluate() const;
		bool isCrossRiver(int position) const;	
		ThreatenLevel getThreatenLevel(bool isRed,int subjective) const;
		bool isSelfSideChessMan(int source,int position) const;
		int getChessManSub(int chessManValue) const;
		int compareChessManCheckAndBalance(int firstChessManValue,int secondChessManValue) const;
		void knightDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndbalance) const;
		void bishopDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndbalance) const;
		void advisorDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndbalance) const;
		void kingDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndbalance) const;
		void rookDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndbalance) const;
		void cannonDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndbalance) const;
		void pawnDynamicPosEvaluate(int position,int* flexibility,int* killScore,int* checkAndbalance) const;
		//动态评估
		double  chessManDynamicPosEvaluate();
		//形式评估
		int abs(int value) const;
		double 	particularFormEvaluate() const;
		double  moreEvaluate() const;
	public:
		EvaluateSituation();
		EvaluateSituation(SmartPtr<CChessStruct> _cChess);
		void bindStruct(SmartPtr<CChessStruct> _cChess);
		void setSubjective(SubjectiveSituation _subjectiveOfPlayer);
		SubjectiveSituation  getSubjective() const;
		void setKnowledgeLevel(KnowledgeLevel _knowledgeLevel);
		KnowledgeLevel getKnowledgeLevel() const;
		double evaluate();
	private:
		//不建议外部使用，值不确定
		ThreatenLevel evaluatedThreatenLevel;
};
typedef void (EvaluateSituation::*chessManEvaluateFunPtr)(int,int*,int*,int*) const;
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
