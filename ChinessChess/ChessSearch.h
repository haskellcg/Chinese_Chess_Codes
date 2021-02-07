#ifndef CHESSSEARCH_H
#define CHESSSEARCH_H
#include "UBHashTable.h"
#include "UBTypeDef.h"
#include "CChessStruct.h"
#include "ChessManMove.h"
#include "EvaluateSituation.h"
#include "MoveMaker.h"
#include "Zobrist.h"
#include "./Book/UBTieTree.h"
#include "./Book/ChessRecordStruct.h"
#include "./Book/Txt2Struct.h"
#include <ctime>
#define MEMOCAPACITY 1024*1024
#define POSITIVE_MAX 10000
#define NEGATIVE_MIN -10000
#define	VICTORY_VALUE 1000
#define STEPNUMBER_MAX 100
//节点的搜索类型
enum ABSearchType
{
	alphaNode,
	betaNode,
	allNode,
	nullNode
};
//置换表的节点，包括Zobrist键值、评估值、搜索深度、节点类型
struct MemoTableNode
{
	uint64_ub	zobristCheck;
	double		evaluateValue;
	int		searchDepth;
	Move		nodeBestMove;
	ABSearchType	nodeType;
};
enum SearchMode
{
	TimeSearch,
	DepthSearch
};
//哈希函数
int	memoHashFunc(const uint32_ub& _zobristKey);
//判断节点相同的函数，用于判断哈希表中的节点相同
bool	memoNodeSameFunc(const UBHashNode<uint32_ub,MemoTableNode>& lnode,const UBHashNode<uint32_ub,MemoTableNode>& rnode);
//最大搜索深度，最底层为0层
const int MaxSearchDepth=4;
const int TotalTimeLimitConst=3000;
const int BookEndRounds=10;
//负责搜索的类，结果反应在成员中
class ChessSearch
{
	friend class UCCIEngine;
	private:
		UBHashTable<uint32_ub,MemoTableNode>	memoTable;
		SmartPtr<CChessStruct>			cChess;
		//不适合使用一个对象，这样所有调用gen..Move的走法会占用一个空间，从而覆盖有用数据
		//所以使用指针
		ChessManMove				*chessManMovePtr;
		EvaluateSituation			evaluateSituation;
		//不适合设置unMakeIsHistory=true的情况，否则空间很快用光
		MoveMaker				moveMaker;
		Zobrist					zobrist;
		int					stepNumber;
		//Alpha-Beta函数的输出结果
		int					finalValue;
		Move					bestMove;
		//思考模式
		SearchMode				alphaSearchMode;
		UBTieTree				*tieTree;
	private:
		//不建议用户操作的变量，只是作为函数的全局变量的需求
		time_t					sg_StartTime;
		int					sg_TimeLimit;
		int 					totalTimeLimit;
		bool					isStartFEN;
		int					totalNodes;
	public:
		ChessSearch(SmartPtr<CChessStruct> _cChess);
		~ChessSearch();
		void bindStruct(SmartPtr<CChessStruct> _cChess);
		void alphaBetaSearch();
		Move getBestMove() const;
		int  getFinalValue() const;
		int  getTotalNodes() const;
		void setSearchMode(SearchMode _searchMode);
		void setBookMode(bool _firstForWin);
		void setTotalTimeLimit(int _totalTimeLimit);
		void makeMoveForSearch(const Move& _move);
	private:
		void _ComputerThinkForTime(int _timeLimit);
		void _ComputerThinkForDepth();
		double _AlphaBetaSearch(int _depth,double _alpha,double _beta);
		double _QuiescentSearch(double _alpha,double _beta);
		bool readMemoTable(int _depth,double _alpha,double _beta,MemoTableNode *memoTableNode);
		bool saveMemoTable(const MemoTableNode& memoTableNode);
		void clearMemoTable();
		bool isNullMove(const Move& _move) const;
		bool isCheckDie();
		bool isSafeMove(const Move& _move);
		void initTieTree();
};
const int sg_FirstMoveNumber=50;
const int sg_SecondMoveNumber=60;
const int sg_ThirdMoveNumber=80;
const int sg_FirstMoveNumberLimit=60;
const int sg_SecondMoveNumberLimit=80;
const int sg_ThirdMoveNumberLimit=100;
#endif
