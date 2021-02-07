#include "ChessSearch.h"
#include "UBException.h"
#include <iostream>
using std::cout;
using std::endl;
//Zobrist函数优化：在调用makemove时调用Formove函数可以减小开销
//ChessManMove
//EvaluateSituation
//目前先不优化
int memoHashFunc(const uint32_ub& _zobristKey)
{
	return (int)(_zobristKey % MEMOCAPACITY);
}
bool memoNodeSameFunc(const UBHashNode<uint32_ub,MemoTableNode>& lnode,const UBHashNode<uint32_ub,MemoTableNode>& rnode)
{
	return ((lnode.nodeKey == rnode.nodeKey) && (lnode.nodeValue.zobristCheck == rnode.nodeValue.zobristCheck));
}
ChessSearch::ChessSearch(SmartPtr<CChessStruct> _cChess):memoTable(MEMOCAPACITY,&memoHashFunc,&memoNodeSameFunc),cChess(_cChess),
							 evaluateSituation(cChess),
							 moveMaker(cChess),zobrist(cChess),tieTree(NULL)
{
	if(cChess.getPtr() == NULL)
		throw UBException(-1,"CChessStruct parameter is null pointer,please check..");
	CChessStruct newStart;
	if(UBString(cChess->toFENStr().getPtr()) == UBString(newStart.toFENStr().getPtr()))
	{
		isStartFEN=true;
	}
	else
	{
		isStartFEN=false;
	}
	chessManMovePtr=new ChessManMove[MaxSearchDepth];
	for(int i=0;i<MaxSearchDepth;i++)
	{
		chessManMovePtr[i].bindStruct(cChess);
	}
	evaluateSituation.setKnowledgeLevel(dynamicThirdLevel);
	stepNumber=0;
	bestMove.from=bestMove.to=bestMove.capture=0;
	finalValue=-1000;
	alphaSearchMode=DepthSearch;
	totalTimeLimit=TotalTimeLimitConst;
	sg_StartTime=0;
	sg_TimeLimit=0;
	totalNodes=0;
	initTieTree();
}
ChessSearch::~ChessSearch()
{
	if(chessManMovePtr != NULL)
		delete []chessManMovePtr;
}
void ChessSearch::bindStruct(SmartPtr<CChessStruct> _cChess)
{
	if(_cChess.getPtr() != NULL)
	{
		CChessStruct newStart;
		if(UBString(_cChess->toFENStr().getPtr()) == UBString(newStart.toFENStr().getPtr()))
			isStartFEN=true;
		else
			isStartFEN=false;
		this->cChess=_cChess;
		clearMemoTable();
		for(int i=0;i<MaxSearchDepth;i++)
			chessManMovePtr[i].bindStruct(cChess);
		evaluateSituation.bindStruct(cChess);
		moveMaker.bindStruct(cChess);
		zobrist.bindStruct(cChess);
		stepNumber=0;
		bestMove.from=bestMove.to=bestMove.capture=0;
		finalValue=0;
		if(tieTree!=NULL)
			tieTree->initSearchPos();
	}
}
void ChessSearch::alphaBetaSearch()
{
	if(isStartFEN && (this->cChess->roundsNumber <= BookEndRounds) && (tieTree != NULL))
	{
		bestMove.from=bestMove.to=bestMove.capture=0;
		finalValue=-1000;
		bool hasHit=false;
		if(moveMaker.getMoveHistoryLen() == 0)
		{
			ChinessMove _sourceMove,_destMove;
			Move _resultMove;
			_destMove=tieTree->searchTieTreeForMove(_sourceMove);
			cout<<"$>> "<<_destMove.chessMan<<_destMove.from<<_destMove.act<<_destMove.to<<endl;
			_resultMove=MoveMaker::translateToMove(cChess,_destMove);
			cout<<"$>> "<<_resultMove.from<<" "<<_resultMove.to<<endl;
			bestMove=_resultMove;
			finalValue=VICTORY_VALUE;
			hasHit=true;
		}
		else
		{
			Move _lastMove,_resultMove;
			ChinessMove _sourceMove,_destMove;
			moveMaker.getHistoryMove(moveMaker.getMoveHistoryLen()-1,&_lastMove);
			moveMaker.unMakeMove();
			_sourceMove=MoveMaker::translateToChinessZX(cChess,_lastMove);
			moveMaker.reMakeMove();
			_destMove=tieTree->searchTieTreeForMove(_sourceMove);
			if(_destMove.chessMan != "")
			{
				cout<<"$>> "<<_destMove.chessMan<<_destMove.from<<_destMove.act<<_destMove.to<<endl;
				_resultMove=MoveMaker::translateToMove(cChess,_destMove);
				cout<<"$>> "<<_resultMove.from<<" "<<_resultMove.to<<endl;
				bestMove=_resultMove;
				finalValue=VICTORY_VALUE;
				hasHit=true;
			}
		}
		if(hasHit==true)
			return;
		else
			isStartFEN=false;
	}
	switch(alphaSearchMode)
	{
		case DepthSearch:
				_ComputerThinkForDepth();
				break;
		case TimeSearch:
				_ComputerThinkForTime(totalTimeLimit);
				break;
		default:
				throw UBException(-1,"there is no the searchMode,please check");
	}
}
Move ChessSearch::getBestMove() const
{
	return this->bestMove;
}
int ChessSearch::getFinalValue() const
{
	return this->finalValue;
}
int ChessSearch::getTotalNodes() const
{
	return this->totalNodes;
}
void ChessSearch::setSearchMode(SearchMode _searchMode)
{
	this->alphaSearchMode=_searchMode;
}
void ChessSearch::setBookMode(bool _firstForWin)
{
	if(tieTree != NULL)
	{
		tieTree->setFirstForWin(_firstForWin);
	}
}
void ChessSearch::setTotalTimeLimit(int _totalTimeLimit)
{
	this->totalTimeLimit=_totalTimeLimit;
}
void ChessSearch::makeMoveForSearch(const Move& _move)
{
	this->moveMaker.makeMove(_move);
}
void ChessSearch::_ComputerThinkForTime(int _timeLimit)
{
	int totalMoveStep=cChess->roundsNumber*2;
	if(totalMoveStep < sg_FirstMoveNumber)
	{
		sg_TimeLimit=_timeLimit/(sg_FirstMoveNumberLimit - totalMoveStep);
	}
	else if(totalMoveStep < sg_SecondMoveNumber)
	{
		sg_TimeLimit=_timeLimit/(sg_SecondMoveNumberLimit - totalMoveStep);
	}
	else if(totalMoveStep < sg_ThirdMoveNumber)
	{
		sg_TimeLimit=_timeLimit/(sg_ThirdMoveNumberLimit - totalMoveStep);
	}
	else
	{
		sg_TimeLimit=_timeLimit/10;
	}
	sg_StartTime=time(NULL);

	bestMove.from=bestMove.to=bestMove.capture=0;
	finalValue=0;
	stepNumber=0;
	finalValue=_AlphaBetaSearch(MaxSearchDepth,NEGATIVE_MIN,POSITIVE_MAX);
	if(bestMove.from == 0)
	{
		cout<<"nobestmove in alphabetasearch ..."<<endl;
	}
	else
	{
		cout<<"bestmove success...."<<endl;
	}
}
void ChessSearch::_ComputerThinkForDepth()
{
	bestMove.from=bestMove.to=bestMove.capture=0;
	finalValue=0;
	stepNumber=0;
	finalValue=_AlphaBetaSearch(MaxSearchDepth,NEGATIVE_MIN,POSITIVE_MAX);
	if(bestMove.from == 0)
	{
		cout<<"nobestmove in alphabetasearch ..."<<endl;
	}
	else
	{
		cout<<"bestmove success...."<<endl;
	}
}
//该函数递归调用
double ChessSearch::_AlphaBetaSearch(int _depth,double _alpha,double _beta)
{
	MemoTableNode _ReadMemoNode;
	bool isAlphaFlag=true;
	double goodValue=NEGATIVE_MIN;
	Move   goodMove;
	double value;
	//超时则立即返回
	if((alphaSearchMode==TimeSearch) && (time(NULL)-sg_StartTime >= sg_TimeLimit))
	{
		cout<<"out of time ..........."<<endl;
		return 3.1415926;
	}
	bool ret=readMemoTable(_depth,_alpha,_beta,&_ReadMemoNode);
	if(ret == true)
	{
		return _ReadMemoNode.evaluateValue;
	}
	else
	{
		++totalNodes;
		//小于等于0则直接估值，最后存入置换表、并返回结果
		//	此时是直接估值，所以是准确的值
		//采用静态搜索方式估值
		if(_depth <= 0)
		{
			//value=_QuiescentSearch(_alpha,_beta);
			value=evaluateSituation.evaluate();
			Move _move;_move.from=_move.to=_move.capture=0;
			MemoTableNode _SaveMemoNode;
			_SaveMemoNode.evaluateValue=value;
			_SaveMemoNode.searchDepth=_depth;
			_SaveMemoNode.nodeBestMove=_move;
			_SaveMemoNode.nodeType=allNode;
			saveMemoTable(_SaveMemoNode);
			return value;
		}
		else
		{
			//这里用数组的形式解决走法数组被覆盖的问题，这样启发策略就只能在同一层共享最佳走法
			//参考二维[256][256]对走法投票的方式
			//或者我们可以让这几个走法对象共享一个存储killerMove的内存地址来实现投票的效果
			(chessManMovePtr)->getMoveArrayLen();
			cout<<"depth:"<<_depth<<endl;
			chessManMovePtr[_depth-1].setBestMove(_ReadMemoNode.nodeBestMove);
			chessManMovePtr[_depth-1].setKillerMove(_ReadMemoNode.nodeBestMove);
			chessManMovePtr[_depth-1].genAllMove();
			bool haveSafeMove=false;
			for(int i=0;i<chessManMovePtr[_depth-1].getMoveArrayLen();i++)
			{
				Move _readMove;
				chessManMovePtr[_depth-1].getMove(i,&_readMove);
				if(isSafeMove(_readMove))
					haveSafeMove=true;
				else
					continue;
				moveMaker.makeMove(_readMove);
				//每走一步，变量自赠
				++stepNumber;
				//由于makeMove函数内已经改变走棋方，所以这里的检测自身是否被将死的函数是检测对方
				//检测将死给一个最大值，只会引起本层快速的剪支,并不会快速收敛到跟节点
				if(isCheckDie())
					value=VICTORY_VALUE-stepNumber;
				else
					value=-_AlphaBetaSearch(_depth-1,-_beta,-_alpha);
				moveMaker.unMakeMove();
				--stepNumber;
				if(value > goodValue)
				{
					goodValue=value;
					goodMove=_readMove;
					if(value > _alpha)
					{
						isAlphaFlag=false;
						_alpha=value;
						if(_depth == MaxSearchDepth)
						{
							bestMove=_readMove;
						}
					}
				}
				if(value > _beta)
				{
					MemoTableNode _SaveMemoNode;
					_SaveMemoNode.evaluateValue=_beta;
					_SaveMemoNode.searchDepth=_depth;
					_SaveMemoNode.nodeBestMove=_readMove;
					_SaveMemoNode.nodeType=betaNode;
					saveMemoTable(_SaveMemoNode);
					return _beta;
				}
			}
			if(isAlphaFlag)
			{
				MemoTableNode _SaveMemoNode;
				_SaveMemoNode.evaluateValue=_alpha;
				_SaveMemoNode.searchDepth=_depth;
				_SaveMemoNode.nodeBestMove=goodMove;
				_SaveMemoNode.nodeType=alphaNode;
				saveMemoTable(_SaveMemoNode);
			}
			else
			{
				MemoTableNode _SaveMemoNode;
				_SaveMemoNode.evaluateValue=_alpha;
				_SaveMemoNode.searchDepth=_depth;
				_SaveMemoNode.nodeBestMove=goodMove;
				_SaveMemoNode.nodeType=allNode;
				saveMemoTable(_SaveMemoNode);
			}
			return _alpha;
		}
	}
}
//静态搜索
//为了防止水平线效应
double ChessSearch::_QuiescentSearch(double _alpha,double _beta)
{
	double value,bestValue;
	//首先查看是否已經形成杀棋，如果是则直接剪支
	double best=stepNumber-VICTORY_VALUE;
	if(best > _beta)
		return best;
	//评估界面，如果超过_beta则直接剪支
	//否则进入主要搜索：搜索所有的吃子走法
	value=evaluateSituation.evaluate();
	if(value > _beta)
		return value;
	if(value > _alpha)
		_alpha=value;
	//超时则立即返回
	if((alphaSearchMode==TimeSearch) && (time(NULL)-sg_StartTime >= sg_TimeLimit))
		return 3.1415926;
	++totalNodes;
	ChessManMove	_chessManMoveForQ(cChess);
	_chessManMoveForQ.genCaptureMove();
	for(int i=0;i<_chessManMoveForQ.getMoveArrayLen();i++)
	{
		Move _move;
		_chessManMoveForQ.getMove(i,&_move);
		moveMaker.makeMove(_move);
		if(isCheckDie())
		{
			value=VICTORY_VALUE-stepNumber;
		}
		else
		{
			value=-_QuiescentSearch(-_beta,-_alpha);
		}
		moveMaker.unMakeMove();
		if(value > _beta)
			return _beta;
		if(value > best)
		{
			best=value;
			if(value > _alpha)
			{
				_alpha=value;
			}
		}
	}
	return best;
}
//该函数：
//输入：当前搜索树的深度、alpha值、beta值
//输出：true，值在_memoTableNode中,并返回
//	false,最佳移动在_memoTableNode中，有助于之后的检索
bool ChessSearch::readMemoTable(int _depth,double _alpha,double _beta,MemoTableNode* _memoTableNode)
{
	zobrist.generate();
	UBHashNode<uint32_ub,MemoTableNode> temp;
	MemoTableNode memoNode;
	Move _move;_move.from=_move.to=_move.capture=0;
	bool ret;
	temp.nodeKey=zobrist.getZobristKey();
	temp.nodeValue.zobristCheck=zobrist.getZobristCheck();
	ret=memoTable.getValue(temp,&memoNode);
	//false则说明在置换表中不存在，则直接返回，注意最佳移动的表示
	if(ret == false)
	{
		_memoTableNode->nodeType=nullNode;
		_memoTableNode->nodeBestMove=_move;
		return false;
	}
	//找到了，但是需要注意深度，alpha、beta
	else
	{
		if(memoNode.searchDepth >= _depth)
		{
			//这里对获胜局面的值进行绝对值->相对值的转换
			//						即：考虑深度
			double absoluteValue=0;
			if(memoNode.evaluateValue > (VICTORY_VALUE-STEPNUMBER_MAX))
			{
				absoluteValue=memoNode.evaluateValue-stepNumber;
			}
			else if(memoNode.evaluateValue < -(VICTORY_VALUE-STEPNUMBER_MAX))
			{
				absoluteValue=memoNode.evaluateValue+stepNumber;
			}
			else
			{
				absoluteValue=memoNode.evaluateValue;
			}
			if((memoNode.nodeType == alphaNode) && (absoluteValue <= _alpha))
			{
				_memoTableNode->evaluateValue=_alpha;
				return true;
			}
			if((memoNode.nodeType == betaNode) && (absoluteValue >= _beta))
			{
				_memoTableNode->evaluateValue=_beta;
				return true;
			}
			if(memoNode.nodeType == allNode)
			{
				_memoTableNode->evaluateValue=absoluteValue;
				return true;
			}
		}
		//如果都没有找到需要的值，则我们取其中的最佳路径
		_memoTableNode->nodeType=nullNode;
		_memoTableNode->nodeBestMove=memoNode.nodeBestMove;
		return false;
	}
}
//将节点信息计入置换表中
//主要用到节点中的后四个字段
bool ChessSearch::saveMemoTable(const MemoTableNode& _memoTableNode)
{
	zobrist.generate();
	//这里将将死局面的分数有相对值->绝对值
	MemoTableNode memoNode;
	memoNode=_memoTableNode;
	if(_memoTableNode.evaluateValue > (VICTORY_VALUE-STEPNUMBER_MAX))
	{
		memoNode.evaluateValue=_memoTableNode.evaluateValue+stepNumber;
	}
	else if(_memoTableNode.evaluateValue < -(VICTORY_VALUE-STEPNUMBER_MAX))
	{
		memoNode.evaluateValue=_memoTableNode.evaluateValue-stepNumber;
	}
	else
	{
		memoNode.evaluateValue=_memoTableNode.evaluateValue;
	}
	UBHashNode<uint32_ub,MemoTableNode> temp;
	bool ret;
	temp.nodeKey=zobrist.getZobristKey();
	temp.nodeValue=memoNode;
	temp.nodeValue.zobristCheck=zobrist.getZobristCheck();
	ret=memoTable.insertNode(temp);
	//如果返回false,则说明已经存在，但是走到这一步说明已经无效，则应该更新
	if(ret==false)
	{
		memoTable.updateNode(temp);
	}
	return true;
}
void ChessSearch::clearMemoTable()
{
	memoTable.clearHashTable();
}
bool ChessSearch::isNullMove(const Move& _move) const
{
	return ((_move.from == 0) && (_move.to == 0) && (_move.capture == 0));
}
//将死：被将军且任何走法都将军
bool ChessSearch::isCheckDie()
{
	if(chessManMovePtr[MaxSearchDepth-1].generalOpposite())
		return true;
	
	bool isNoSolution=false;
	if(chessManMovePtr[MaxSearchDepth-1].check())
	{
		isNoSolution=true;
		//为了防治用原来的ChessManMove引发覆盖原来的数组内容，这里我们重新申请一个对象
		ChessManMove	_chessManMoveForCheck(cChess);
		_chessManMoveForCheck.genAllMove();
		for(int i=0;i<_chessManMoveForCheck.getMoveArrayLen();i++)
		{
			Move _move;
			_chessManMoveForCheck.getMove(i,&_move);
			//由于makeMove会改变走棋方，但是判断函数check是判断自己是否被将军，所以这里需要改变chess中的isRed变量
			moveMaker.makeMove(_move);
			cChess->isRed=!cChess->isRed;
				if(!_chessManMoveForCheck.check())
				{
					isNoSolution=false;
				}
			cChess->isRed=!cChess->isRed;
			moveMaker.unMakeMove();
			if(isNoSolution == false)
				break;
		}
	}
	return isNoSolution;
}
bool ChessSearch::isSafeMove(const Move& _move)
{
	int fromChessManValue=cChess->cChessArray[_move.from];
	if(fromChessManValue==0)
	{
		cout<<"There no chessMan in _move.from :"<<_move.from<<endl;
		return false;
	}
	bool flag=true;
	bool oldIsRed=cChess->isRed;
		moveMaker.makeMove(_move);
			cChess->isRed=!cChess->isRed;
			if(chessManMovePtr[MaxSearchDepth-1].check())
				flag=false;
			if(flag && chessManMovePtr[MaxSearchDepth-1].generalOpposite())
				flag=false;
		moveMaker.unMakeMove();  
	cChess->isRed=oldIsRed;
	return flag;
}
void ChessSearch::initTieTree()
{
	Txt2Struct _txt2Struct;
	string _baseBookString="./Book/MyStdio/Book_";
	string _extBookString=".txt";
	for(int i=1;i<=30;i++)
	{
		string fileName=_baseBookString+UBString::toString(i).getData()+_extBookString;
		_txt2Struct.convertFunction(fileName);
	}
	tieTree=new UBTieTree(_txt2Struct.getStructVector());
}
