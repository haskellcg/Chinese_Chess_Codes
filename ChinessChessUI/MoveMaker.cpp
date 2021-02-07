#include "MoveMaker.h"
#include "UBException.h"
#include <iostream>
using std::cout;
using std::endl;
//判断是否为红方棋子，前提有棋子且位置合法
bool MoveMaker::isRedChessMan(int position) const
{
	if(position<0 || position>255)
	{
		cout<<"Position Out of bound in MoveHistory !"<<endl;
		return false;
	}
	if(cChess->cChessArray[position]==0)
	{
		cout<<"No Chess Man in this position !"<<endl;
		return false;
	}
	return (cChess->cChessArray[position] <32);
}
//对于move中的数据进行的基本的验证
bool MoveMaker::isLegalMove(const Move& _move) const
{
	return (_move.from>=0 && _move.from<=255 && _move.to>=0 && _move.to<=255 && 
		((_move.capture>=16 && _move.capture<=47) || _move.capture==0));
}
MoveMaker::MoveMaker():cChess(NULL)
{
	CChessStruct* temp=new CChessStruct;
	this->cChess=makeSmartPtr(temp);
	this->moveHistory=new Move[moveHistoryInitialCapacity];
	this->counterHistory=new RoundsCapture[moveHistoryInitialCapacity];
	this->moveHistoryLen=0;
	this->moveHistoryLimit=moveHistoryInitialCapacity;
	this->unMakeIsHistory=false;
	this->lastNewIndex=0;
	this->lastIsUnMake=false;
	this->lastIsReMake=false;
	this->lastVisitIndex=0;
	this->chessIsNewForMe=true;
}
MoveMaker::MoveMaker(SmartPtr<CChessStruct> _cChess):cChess(_cChess)
{
	if(_cChess.getPtr() == NULL)
	{
		CChessStruct* temp=new CChessStruct;
		this->cChess=makeSmartPtr(temp);
	}
	this->moveHistory=new Move[moveHistoryInitialCapacity];
	this->counterHistory=new RoundsCapture[moveHistoryInitialCapacity];
	this->moveHistoryLen=0;
	this->moveHistoryLimit=moveHistoryInitialCapacity;
	this->unMakeIsHistory=false;
	this->lastNewIndex=0;
	this->lastIsUnMake=false;
	this->lastIsReMake=false;
	this->lastVisitIndex=0;
	this->chessIsNewForMe=true;
}
MoveMaker::MoveMaker(const MoveMaker& _moveMaker):cChess(_moveMaker.cChess)
{
	this->moveHistory=new Move[_moveMaker.moveHistoryLimit];
	this->counterHistory=new RoundsCapture[_moveMaker.moveHistoryLimit];
	for(int i=0;i<_moveMaker.moveHistoryLen;i++)
	{
		this->moveHistory[i].from=_moveMaker.moveHistory[i].from;
		this->moveHistory[i].to=_moveMaker.moveHistory[i].to;
		this->moveHistory[i].capture=_moveMaker.moveHistory[i].capture;
		this->counterHistory[i].movedCaptureStepNumber=_moveMaker.counterHistory[i].movedCaptureStepNumber;
		this->counterHistory[i].movedRoundsNumber=_moveMaker.counterHistory[i].movedRoundsNumber;
	}
	this->moveHistoryLen=_moveMaker.moveHistoryLen;
	this->moveHistoryLimit=_moveMaker.moveHistoryLimit;
	this->unMakeIsHistory=_moveMaker.unMakeIsHistory;
	this->lastNewIndex=_moveMaker.lastNewIndex;
	this->lastIsUnMake=_moveMaker.lastIsUnMake;
	this->lastIsReMake=_moveMaker.lastIsReMake;
	this->lastVisitIndex=_moveMaker.lastVisitIndex;
	this->chessIsNewForMe=_moveMaker.chessIsNewForMe;
}
MoveMaker& MoveMaker::operator=(const MoveMaker& _moveMaker)
{
	if(_moveMaker.moveHistoryLen <= this->moveHistoryLimit)
	{
		for(int i=0;i<_moveMaker.moveHistoryLen;i++)
		{
			this->moveHistory[i].from=_moveMaker.moveHistory[i].from;
			this->moveHistory[i].to=_moveMaker.moveHistory[i].to;
			this->moveHistory[i].capture=_moveMaker.moveHistory[i].capture;
			this->counterHistory[i].movedCaptureStepNumber=_moveMaker.counterHistory[i].movedCaptureStepNumber;
			this->counterHistory[i].movedRoundsNumber=_moveMaker.counterHistory[i].movedRoundsNumber;
		}
		this->moveHistoryLen=_moveMaker.moveHistoryLen;
		this->unMakeIsHistory=_moveMaker.unMakeIsHistory;
	}
	else
	{
		do
		{
			this->moveHistoryLimit*=2;
		}while(_moveMaker.moveHistoryLen > this->moveHistoryLimit);
		delete[] this->moveHistory;
		delete[] this->counterHistory;
		this->moveHistory=new Move[this->moveHistoryLimit];
		this->counterHistory=new RoundsCapture[this->moveHistoryLimit];
		for(int i=0;i<_moveMaker.moveHistoryLen;i++)
		{
			this->moveHistory[i].from=_moveMaker.moveHistory[i].from;
			this->moveHistory[i].to=_moveMaker.moveHistory[i].to;
			this->moveHistory[i].capture=_moveMaker.moveHistory[i].capture;
			this->counterHistory[i].movedCaptureStepNumber=_moveMaker.counterHistory[i].movedCaptureStepNumber;
			this->counterHistory[i].movedRoundsNumber=_moveMaker.counterHistory[i].movedRoundsNumber;
		}
		this->moveHistoryLen=_moveMaker.moveHistoryLen;
		this->unMakeIsHistory=_moveMaker.unMakeIsHistory;
	}
	this->lastNewIndex=_moveMaker.lastNewIndex;
	this->lastIsUnMake=_moveMaker.lastIsUnMake;
	this->lastIsReMake=_moveMaker.lastIsReMake;
	this->lastVisitIndex=_moveMaker.lastVisitIndex;
	return *this;
}
MoveMaker::~MoveMaker()
{
	if(this->moveHistory != NULL)
		delete[] this->moveHistory;
}
void MoveMaker::bindStruct(SmartPtr<CChessStruct> _cChess)
{
	if(_cChess.getPtr() != NULL)
	{
		this->cChess=_cChess;
		this->moveHistoryLen=0;
	}
	this->lastNewIndex=0;
	this->lastIsUnMake=false;
	this->lastIsReMake=false;
	this->lastVisitIndex=0;
	this->chessIsNewForMe=true;
}
bool MoveMaker::makeMove(const Move& _move)
{
	//验证参数的合法性
	if(!isLegalMove(_move))
	{
		cout<<"Not Legal Move ."<<endl;
		return false;
	}
	if(cChess->cChessArray[_move.from] ==0)
	{
		cout<<"there is no source chessMan .."<<endl;
		return false;
	}
	//在内存中做出实际的移动
	int fromChessManValue=cChess->cChessArray[_move.from];
	cChess->cChessArray[_move.from]=0;
	cChess->cChessArray[_move.to]=fromChessManValue;
	cChess->cChessManPosition[fromChessManValue-16]=_move.to;
	//改变CChessStruct的成员
	if(cChess->isRed == 1 && !chessIsNewForMe)
		++(cChess->roundsNumber);
	cChess->isRed=!cChess->isRed;
	if(_move.capture!=0)
	{
		cChess->cChessManPosition[_move.capture-16]=0;
		cChess->captureStepNumber=0;
	}
	else
	{
		++cChess->captureStepNumber;
	}
	//将移动以及吃字数、回合书记入历史
	if(this->moveHistoryLen >= this->moveHistoryLimit)
	{
		Move* tempPtr=this->moveHistory;
		RoundsCapture* counterTempPtr=this->counterHistory;
		do
		{
			this->moveHistoryLimit*=2;
		}while(this->moveHistoryLen >= this->moveHistoryLimit);
		this->moveHistory=new Move[this->moveHistoryLimit];
		this->counterHistory=new RoundsCapture[this->moveHistoryLimit];
		for(int i=0;i<this->moveHistoryLen;i++)
		{
			this->moveHistory[i]=tempPtr[i];
			this->counterHistory[i]=counterTempPtr[i];
		}
		delete[] tempPtr;
		delete[] counterTempPtr;
	}
	this->moveHistory[this->moveHistoryLen]=_move;
	this->counterHistory[this->moveHistoryLen].movedCaptureStepNumber=cChess->captureStepNumber;
	this->counterHistory[this->moveHistoryLen].movedRoundsNumber=cChess->roundsNumber;
	++this->moveHistoryLen;
	//标识变量的赋值，后面的撤销策略需要用到
	this->lastNewIndex=this->moveHistoryLen-1;
	this->lastVisitIndex=this->moveHistoryLen-1;
	this->lastIsUnMake=false;
	this->lastIsReMake=false;
	this->chessIsNewForMe=false;
	return true;
}
//撤销动作，在移动后、以及re移动后均可使用
//由于在re移动后lastVisitIndex指向下一个要恢复的节点，但是对于该函数应该访问已经恢复的节点，所以-1
//并且调用后指向下一个要撤销的节点
//对于常规撤销策略，这些不用考虑，只是为适应新策略，需要设置lastVisitIndex的值，虽然本身策略不需要，但是若策略切换就很有用
bool MoveMaker::unMakeMove()
{
	//验证合法性
	if(moveHistoryLen==0)
	{
		cout<<"No Move in MoveHistory...."<<endl;
		return false;
	}
	//撤销动作算作历史的撤销策略
	if(unMakeIsHistory)
	{
		if(lastIsReMake)
			--lastVisitIndex;
		if(lastVisitIndex<0)
			return false;
		//撤销的做动记录到历史中需要注意：
		//				吃子问题：由于cature只是记录了from到to的吃子，但是在撤销时会遇到to到from的情况，所以
		//					  吃子时应该注意检测对应位置的棋子
		//				记录时要调换顺序
		Move temp=this->moveHistory[this->lastVisitIndex];
		Move _moveForRecord;
		RoundsCapture counterTemp;
		if(lastVisitIndex == 0)
			counterTemp.movedCaptureStepNumber=counterTemp.movedRoundsNumber=0;
		else
			counterTemp=this->counterHistory[this->lastVisitIndex-1];
		int fromChessValue=cChess->cChessArray[temp.from];
		int toChessManValue=cChess->cChessArray[temp.to];
		_moveForRecord.from=temp.to;
		_moveForRecord.to=temp.from;
		_moveForRecord.capture=fromChessValue;
		cChess->cChessArray[temp.from]=toChessManValue;
		if(fromChessValue != 0)
			cChess->cChessManPosition[fromChessValue-16]=0;
		cChess->cChessArray[temp.to]=temp.capture;
		cChess->cChessManPosition[toChessManValue-16]=temp.from;
		cChess->isRed=!cChess->isRed;
		cChess->roundsNumber=counterTemp.movedRoundsNumber;
		cChess->captureStepNumber=counterTemp.movedCaptureStepNumber;
		if(temp.capture!=0)
			cChess->cChessManPosition[temp.capture-16]=temp.to;
		
		if(this->moveHistoryLen >= this->moveHistoryLimit)
		{
			Move* tempPtr=this->moveHistory;
			RoundsCapture* counterTempPtr=this->counterHistory;
			do
			{
				this->moveHistoryLimit*=2;
			}while(this->moveHistoryLen >= this->moveHistoryLimit);
			this->moveHistory=new Move[this->moveHistoryLimit];
			this->counterHistory=new RoundsCapture[this->moveHistoryLimit];
			for(int i=0;i<this->moveHistoryLen;i++)
			{
				this->moveHistory[i]=tempPtr[i];
				this->counterHistory[i]=counterTempPtr[i];
			}
			delete[] tempPtr;
			delete[] counterTempPtr;
		}
		this->moveHistory[this->moveHistoryLen]=_moveForRecord;
		this->counterHistory[this->moveHistoryLen]=counterTemp;
		++this->moveHistoryLen;
		--this->lastVisitIndex;
	}
	else
	{
		--this->moveHistoryLen;
		Move temp=this->moveHistory[this->moveHistoryLen];
		RoundsCapture counterTemp=this->counterHistory[this->moveHistoryLen-1];
		int toChessManValue=cChess->cChessArray[temp.to];
		cChess->cChessArray[temp.from]=toChessManValue;
		if(temp.capture!=0)
			cChess->cChessArray[temp.to]=temp.capture;
		else
			cChess->cChessArray[temp.to]=0;
		cChess->cChessManPosition[toChessManValue-16]=temp.from;
		if(temp.capture!=0)
			cChess->cChessManPosition[temp.capture-16]=temp.to;
		cChess->isRed=!cChess->isRed;
		cChess->roundsNumber=counterTemp.movedRoundsNumber;
		cChess->captureStepNumber=counterTemp.movedCaptureStepNumber;
		this->lastVisitIndex=this->moveHistoryLen-1;
	}
	this->lastIsUnMake=true;
	this->lastIsReMake=false;
	return true;
}
bool MoveMaker::reMakeMove()
{
	if(!lastIsUnMake && !lastIsReMake)
		return false;
	if(unMakeIsHistory)
	{
		if(lastIsUnMake)
			++lastVisitIndex;
		if(lastVisitIndex>lastNewIndex)
			return false;
		Move _move=this->moveHistory[lastVisitIndex];
		RoundsCapture counterTemp=this->counterHistory[lastVisitIndex];
		int fromChessManValue=cChess->cChessArray[_move.from];
		cChess->cChessArray[_move.from]=0;
		cChess->cChessArray[_move.to]=fromChessManValue;
		cChess->cChessManPosition[fromChessManValue-16]=_move.to;
		if(_move.capture!=0)
			cChess->cChessManPosition[_move.capture-16]=0;
		cChess->isRed=!cChess->isRed;
		cChess->roundsNumber=counterTemp.movedRoundsNumber;
		cChess->captureStepNumber=counterTemp.movedCaptureStepNumber;
		
		if(this->moveHistoryLen >= this->moveHistoryLimit)
		{
			Move* tempPtr=this->moveHistory;
			RoundsCapture* counterTempPtr=this->counterHistory;
			do
			{
				this->moveHistoryLimit*=2;
			}while(this->moveHistoryLen >= this->moveHistoryLimit);
			this->moveHistory=new Move[this->moveHistoryLimit];
			this->counterHistory=new RoundsCapture[this->moveHistoryLimit];
			for(int i=0;i<this->moveHistoryLen;i++)
			{
				this->moveHistory[i]=tempPtr[i];
				this->counterHistory[i]=counterTempPtr[i];
			}
			delete[] tempPtr;
			delete[] counterTempPtr;
		}
		this->moveHistory[this->moveHistoryLen]=_move;
		this->counterHistory[this->moveHistoryLen]=counterTemp;
		++this->moveHistoryLen;
		++this->lastVisitIndex;
	}
	else
	{
		if(lastVisitIndex <= lastNewIndex)
		{
			Move temp=this->moveHistory[this->moveHistoryLen];
			RoundsCapture counterTemp=this->counterHistory[this->moveHistoryLen];
			++this->moveHistoryLen;
			int fromChessManValue=cChess->cChessArray[temp.from];
			cChess->cChessArray[temp.from]=0;
			cChess->cChessArray[temp.to]=fromChessManValue;
			cChess->cChessManPosition[fromChessManValue-16]=temp.to;
			if(temp.capture!=0)
				cChess->cChessManPosition[temp.capture]=0;
			cChess->isRed=!cChess->isRed;
			cChess->roundsNumber=counterTemp.movedRoundsNumber;
			cChess->captureStepNumber=counterTemp.movedCaptureStepNumber;
			lastVisitIndex=this->moveHistoryLen;
		}
		else
			return false;
	}
	this->lastIsUnMake=false;
	this->lastIsReMake=true;
	return true;
}
int MoveMaker::getMoveHistoryLen() const
{
	return this->moveHistoryLen;
}
bool MoveMaker::getHistoryMove(int index,Move* _move) const
{
	if(index<0 || index>this->moveHistoryLen-1)
	{
		cout<<"Out Of bound in getHistoryMove..."<<endl;
		return false;
	}
	_move->from=this->moveHistory[index].from;
	_move->to=this->moveHistory[index].to;
	_move->capture=this->moveHistory[index].capture;
	return true;
}
void MoveMaker::setHistoryDecision(bool _unMakeIsHistory)
{
	this->unMakeIsHistory=_unMakeIsHistory;
}
void MoveMaker::report() const
{
	for(int i=0;i<moveHistoryLen;i++)
	{
		cout<<"["<<(i+1)<<"]:";
		cout<<moveHistory[i].from<<"-"<<moveHistory[i].to<<"-"<<moveHistory[i].capture<<endl;
	}
	cout<<"HistoryLen:"<<moveHistoryLen<<endl;
	cout<<"HistoryLimit:"<<moveHistoryLimit<<endl;
	cout<<"unMakeIsHistory:"<<unMakeIsHistory<<endl;
	cout<<"lastNewIndex:"<<lastNewIndex<<endl;
	cout<<"lastVisitIndex:"<<lastVisitIndex<<endl;
	cout<<"lastIsUnMake:"<<lastIsUnMake<<endl;
	cout<<"lastIsReMake:"<<lastIsReMake<<endl;
}
ICCSMove MoveMaker::translateToICCS(const Move& _move)
{
	int fromRow=_move.from/16;
	int fromCol=_move.from%16;
	int toRow=_move.to/16;
	int toCol=_move.to%16;
	char _fromX=(char)('a'+(fromCol-3));
	char _fromY=(char)('9'-(fromRow-3));
	char _toX=(char)('a'+(toCol-3));
	char _toY=(char)('9'-(toRow-3));
	ICCSMove temp;
	temp.fromX=_fromX;	temp.fromY=_fromY;
	temp.toX=_toX;		temp.toY=_toY;
	return temp;
}
Move MoveMaker::translateToMove(SmartPtr<CChessStruct> _cChess,const ICCSMove& _iccsMove)
{
	if(_cChess.getPtr() == NULL)
		throw UBException(-1,"CChessStruct object is null");
	int _fromRow=('9'-_iccsMove.fromY)+3;
	int _fromCol=(_iccsMove.fromX-'a')+3;
	int _toRow=('9'-_iccsMove.toY)+3;
	int _toCol=(_iccsMove.toX-'a')+3;
	Move temp;
	temp.from=_fromRow*16+_fromCol;
	temp.to=_toRow*16+_toCol;
	if(_cChess->cChessArray[temp.from] ==0)
		throw UBException(-1,"the cChess don't match the iccsMove");
	temp.capture=_cChess->cChessArray[temp.to];
	return temp;
}
//这里需要统计除将、士、象之外的在一条线上的棋子，因为表示在中文格式中完全不同与常规的表示
//其次就是要注意卒的表示
//3卒一条线、2列都有两个以上卒表示遵循前中后、一二三等，一二三遵循从右到左，从上到下
//需要注意，上面的四个变量都是一红方在大端的形式表示的，所以如果相反则需要转换
//前后：这里的前后是相对于每一个走棋方
//多卒一线：是以红方为标准的从左到右，从上到下
ChinessMove MoveMaker::translateToChinessZX(SmartPtr<CChessStruct> _cChess,const Move& _move)
{
	if(_cChess.getPtr() == NULL)
		throw UBException(-1,"CChessStruct object is null");
	if(_cChess->cChessArray[_move.from]==0)
		throw UBException(-1,"the cChess don't match the iccsMove");
	int fromRow=_move.from/16;
	int fromCol=_move.from%16;
	int toRow=_move.to/16;
	int toCol=_move.to%16;
	int chessManValue=_cChess->cChessArray[_move.from];
	bool isRedOnBigEnd=false;
	bool isRedChessMan=false;
	//为多兵一条线、两条线
	//以及其他棋子一条线做准备
	int  firstColSameInSmallEnd=0;
	int  firstColSameInBigEnd=0;
	int  secondColSameInSmallEnd=0;
	int  secondColSameInBigEnd=0;
	if(_cChess->cChessManPosition[0] > 8*16)
	{
		isRedOnBigEnd=true;
	}
	if(chessManValue < 32)
	{
		isRedChessMan=true;
	}
	if(isRedChessMan && chessManValue!=redAdvisorOne && chessManValue!=redAdvisorTwo && chessManValue!=redBishopOne &&
			    chessManValue!=redBishopTwo && chessManValue!=redKing)
	{
		if(chessManValue!=redPawnOne && chessManValue!=redPawnTwo && chessManValue!=redPawnThree && 
						chessManValue!=redPawnThree && chessManValue!=redPawnFive)
		{
			int anotherChessManValue=0;
			int anotherChessManPosition=0;
			if(chessManValue==redKnightOne)
				anotherChessManValue=redKnightTwo;
			else if(chessManValue==redKnightTwo)
				anotherChessManValue=redKnightOne;
			else if(chessManValue==redRookOne)
				anotherChessManValue=redRookTwo;
			else if(chessManValue==redRookTwo)
				anotherChessManValue=redRookOne;
			else if(chessManValue==redCannonOne)
				anotherChessManValue=redCannonTwo;
			else
				anotherChessManValue=redCannonOne;
			anotherChessManPosition=_cChess->cChessManPosition[anotherChessManValue-16];
			if(anotherChessManPosition!=0)
			{
				if(anotherChessManPosition%16 == fromCol)
				{
					if(anotherChessManPosition < _move.from)
						++firstColSameInSmallEnd;
					else
						++firstColSameInBigEnd;
				}
			}
		}
		else
		{
			for(int i=11;i<=15;i++)	
			{
				if((chessManValue-16) != i && _cChess->cChessManPosition[i]!=0 &&
							      _cChess->cChessManPosition[i]%16==fromCol)
				{
					if(_cChess->cChessManPosition[i] <_move.from)
						++firstColSameInSmallEnd;
					else
						++firstColSameInBigEnd;
				}
			}
			if(firstColSameInSmallEnd+firstColSameInBigEnd!=0)
			{
				for(int i=11;i<=15;i++)
				{
					int onePawnPosition=_cChess->cChessManPosition[i];
					bool isBigThanFrom=false;
					if(onePawnPosition%16 != fromCol)
					{
						if(onePawnPosition%16 > fromCol)
							isBigThanFrom=true;
						for(int j=i+1;j<=15;j++)
						{
							int anotherPawnPosition=_cChess->cChessManPosition[j];
							if(anotherPawnPosition%16 == onePawnPosition%16)
							{
								if(isBigThanFrom)
									++secondColSameInBigEnd;
								else
									++secondColSameInSmallEnd;
							}
						}
						if(secondColSameInBigEnd!=0)
						{
							++secondColSameInBigEnd;
							break;
						}
						if(secondColSameInSmallEnd!=0)
						{
							++secondColSameInSmallEnd;
							break;
						}
							
					}
				}
			}
		}
		
	}
	else if(!isRedChessMan && chessManValue!=blackAdvisorOne && chessManValue!=blackAdvisorTwo &&
				  chessManValue!=blackBishopOne  && chessManValue!=blackBishopTwo && 
				  chessManValue!=blackKing)
	{
		if(chessManValue!=blackPawnOne && chessManValue!=blackPawnTwo && chessManValue!=blackPawnThree && 
						chessManValue!=blackPawnThree && chessManValue!=blackPawnFive)
		{
			int anotherChessManValue=0;
			int anotherChessManPosition=0;
			if(chessManValue==blackKnightOne)
				anotherChessManValue=blackKnightTwo;
			else if(chessManValue==blackKnightTwo)
				anotherChessManValue=blackKnightOne;
			else if(chessManValue==blackRookOne)
				anotherChessManValue=blackRookTwo;
			else if(chessManValue==blackRookTwo)
				anotherChessManValue=blackRookOne;
			else if(chessManValue==blackCannonOne)
				anotherChessManValue=blackCannonTwo;
			else
				anotherChessManValue=blackCannonOne;
			anotherChessManPosition=_cChess->cChessManPosition[anotherChessManValue-16];
			if(anotherChessManPosition!=0)
			{
				if(anotherChessManPosition%16 == fromCol)
				{
					if(anotherChessManPosition < _move.from)
						++firstColSameInSmallEnd;
					else
						++firstColSameInBigEnd;
				}
			}
		}
		else
		{
			for(int i=27;i<=31;i++)	
			{
				if((chessManValue-16) != i && _cChess->cChessManPosition[i]!=0 &&
							      _cChess->cChessManPosition[i]%16==fromCol)
				{
					if(_cChess->cChessManPosition[i] <_move.from)
						++firstColSameInSmallEnd;
					else
						++firstColSameInBigEnd;
				}
			}
			if(firstColSameInSmallEnd+firstColSameInBigEnd!=0)
			{
				for(int i=27;i<=31;i++)
				{
					int onePawnPosition=_cChess->cChessManPosition[i];
					bool isBigThanFrom=false;
					if(onePawnPosition%16 != fromCol)
					{
						if(onePawnPosition%16 > fromCol)
							isBigThanFrom=true;
						for(int j=i+1;j<=31;j++)
						{
							int anotherPawnPosition=_cChess->cChessManPosition[j];
							if(anotherPawnPosition%16 == onePawnPosition%16)
							{
								if(isBigThanFrom)
									++secondColSameInBigEnd;
								else
									++secondColSameInSmallEnd;
							}
						}
						if(secondColSameInBigEnd!=0)
						{
							++secondColSameInBigEnd;
							break;
						}
						if(secondColSameInSmallEnd!=0)
						{
							++secondColSameInSmallEnd;
							break;
						}
							
					}
				}
			}
		}
	}
	UBString chessManName=getChessManChinessName(chessManValue);
	ChinessMove temp;
	UBString actStr,sourceStr,destStr;
	if(isRedChessMan && isRedOnBigEnd)
	{
		if(chessManValue == redAdvisorOne || chessManValue == redAdvisorTwo ||
					       chessManValue == redBishopOne  || chessManValue == redBishopTwo)
		{
			if(_move.from > _move.to)
				actStr="进";
			else
				actStr="退";
			sourceStr=getNumberStr(12-fromCol);
			destStr=getNumberStr(12-toCol);
			temp.chessMan=chessManName;
			temp.from=sourceStr;
			temp.act=actStr;
			temp.to=destStr;
		}
		else if(chessManValue == redKing)
		{
			sourceStr=getNumberStr(12-fromCol);
			if(fromCol == toCol)
			{
				if(fromRow > toRow)
				{
					actStr="进";
					destStr=getNumberStr(fromRow-toRow);
				}
				else 
				{
					actStr="退";
					destStr=getNumberStr(toRow-fromRow);
				}
			}
			else
			{
					actStr="平";
					destStr=getNumberStr(12-toCol);
			}
			temp.chessMan=chessManName;
			temp.from=sourceStr;
			temp.act=actStr;
			temp.to=destStr;
		}
		else if(chessManValue == redKnightOne || chessManValue == redKnightTwo || chessManValue == redRookOne ||
			chessManValue == redRookTwo   || chessManValue == redCannonOne || chessManValue == redCannonTwo)
		{
			if(firstColSameInBigEnd + firstColSameInSmallEnd ==0)
			{
				sourceStr=getNumberStr(12-fromCol);
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="进";
						destStr=getNumberStr(fromRow-toRow);
					}
					else 
					{
						actStr="退";
						destStr=getNumberStr(toRow-fromRow);
					}
				}
				else if(fromRow == toRow)
				{
						actStr="平";
						destStr=getNumberStr(12-toCol);
				}
				else
				{
					if(_move.from > _move.to)
						actStr="进";
					else
						actStr="退";
					destStr=getNumberStr(12-toCol);
				}
				temp.chessMan=chessManName;
				temp.from=sourceStr;
				temp.act=actStr;
				temp.to=destStr;
			}
			else
			{
				if(firstColSameInBigEnd!=0)
					sourceStr="前";
				else
					sourceStr="后";
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="进";
						destStr=getNumberStr(fromRow-toRow);
					}
					else 
					{
						actStr="退";
						destStr=getNumberStr(toRow-fromRow);
					}
				}
				else if(fromRow == toRow)
				{
						actStr="平";
						destStr=getNumberStr(12-toCol);
				}
				else
				{
					if(_move.from > _move.to)
						actStr="进";
					else
						actStr="退";
					destStr=getNumberStr(12-toCol);
				}
				//为了便于显示，这里按照中文的表示法安排顺序
				temp.chessMan=sourceStr;
				temp.from=chessManName;
				temp.act=actStr;
				temp.to=destStr;
			}
		}
		//处理兵的部分
		else
		{
			if(firstColSameInBigEnd+firstColSameInSmallEnd==0)
			{
				sourceStr=getNumberStr(12-fromCol);
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="进";
						destStr=getNumberStr(fromRow-toRow);
					}
					else 
					{
						actStr="退";
						destStr=getNumberStr(toRow-fromRow);
					}
				}
				else
				{
						actStr="平";
						destStr=getNumberStr(12-toCol);
				}
				temp.chessMan=chessManName;
				temp.from=sourceStr;
				temp.act=actStr;
				temp.to=destStr;
			}
			else
			{
				if((firstColSameInBigEnd+firstColSameInSmallEnd==2) && (secondColSameInBigEnd+secondColSameInSmallEnd==0))
				{
					if(firstColSameInSmallEnd==0)
						sourceStr="前";
					else if(firstColSameInSmallEnd==1)
						sourceStr="中";
					else
						sourceStr="后";
				}
				else if(firstColSameInBigEnd+firstColSameInSmallEnd>=3)
				{
					sourceStr=getNumberStr(firstColSameInSmallEnd+1);
				}
				else if((firstColSameInBigEnd+firstColSameInSmallEnd!=0) && 
					(secondColSameInBigEnd+secondColSameInSmallEnd!=0))
				{
					sourceStr=getNumberStr(secondColSameInBigEnd+firstColSameInSmallEnd+1);
				}
				else
				{
					if(firstColSameInBigEnd!=0)
						sourceStr="前";
					else
						sourceStr="后";
				}
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="进";
						destStr=getNumberStr(fromRow-toRow);
					}
					else 
					{
						actStr="退";
						destStr=getNumberStr(toRow-fromRow);
					}
				}
				else
				{
						actStr="平";
						destStr=getNumberStr(12-toCol);
				}
				temp.chessMan=sourceStr;
				temp.from=chessManName;
				temp.act=actStr;
				temp.to=destStr;
			}
		}
	}
	else if(isRedChessMan && !isRedOnBigEnd)
	{
		if(chessManValue == redAdvisorOne || chessManValue == redAdvisorTwo ||
					       chessManValue == redBishopOne  || chessManValue == redBishopTwo)
		{
			if(_move.from > _move.to)
				actStr="退";
			else
				actStr="进";
			sourceStr=getNumberStr(fromCol-2);
			destStr=getNumberStr(toCol-2);
			temp.chessMan=chessManName;
			temp.from=sourceStr;
			temp.act=actStr;
			temp.to=destStr;
		}
		else if(chessManValue == redKing)
		{
			sourceStr=getNumberStr(fromCol-2);
			if(fromCol == toCol)
			{
				if(fromRow > toRow)
				{
					actStr="退";
					destStr=getNumberStr(fromRow-toRow);
				}
				else 
				{
					actStr="进";
					destStr=getNumberStr(toRow-fromRow);
				}
			}
			else
			{
					actStr="平";
					destStr=getNumberStr(toCol-2);
			}
			temp.chessMan=chessManName;
			temp.from=sourceStr;
			temp.act=actStr;
			temp.to=destStr;
		}
		else if(chessManValue == redKnightOne || chessManValue == redKnightTwo || chessManValue == redRookOne ||
			chessManValue == redRookTwo   || chessManValue == redCannonOne || chessManValue == redCannonTwo)
		{
			if(firstColSameInBigEnd + firstColSameInSmallEnd ==0)
			{
				sourceStr=getNumberStr(fromCol-2);
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="退";
						destStr=getNumberStr(fromRow-toRow);
					}
					else 
					{
						actStr="进";
						destStr=getNumberStr(toRow-fromRow);
					}
				}
				else if(fromRow == toRow)
				{
						actStr="平";
						destStr=getNumberStr(toCol-2);
				}
				else
				{
					if(_move.from < _move.to)
						actStr="进";
					else
						actStr="退";
					destStr=getNumberStr(toCol-2);
				}
				temp.chessMan=chessManName;
				temp.from=sourceStr;
				temp.act=actStr;
				temp.to=destStr;
			}
			else
			{
				if(firstColSameInBigEnd!=0)
					sourceStr="后";
				else
					sourceStr="前";
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="退";
						destStr=getNumberStr(fromRow-toRow);
					}
					else 
					{
						actStr="进";
						destStr=getNumberStr(toRow-fromRow);
					}
				}
				else if(fromRow == toRow)
				{
						actStr="平";
						destStr=getNumberStr(toCol-2);
				}
				else
				{
					if(_move.from < _move.to)
						actStr="进";
					else
						actStr="退";
					destStr=getNumberStr(toCol-2);
				}
				temp.chessMan=sourceStr;
				temp.from=chessManName;
				temp.act=actStr;
				temp.to=destStr;
			}
		}
		else
		{
			if(firstColSameInBigEnd+firstColSameInSmallEnd==0)
			{
				sourceStr=getNumberStr(fromCol-2);
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="退";
						destStr=getNumberStr(fromRow-toRow);
					}
					else 
					{
						actStr="进";
						destStr=getNumberStr(toRow-fromRow);
					}
				}
				else
				{
						actStr="平";
						destStr=getNumberStr(toCol-2);
				}
				temp.chessMan=chessManName;
				temp.from=sourceStr;
				temp.act=actStr;
				temp.to=destStr;
			}
			else
			{
				if((firstColSameInBigEnd+firstColSameInSmallEnd==2) && (secondColSameInBigEnd+secondColSameInSmallEnd==0))
				{
					if(firstColSameInBigEnd==0)
						sourceStr="前";
					else if(firstColSameInBigEnd==1)
						sourceStr="中";
					else
						sourceStr="后";
				}
				else if(firstColSameInBigEnd+firstColSameInSmallEnd>=3)
				{
					sourceStr=getNumberStr(firstColSameInBigEnd+1);
				}
				else if((firstColSameInBigEnd+firstColSameInSmallEnd!=0) && 
					(secondColSameInBigEnd+secondColSameInSmallEnd!=0))
				{
					sourceStr=getNumberStr(secondColSameInSmallEnd+firstColSameInBigEnd+1);
				}
				else
				{
					if(firstColSameInBigEnd!=0)
						sourceStr="后";
					else
						sourceStr="前";
				}
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="退";
						destStr=getNumberStr(fromRow-toRow);
					}
					else 
					{
						actStr="进";
						destStr=getNumberStr(toRow-fromRow);
					}
				}
				else
				{
						actStr="平";
						destStr=getNumberStr(toCol-2);
				}
				temp.chessMan=sourceStr;
				temp.from=chessManName;
				temp.act=actStr;
				temp.to=destStr;
			}
		}
	}
	else if(!isRedChessMan && isRedOnBigEnd)
	{
		if(chessManValue == blackAdvisorOne || chessManValue == blackAdvisorTwo ||
					       chessManValue == blackBishopOne  || chessManValue == blackBishopTwo)
		{
			if(_move.from > _move.to)
				actStr="退";
			else
				actStr="进";
			sourceStr=getNumberStr(fromCol-2,0);
			destStr=getNumberStr(toCol-2,0);
			temp.chessMan=chessManName;
			temp.from=sourceStr;
			temp.act=actStr;
			temp.to=destStr;
		}
		else if(chessManValue == blackKing)
		{
			sourceStr=getNumberStr(fromCol-2,0);
			if(fromCol == toCol)
			{
				if(fromRow > toRow)
				{
					actStr="退";
					destStr=getNumberStr(fromRow-toRow,0);
				}
				else 
				{
					actStr="进";
					destStr=getNumberStr(toRow-fromRow,0);
				}
			}
			else
			{
					actStr="平";
					destStr=getNumberStr(toCol-2,0);
			}
			temp.chessMan=chessManName;
			temp.from=sourceStr;
			temp.act=actStr;
			temp.to=destStr;
		}
		else if(chessManValue == blackKnightOne || chessManValue == blackKnightTwo || chessManValue == blackRookOne ||
			chessManValue == blackRookTwo   || chessManValue == blackCannonOne || chessManValue == blackCannonTwo)
		{
			if(firstColSameInBigEnd + firstColSameInSmallEnd ==0)
			{
				sourceStr=getNumberStr(fromCol-2,0);
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="退";
						destStr=getNumberStr(fromRow-toRow,0);
					}
					else 
					{
						actStr="进";
						destStr=getNumberStr(toRow-fromRow,0);
					}
				}
				else if(fromRow == toRow)
				{
						actStr="平";
						destStr=getNumberStr(toCol-2,0);
				}
				else
				{
					if(_move.from < _move.to)
						actStr="进";
					else
						actStr="退";
					destStr=getNumberStr(toCol-2,0);
				}
				temp.chessMan=chessManName;
				temp.from=sourceStr;
				temp.act=actStr;
				temp.to=destStr;
			}
			else
			{
				if(firstColSameInBigEnd!=0)
					sourceStr="后";
				else
					sourceStr="前";
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="退";
						destStr=getNumberStr(fromRow-toRow,0);
					}
					else 
					{
						actStr="进";
						destStr=getNumberStr(toRow-fromRow,0);
					}
				}
				else if(fromRow == toRow)
				{
						actStr="平";
						destStr=getNumberStr(toCol-2,0);
				}
				else
				{
					if(_move.from < _move.to)
						actStr="进";
					else
						actStr="退";
					destStr=getNumberStr(toCol-2,0);
				}
				temp.chessMan=sourceStr;
				temp.from=chessManName;
				temp.act=actStr;
				temp.to=destStr;
			}
		}
		else
		{
			if(firstColSameInBigEnd+firstColSameInSmallEnd==0)
			{
				sourceStr=getNumberStr(fromCol-2,0);
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="退";
						destStr=getNumberStr(fromRow-toRow,0);
					}
					else 
					{
						actStr="进";
						destStr=getNumberStr(toRow-fromRow,0);
					}
				}
				else
				{
						actStr="平";
						destStr=getNumberStr(toCol-2,0);
				}
				temp.chessMan=chessManName;
				temp.from=sourceStr;
				temp.act=actStr;
				temp.to=destStr;
			}
			else
			{
				if((firstColSameInBigEnd+firstColSameInSmallEnd==2) && (secondColSameInBigEnd+secondColSameInSmallEnd==0))
				{
					if(firstColSameInBigEnd==0)
						sourceStr="前";
					else if(firstColSameInBigEnd==1)
						sourceStr="中";
					else
						sourceStr="后";
				}
				else if(firstColSameInBigEnd+firstColSameInSmallEnd>=3)
				{
					sourceStr=getNumberStr(firstColSameInBigEnd+1);
				}
				else if((firstColSameInBigEnd+firstColSameInSmallEnd!=0) && 
					(secondColSameInBigEnd+secondColSameInSmallEnd!=0))
				{
					sourceStr=getNumberStr(secondColSameInBigEnd+firstColSameInSmallEnd+1);
				}
				else
				{
					if(firstColSameInBigEnd!=0)
						sourceStr="后";
					else
						sourceStr="前";
				}
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="退";
						destStr=getNumberStr(fromRow-toRow,0);
					}
					else 
					{
						actStr="进";
						destStr=getNumberStr(toRow-fromRow,0);
					}
				}
				else
				{
						actStr="平";
						destStr=getNumberStr(toCol-2,0);
				}
				temp.chessMan=sourceStr;
				temp.from=chessManName;
				temp.act=actStr;
				temp.to=destStr;
			}
		}
	}
	else
	{
		if(chessManValue == blackAdvisorOne || chessManValue == blackAdvisorTwo ||
					       chessManValue == blackBishopOne  || chessManValue == blackBishopTwo)
		{
			if(_move.from > _move.to)
				actStr="进";
			else
				actStr="退";
			sourceStr=getNumberStr(12-fromCol,0);
			destStr=getNumberStr(12-toCol,0);
			temp.chessMan=chessManName;
			temp.from=sourceStr;
			temp.act=actStr;
			temp.to=destStr;
		}
		else if(chessManValue == blackKing)
		{
			sourceStr=getNumberStr(12-fromCol,0);
			if(fromCol == toCol)
			{
				if(fromRow > toRow)
				{
					actStr="进";
					destStr=getNumberStr(fromRow-toRow,0);
				}
				else 
				{
					actStr="退";
					destStr=getNumberStr(toRow-fromRow,0);
				}
			}
			else
			{
					actStr="平";
					destStr=getNumberStr(12-toCol,0);
			}
			temp.chessMan=chessManName;
			temp.from=sourceStr;
			temp.act=actStr;
			temp.to=destStr;
		}
		else if(chessManValue == blackKnightOne || chessManValue == blackKnightTwo || chessManValue == blackRookOne ||
			chessManValue == blackRookTwo   || chessManValue == blackCannonOne || chessManValue == blackCannonTwo)
		{
			if(firstColSameInBigEnd + firstColSameInSmallEnd ==0)
			{
				sourceStr=getNumberStr(12-fromCol,0);
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="进";
						destStr=getNumberStr(fromRow-toRow,0);
					}
					else 
					{
						actStr="退";
						destStr=getNumberStr(toRow-fromRow,0);
					}
				}
				else if(fromRow == toRow)
				{
						actStr="平";
						destStr=getNumberStr(12-toCol,0);
				}
				else
				{
					if(_move.from > _move.to)
						actStr="进";
					else
						actStr="退";
					destStr=getNumberStr(12-toCol,0);
				}
				temp.chessMan=chessManName;
				temp.from=sourceStr;
				temp.act=actStr;
				temp.to=destStr;
			}
			else
			{
				if(firstColSameInBigEnd!=0)
					sourceStr="前";
				else
					sourceStr="后";
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="进";
						destStr=getNumberStr(fromRow-toRow,0);
					}
					else 
					{
						actStr="退";
						destStr=getNumberStr(toRow-fromRow,0);
					}
				}
				else if(fromRow == toRow)
				{
						actStr="平";
						destStr=getNumberStr(12-toCol,0);
				}
				else
				{
					if(_move.from > _move.to)
						actStr="进";
					else
						actStr="退";
					destStr=getNumberStr(12-toCol,0);
				}
				temp.chessMan=sourceStr;
				temp.from=chessManName;
				temp.act=actStr;
				temp.to=destStr;
			}
		}
		else
		{
			if(firstColSameInBigEnd+firstColSameInSmallEnd==0)
			{
				sourceStr=getNumberStr(12-fromCol,0);
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="进";
						destStr=getNumberStr(fromRow-toRow,0);
					}
					else 
					{
						actStr="退";
						destStr=getNumberStr(toRow-fromRow,0);
					}
				}
				else
				{
						actStr="平";
						destStr=getNumberStr(12-toCol,0);
				}
				temp.chessMan=chessManName;
				temp.from=sourceStr;
				temp.act=actStr;
				temp.to=destStr;
			}
			else
			{
				if((firstColSameInBigEnd+firstColSameInSmallEnd==2) && (secondColSameInBigEnd+secondColSameInSmallEnd==0))
				{
					if(firstColSameInSmallEnd==0)
						sourceStr="前";
					else if(firstColSameInSmallEnd==1)
						sourceStr="中";
					else
						sourceStr="后";
				}
				else if(firstColSameInBigEnd+firstColSameInSmallEnd>=3)
				{
					sourceStr=getNumberStr(firstColSameInSmallEnd+1);
				}
				else if((firstColSameInBigEnd+firstColSameInSmallEnd!=0) && 
					(secondColSameInBigEnd+secondColSameInSmallEnd!=0))
				{
					sourceStr=getNumberStr(secondColSameInSmallEnd+firstColSameInBigEnd+1);
				}
				else
				{
					if(firstColSameInBigEnd!=0)
						sourceStr="前";
					else
						sourceStr="后";
				}
				if(fromCol == toCol)
				{
					if(fromRow > toRow)
					{
						actStr="进";
						destStr=getNumberStr(fromRow-toRow,0);
					}
					else 
					{
						actStr="退";
						destStr=getNumberStr(toRow-fromRow,0);
					}
				}
				else
				{
						actStr="平";
						destStr=getNumberStr(12-toCol,0);
				}
				temp.chessMan=sourceStr;
				temp.from=chessManName;
				temp.act=actStr;
				temp.to=destStr;
			}
		}
	}
	return temp;
}
//这一函数主要实现中文纵线表示到程序中的表示的转换
//首先分析第一个字段chessMan，如果是棋子名字，则是一般的情况，我们更具第二个字段找到对应的行然后找出其中的棋子值，然后对于不同
//	棋子采取不同的移动策略，需要注意的是四类的划分，以及坐标的转换；在每一类中，又分为平、进、退
//然后分析第一个字段不是名字，则对于车马炮只可能是前后，所以又分类非兵类、兵类
//	非兵类：只要找到一个棋子即可
//	兵类：由于其规定，我们需要考虑三兵通线、多兵同线、两列多兵的情况
//该函数并未优化，非类之间的联系如果找到可以简化为现在的1/4
Move MoveMaker::translateToMove(SmartPtr<CChessStruct> _cChess,const ChinessMove& _chinessMove)
{
	if(_cChess.getPtr() == NULL)
		throw UBException(-1,"_cChess is null pointer..");
	bool isRedOnBigEnd=false,isRedChessMan=true;
	int chessManValue,chessManPosition;
	if(_cChess->cChessManPosition[0] > 8*16)
		isRedOnBigEnd=true;
	Move temp;
	int toPosition,captureChessMan;
	if(isChessManChinessName(_chinessMove.chessMan))
	{
		int col=0;
		try
		{
			col=getNumber(_chinessMove.from);
		}catch(UBException& e)
		{
			isRedChessMan=false;
			col=getNumber(_chinessMove.from,0);
		}
		if(isRedOnBigEnd && isRedChessMan)
		{
			col=12-col;
			for(int i=3;i<=12;i++)
			{
				int tempChessManValue=_cChess->cChessArray[i*16+col];
				if(tempChessManValue!=0 && getChessManChinessName(tempChessManValue)==_chinessMove.chessMan)
				{
					chessManValue=tempChessManValue;
					chessManPosition=i*16+col;
					break;
				} 
			}
			toPosition=chessManPosition;
			if(_chinessMove.act == UBString("平"))
			{
				int destCol=12-getNumber(_chinessMove.to);
				toPosition=chessManPosition+destCol-col;
			}
			else if(_chinessMove.act == UBString("进"))
			{
				if(chessManValue==redKing)
				{
					toPosition=chessManPosition-16;
				}
				else if(chessManValue==redAdvisorOne || chessManValue==redAdvisorTwo)
				{
					int destCol=12-getNumber(_chinessMove.to);
					if(destCol>col)
						toPosition-=15;
					else
						toPosition-=17;
				}
				else if(chessManValue==redBishopOne || chessManValue==redBishopTwo)
				{
					int destCol=12-getNumber(_chinessMove.to);
					if(destCol>col)
						toPosition-=30;
					else
						toPosition-=34;
				}
				else if(chessManValue==redKnightOne || chessManValue==redKnightTwo)
				{
					int destCol=12-getNumber(_chinessMove.to);
					if(destCol-col==1)
						toPosition-=31;
					else if(destCol-col==2)
						toPosition-=14;
					else if(destCol-col==-1)
						toPosition-=33;
					else
						toPosition-=18;
				}
				else if(chessManValue==redRookOne || chessManValue==redRookTwo ||
					chessManValue==redCannonOne || chessManValue==redCannonTwo ||
					chessManValue==redPawnOne || chessManValue==redPawnTwo ||
					chessManValue==redPawnThree || chessManValue==redPawnFour ||
					chessManValue==redPawnFive)
				{
					int off=getNumber(_chinessMove.to);
					toPosition-=off*16;
				}
					
			}
			else if(_chinessMove.act == UBString("退"))
			{
				if(chessManValue==redKing)
				{
					toPosition=chessManPosition+16;
				}
				else if(chessManValue==redAdvisorOne || chessManValue==redAdvisorTwo)
				{
					int destCol=12-getNumber(_chinessMove.to);
					if(destCol>col)
						toPosition+=17;
					else
						toPosition+=15;
				}
				else if(chessManValue==redBishopOne || chessManValue==redBishopTwo)
				{
					int destCol=12-getNumber(_chinessMove.to);
					if(destCol>col)
						toPosition+=34;
					else
						toPosition+=30;
				}
				else if(chessManValue==redKnightOne || chessManValue==redKnightTwo)
				{
					int destCol=12-getNumber(_chinessMove.to);
					if(destCol-col==1)
						toPosition+=33;
					else if(destCol-col==2)
						toPosition+=18;
					else if(destCol-col==-1)
						toPosition+=31;
					else
						toPosition+=14;
				}
				else if(chessManValue==redRookOne || chessManValue==redRookTwo ||
					chessManValue==redCannonOne || chessManValue==redCannonTwo ||
					chessManValue==redPawnOne || chessManValue==redPawnTwo ||
					chessManValue==redPawnThree || chessManValue==redPawnFour ||
					chessManValue==redPawnFive)
				{
					int off=getNumber(_chinessMove.to);
					toPosition+=off*16;
				}
			}
			
			captureChessMan=_cChess->cChessArray[toPosition];
			temp.from=chessManPosition;
			temp.to=toPosition;
			temp.capture=captureChessMan;
		}
		else if(isRedChessMan && !isRedOnBigEnd)
		{
			col=col+2;
			for(int i=3;i<=12;i++)
			{
				int tempChessManValue=_cChess->cChessArray[i*16+col];
				if(tempChessManValue!=0 && getChessManChinessName(tempChessManValue)==_chinessMove.chessMan)
				{
					chessManValue=tempChessManValue;
					chessManPosition=i*16+col;
					break;
				} 
			}
			toPosition=chessManPosition;
			if(_chinessMove.act == UBString("平"))
			{
				int destCol=getNumber(_chinessMove.to)+2;
				toPosition=chessManPosition+destCol-col;
			}
			else if(_chinessMove.act == UBString("进"))
			{
				if(chessManValue==redKing)
				{
					toPosition=chessManPosition+16;
				}
				else if(chessManValue==redAdvisorOne || chessManValue==redAdvisorTwo)
				{
					int destCol=getNumber(_chinessMove.to)+2;
					if(destCol>col)
						toPosition+=17;
					else
						toPosition+=15;
				}
				else if(chessManValue==redBishopOne || chessManValue==redBishopTwo)
				{
					int destCol=getNumber(_chinessMove.to)+2;
					if(destCol>col)
						toPosition+=34;
					else
						toPosition+=30;
				}
				else if(chessManValue==redKnightOne || chessManValue==redKnightTwo)
				{
					int destCol=getNumber(_chinessMove.to)+2;
					if(destCol-col==1)
						toPosition+=33;
					else if(destCol-col==2)
						toPosition+=18;
					else if(destCol-col==-1)
						toPosition+=31;
					else
						toPosition+=14;
				}
				else if(chessManValue==redRookOne || chessManValue==redRookTwo ||
					chessManValue==redCannonOne || chessManValue==redCannonTwo ||
					chessManValue==redPawnOne || chessManValue==redPawnTwo ||
					chessManValue==redPawnThree || chessManValue==redPawnFour ||
					chessManValue==redPawnFive)
				{
					int off=getNumber(_chinessMove.to);
					toPosition+=off*16;
				}
					
			}
			else if(_chinessMove.act == UBString("退"))
			{
				if(chessManValue==redKing)
				{
					toPosition=chessManPosition-16;
				}
				else if(chessManValue==redAdvisorOne || chessManValue==redAdvisorTwo)
				{
					int destCol=getNumber(_chinessMove.to)+2;
					if(destCol>col)
						toPosition-=15;
					else
						toPosition-=17;
				}
				else if(chessManValue==redBishopOne || chessManValue==redBishopTwo)
				{
					int destCol=getNumber(_chinessMove.to)+2;
					if(destCol>col)
						toPosition-=30;
					else
						toPosition-=34;
				}
				else if(chessManValue==redKnightOne || chessManValue==redKnightTwo)
				{
					int destCol=getNumber(_chinessMove.to)+2;
					if(destCol-col==1)
						toPosition-=31;
					else if(destCol-col==2)
						toPosition-=14;
					else if(destCol-col==-1)
						toPosition-=33;
					else
						toPosition-=18;
				}
				else if(chessManValue==redRookOne || chessManValue==redRookTwo ||
					chessManValue==redCannonOne || chessManValue==redCannonTwo ||
					chessManValue==redPawnOne || chessManValue==redPawnTwo ||
					chessManValue==redPawnThree || chessManValue==redPawnFour ||
					chessManValue==redPawnFive)
				{
					int off=getNumber(_chinessMove.to);
					toPosition-=off*16;
				}
			}
			
			captureChessMan=_cChess->cChessArray[toPosition];
			temp.from=chessManPosition;
			temp.to=toPosition;
			temp.capture=captureChessMan;
		}
		else if(!isRedChessMan && isRedOnBigEnd)
		{
			col=col+2;
			for(int i=3;i<=12;i++)
			{
				int tempChessManValue=_cChess->cChessArray[i*16+col];
				if(tempChessManValue!=0 && getChessManChinessName(tempChessManValue)==_chinessMove.chessMan)
				{
					chessManValue=tempChessManValue;
					chessManPosition=i*16+col;
					break;
				} 
			}
			toPosition=chessManPosition;
			if(_chinessMove.act == UBString("平"))
			{
				int destCol=getNumber(_chinessMove.to)+2;
				toPosition=chessManPosition+destCol-col;
			}
			else if(_chinessMove.act == UBString("进"))
			{
				if(chessManValue==blackKing)
				{
					toPosition=chessManPosition+16;
				}
				else if(chessManValue==blackAdvisorOne || chessManValue==blackAdvisorTwo)
				{
					int destCol=getNumber(_chinessMove.to)+2;
					if(destCol>col)
						toPosition+=17;
					else
						toPosition+=15;
				}
				else if(chessManValue==blackBishopOne || chessManValue==blackBishopTwo)
				{
					int destCol=getNumber(_chinessMove.to)+2;
					if(destCol>col)
						toPosition+=34;
					else
						toPosition+=30;
				}
				else if(chessManValue==blackKnightOne || chessManValue==blackKnightTwo)
				{
					int destCol=getNumber(_chinessMove.to)+2;
					if(destCol-col==1)
						toPosition+=33;
					else if(destCol-col==2)
						toPosition+=18;
					else if(destCol-col==-1)
						toPosition+=31;
					else
						toPosition+=14;
				}
				else if(chessManValue==blackRookOne || chessManValue==blackRookTwo ||
					chessManValue==blackCannonOne || chessManValue==blackCannonTwo ||
					chessManValue==blackPawnOne || chessManValue==blackPawnTwo ||
					chessManValue==blackPawnThree || chessManValue==blackPawnFour ||
					chessManValue==blackPawnFive)
				{
					int off=getNumber(_chinessMove.to);
					toPosition+=off*16;
				}
					
			}
			else if(_chinessMove.act == UBString("退"))
			{
				if(chessManValue==blackKing)
				{
					toPosition=chessManPosition-16;
				}
				else if(chessManValue==blackAdvisorOne || chessManValue==blackAdvisorTwo)
				{
					int destCol=getNumber(_chinessMove.to)+2;
					if(destCol>col)
						toPosition-=15;
					else
						toPosition-=17;
				}
				else if(chessManValue==blackBishopOne || chessManValue==blackBishopTwo)
				{
					int destCol=getNumber(_chinessMove.to)+2;
					if(destCol>col)
						toPosition-=30;
					else
						toPosition-=34;
				}
				else if(chessManValue==blackKnightOne || chessManValue==blackKnightTwo)
				{
					int destCol=getNumber(_chinessMove.to)+2;
					if(destCol-col==1)
						toPosition-=31;
					else if(destCol-col==2)
						toPosition-=14;
					else if(destCol-col==-1)
						toPosition-=33;
					else
						toPosition-=18;
				}
				else if(chessManValue==blackRookOne || chessManValue==blackRookTwo ||
					chessManValue==blackCannonOne || chessManValue==blackCannonTwo ||
					chessManValue==blackPawnOne || chessManValue==blackPawnTwo ||
					chessManValue==blackPawnThree || chessManValue==blackPawnFour ||
					chessManValue==blackPawnFive)
				{
					int off=getNumber(_chinessMove.to);
					toPosition-=off*16;
				}
			}
			
			captureChessMan=_cChess->cChessArray[toPosition];
			temp.from=chessManPosition;
			temp.to=toPosition;
			temp.capture=captureChessMan;
		}
		//!isRedChessMan && !isRedOnBigEnd
		else
		{
			col=12-col;
			for(int i=3;i<=12;i++)
			{
				int tempChessManValue=_cChess->cChessArray[i*16+col];
				if(tempChessManValue!=0 && getChessManChinessName(tempChessManValue)==_chinessMove.chessMan)
				{
					chessManValue=tempChessManValue;
					chessManPosition=i*16+col;
					break;
				} 
			}
			toPosition=chessManPosition;
			if(_chinessMove.act == UBString("平"))
			{
				int destCol=12-getNumber(_chinessMove.to);
				toPosition=chessManPosition+destCol-col;
			}
			else if(_chinessMove.act == UBString("进"))
			{
				if(chessManValue==blackKing)
				{
					toPosition=chessManPosition-16;
				}
				else if(chessManValue==blackAdvisorOne || chessManValue==blackAdvisorTwo)
				{
					int destCol=12-getNumber(_chinessMove.to);
					if(destCol>col)
						toPosition-=15;
					else
						toPosition-=17;
				}
				else if(chessManValue==blackBishopOne || chessManValue==blackBishopTwo)
				{
					int destCol=12-getNumber(_chinessMove.to);
					if(destCol>col)
						toPosition-=30;
					else
						toPosition-=34;
				}
				else if(chessManValue==blackKnightOne || chessManValue==blackKnightTwo)
				{
					int destCol=12-getNumber(_chinessMove.to);
					if(destCol-col==1)
						toPosition-=31;
					else if(destCol-col==2)
						toPosition-=14;
					else if(destCol-col==-1)
						toPosition-=33;
					else
						toPosition-=18;
				}
				else if(chessManValue==blackRookOne || chessManValue==blackRookTwo ||
					chessManValue==blackCannonOne || chessManValue==blackCannonTwo ||
					chessManValue==blackPawnOne || chessManValue==blackPawnTwo ||
					chessManValue==blackPawnThree || chessManValue==blackPawnFour ||
					chessManValue==blackPawnFive)
				{
					int off=getNumber(_chinessMove.to);
					toPosition-=off*16;
				}
					
			}
			else if(_chinessMove.act == UBString("退"))
			{
				if(chessManValue==blackKing)
				{
					toPosition=chessManPosition+16;
				}
				else if(chessManValue==blackAdvisorOne || chessManValue==blackAdvisorTwo)
				{
					int destCol=12-getNumber(_chinessMove.to);
					if(destCol>col)
						toPosition+=17;
					else
						toPosition+=15;
				}
				else if(chessManValue==blackBishopOne || chessManValue==blackBishopTwo)
				{
					int destCol=12-getNumber(_chinessMove.to);
					if(destCol>col)
						toPosition+=34;
					else
						toPosition+=30;
				}
				else if(chessManValue==blackKnightOne || chessManValue==blackKnightTwo)
				{
					int destCol=12-getNumber(_chinessMove.to);
					if(destCol-col==1)
						toPosition+=33;
					else if(destCol-col==2)
						toPosition+=18;
					else if(destCol-col==-1)
						toPosition+=31;
					else
						toPosition+=14;
				}
				else if(chessManValue==blackRookOne || chessManValue==blackRookTwo ||
					chessManValue==blackCannonOne || chessManValue==blackCannonTwo ||
					chessManValue==blackPawnOne || chessManValue==blackPawnTwo ||
					chessManValue==blackPawnThree || chessManValue==blackPawnFour ||
					chessManValue==blackPawnFive)
				{
					int off=getNumber(_chinessMove.to);
					toPosition+=off*16;
				}
			}
			
			captureChessMan=_cChess->cChessArray[toPosition];
			temp.from=chessManPosition;
			temp.to=toPosition;
			temp.capture=captureChessMan;
		}
	}
	else
	{
		//该分支只可能是卒、马、炮、车
		int destOrOff;
		int fromCol;
		try
		{
			destOrOff=getNumber(_chinessMove.to);
		}catch(UBException& e)
		{
			isRedChessMan=false;
			destOrOff=getNumber(_chinessMove.to,0);
		}
		if(isRedChessMan && isRedOnBigEnd)
		{
			if(_chinessMove.from == UBString("兵"))
			{
                int tempChessManValue;
				int checkNum=0;
				int firstSameCol=0,firstSameNumber=0,secondSameCol=0,secondSameNumber=0;
				int firstChessManGroup[5];
				int secondChessManGroup[3];
				for(int j=3;j<=11;j++)
				{
					checkNum=0;
					for(int i=3;i<=12;i++)
					{
						tempChessManValue=_cChess->cChessArray[i*16+j];
						if(tempChessManValue!=0 && 
						   getChessManChinessName(tempChessManValue)==_chinessMove.from)
						{
							++checkNum;
							if(checkNum==1 && firstSameCol==0)
								firstChessManGroup[0]=i*16+j;
							if(checkNum==1 && firstSameCol!=0)
								secondChessManGroup[0]=i*16+j;
							if(checkNum >= 2)
							{
								if(firstSameCol==0 || firstSameCol==j)
								{
									firstSameCol=j;
									firstSameNumber=checkNum;
									firstChessManGroup[firstSameNumber-1]=i*16+j;
								}
								else
								{
									secondSameCol=j;
									secondSameNumber=checkNum;
									secondChessManGroup[secondSameNumber-1]=i*16+j;
								}
							}
						}
					}
				}
				if(_chinessMove.chessMan == UBString("前"))
				{
					chessManPosition=firstChessManGroup[0];
					fromCol=chessManPosition%16;
				}
				else if(_chinessMove.chessMan == UBString("中"))
				{
					chessManPosition=firstChessManGroup[1];
					fromCol=chessManPosition%16;
				}
				else if(_chinessMove.chessMan == UBString("后"))
				{
					chessManPosition=firstChessManGroup[firstSameNumber-1];
					fromCol=chessManPosition%16;
				}
				else
				{
					if(secondSameNumber==0)
					{
						chessManPosition=firstChessManGroup[getNumber(_chinessMove.chessMan)-1];
					}
					else
					{
						if(getNumber(_chinessMove.chessMan) <= secondSameNumber)
						   chessManPosition=secondChessManGroup[getNumber(_chinessMove.chessMan)-1];
						else
						   chessManPosition=firstChessManGroup[
									getNumber(_chinessMove.chessMan)-secondSameNumber-1];
					}
					fromCol=chessManPosition%16;
				}
				//已经找到兵的位置
				toPosition=chessManPosition;
				if(_chinessMove.act == UBString("平"))
				{
					destOrOff=12-destOrOff;
					toPosition=chessManPosition+destOrOff-fromCol;
				}
				else if(_chinessMove.act == UBString("进"))
				{
					toPosition-=destOrOff*16;
				}
				else
				{
					toPosition+=destOrOff*16;
				}
			}
			else
			{
				int tempChessManValue,col;
				bool check=false;
				for(int j=3;j<=11;j++)
				{
					for(int i=3;i<=12;i++)
					{
						tempChessManValue=_cChess->cChessArray[i*16+j];
						if(tempChessManValue!=0 && 
						   getChessManChinessName(tempChessManValue)==_chinessMove.from)
						{
							if((_chinessMove.chessMan == UBString("前") && !check) ||
							   (_chinessMove.chessMan ==UBString("后") && check))
							{
								chessManValue=tempChessManValue;
								chessManPosition=i*16+j;
								break;
							}
							check=true;
						}
					}
				}
				fromCol=chessManPosition%16;
				toPosition=chessManPosition;
				if(_chinessMove.act == UBString("平"))
				{
					destOrOff=12-destOrOff;
					toPosition=chessManPosition+destOrOff-fromCol;
				}
				else if(_chinessMove.act == UBString("进"))
				{
					if(chessManValue == redKnightOne || chessManValue == redKnightTwo)
					{
						destOrOff=12-destOrOff;
						if(destOrOff-fromCol==1)
							toPosition-=31;
						else if(destOrOff-fromCol==2)
							toPosition-=14;
						else if(destOrOff-fromCol==-1)
							toPosition-=33;
						else
							toPosition-=18;
					}
					else if(chessManValue == redRookOne || chessManValue == redRookTwo ||
						chessManValue == redCannonOne || chessManValue == redCannonTwo)
					{
						toPosition-=destOrOff*16;
					}
				}
				else if(_chinessMove.act == UBString("退"))
				{
					if(chessManValue == redKnightOne || chessManValue == redKnightTwo)
					{
						destOrOff=12-destOrOff;
						if(destOrOff-fromCol==1)
							toPosition+=33;
						else if(destOrOff-fromCol==2)
							toPosition+=18;
						else if(destOrOff-fromCol==-1)
							toPosition+=31;
						else
							toPosition+=14;
					}
					else if(chessManValue == redRookOne || chessManValue == redRookTwo ||
						chessManValue == redCannonOne || chessManValue == redCannonTwo)
					{
						toPosition+=destOrOff*16;
					}
				}
				
			}
			captureChessMan=_cChess->cChessArray[toPosition];
			temp.from=chessManPosition;
			temp.to=toPosition;
			temp.capture=captureChessMan;
		}
		else if(isRedChessMan && !isRedOnBigEnd)
		{
			if(_chinessMove.from == UBString("兵"))
			{
				int tempChessManValue,col;
				int checkNum=0;
				int firstSameCol=0,firstSameNumber=0,secondSameCol=0,secondSameNumber=0;
				int firstChessManGroup[5];
				int secondChessManGroup[3];
				for(int j=3;j<=11;j++)
				{
					checkNum=0;
					for(int i=3;i<=12;i++)
					{
						tempChessManValue=_cChess->cChessArray[i*16+j];
						if(tempChessManValue!=0 && 
						   getChessManChinessName(tempChessManValue)==_chinessMove.from)
						{
							++checkNum;
							if(checkNum==1 && firstSameCol==0)
								firstChessManGroup[0]=i*16+j;
							if(checkNum==1 && firstSameCol!=0)
								secondChessManGroup[0]=i*16+j;
							if(checkNum >= 2)
							{
								if(firstSameCol==0 || firstSameCol==j)
								{
									firstSameCol=j;
									firstSameNumber=checkNum;
									firstChessManGroup[firstSameNumber-1]=i*16+j;
								}
								else
								{
									secondSameCol=j;
									secondSameNumber=checkNum;
									secondChessManGroup[secondSameNumber-1]=i*16+j;
								}
							}
						}
					}
				}
				if(_chinessMove.chessMan == UBString("后"))
				{
					chessManPosition=firstChessManGroup[0];
					fromCol=chessManPosition%16;
				}
				else if(_chinessMove.chessMan == UBString("中"))
				{
					chessManPosition=firstChessManGroup[1];
					fromCol=chessManPosition%16;
				}
				else if(_chinessMove.chessMan == UBString("前"))
				{
					chessManPosition=firstChessManGroup[firstSameNumber-1];
					fromCol=chessManPosition%16;
				}
				else
				{
					if(secondSameNumber==0)
					{
						chessManPosition=firstChessManGroup[
									firstSameNumber-getNumber(_chinessMove.chessMan)];
					}
					else
					{
						if(getNumber(_chinessMove.chessMan) <= firstSameNumber)
						   chessManPosition=secondChessManGroup[
									firstSameNumber-getNumber(_chinessMove.chessMan)];
						else
						   chessManPosition=firstChessManGroup[
							secondSameNumber-(getNumber(_chinessMove.chessMan)-firstSameNumber)];
					}
					fromCol=chessManPosition%16;
				}
				//已经找到兵的位置
				toPosition=chessManPosition;
				if(_chinessMove.act == UBString("平"))
				{
					destOrOff=destOrOff+2;
					toPosition=chessManPosition+destOrOff-fromCol;
				}
				else if(_chinessMove.act == UBString("进"))
				{
					toPosition+=destOrOff*16;
				}
				else
				{
					toPosition-=destOrOff*16;
				}
			}
			else
			{
				int tempChessManValue,col;
				bool check=false;
				for(int j=3;j<=11;j++)
				{
					for(int i=3;i<=12;i++)
					{
						tempChessManValue=_cChess->cChessArray[i*16+j];
						if(tempChessManValue!=0 && 
						   getChessManChinessName(tempChessManValue)==_chinessMove.from)
						{
							if((_chinessMove.chessMan == UBString("后") && !check) ||
							   (_chinessMove.chessMan ==UBString("前") && check))
							{
								chessManValue=tempChessManValue;
								chessManPosition=i*16+j;
								break;
							}
							check=true;
						}
					}
				}
				fromCol=chessManPosition%16;
				toPosition=chessManPosition;
				if(_chinessMove.act == UBString("平"))
				{
					destOrOff=destOrOff+2;
					toPosition=chessManPosition+destOrOff-fromCol;
				}
				else if(_chinessMove.act == UBString("进"))
				{
					if(chessManValue == redKnightOne || chessManValue == redKnightTwo)
					{
						destOrOff=destOrOff+2;
						if(destOrOff-fromCol==1)
							toPosition+=33;
						else if(destOrOff-fromCol==2)
							toPosition+=18;
						else if(destOrOff-fromCol==-1)
							toPosition+=31;
						else
							toPosition+=14;
					}
					else if(chessManValue == redRookOne || chessManValue == redRookTwo ||
						chessManValue == redCannonOne || chessManValue == redCannonTwo)
					{
						toPosition+=destOrOff*16;
					}
				}
				else if(_chinessMove.act == UBString("退"))
				{
					if(chessManValue == redKnightOne || chessManValue == redKnightTwo)
					{
						destOrOff=destOrOff+2;
						if(destOrOff-fromCol==1)
							toPosition-=31;
						else if(destOrOff-fromCol==2)
							toPosition-=14;
						else if(destOrOff-fromCol==-1)
							toPosition-=33;
						else
							toPosition-=18;
					}
					else if(chessManValue == redRookOne || chessManValue == redRookTwo ||
						chessManValue == redCannonOne || chessManValue == redCannonTwo)
					{
						toPosition-=destOrOff*16;
					}
				}
				
			}
			captureChessMan=_cChess->cChessArray[toPosition];
			temp.from=chessManPosition;
			temp.to=toPosition;
			temp.capture=captureChessMan;
		}
		else if(!isRedChessMan && isRedOnBigEnd)
		{
			if(_chinessMove.from == UBString("卒"))
			{
				int tempChessManValue,col;
				int checkNum=0;
				int firstSameCol=0,firstSameNumber=0,secondSameCol=0,secondSameNumber=0;
				int firstChessManGroup[5];
				int secondChessManGroup[3];
				for(int j=3;j<=11;j++)
				{
					checkNum=0;
					for(int i=3;i<=12;i++)
					{
						tempChessManValue=_cChess->cChessArray[i*16+j];
						if(tempChessManValue!=0 && 
						   getChessManChinessName(tempChessManValue)==_chinessMove.from)
						{
							++checkNum;
							if(checkNum==1 && firstSameCol==0)
								firstChessManGroup[0]=i*16+j;
							if(checkNum==1 && firstSameCol!=0)
								secondChessManGroup[0]=i*16+j;
							if(checkNum >= 2)
							{
								if(firstSameCol==0 || firstSameCol==j)
								{
									firstSameCol=j;
									firstSameNumber=checkNum;
									firstChessManGroup[firstSameNumber-1]=i*16+j;
								}
								else
								{
									secondSameCol=j;
									secondSameNumber=checkNum;
									secondChessManGroup[secondSameNumber-1]=i*16+j;
								}
							}
						}
					}
				}
				if(_chinessMove.chessMan == UBString("后"))
				{
					chessManPosition=firstChessManGroup[0];
					fromCol=chessManPosition%16;
				}
				else if(_chinessMove.chessMan == UBString("中"))
				{
					chessManPosition=firstChessManGroup[1];
					fromCol=chessManPosition%16;
				}
				else if(_chinessMove.chessMan == UBString("前"))
				{
					chessManPosition=firstChessManGroup[firstSameNumber-1];
					fromCol=chessManPosition%16;
				}
				else
				{
					if(secondSameNumber==0)
					{
						chessManPosition=firstChessManGroup[
									firstSameNumber-getNumber(_chinessMove.chessMan)];
					}
					else
					{
						if(getNumber(_chinessMove.chessMan) <= firstSameNumber)
						   chessManPosition=secondChessManGroup[
									firstSameNumber-getNumber(_chinessMove.chessMan)];
						else
						   chessManPosition=firstChessManGroup[
							secondSameNumber-(getNumber(_chinessMove.chessMan)-firstSameNumber)];
					}
					fromCol=chessManPosition%16;
				}
				//已经找到兵的位置
				toPosition=chessManPosition;
				if(_chinessMove.act == UBString("平"))
				{
					destOrOff=destOrOff+2;
					toPosition=chessManPosition+destOrOff-fromCol;
				}
				else if(_chinessMove.act == UBString("进"))
				{
					toPosition+=destOrOff*16;
				}
				else
				{
					toPosition-=destOrOff*16;
				}
			}
			else
			{
				int tempChessManValue,col;
				bool check=false;
				for(int j=3;j<=11;j++)
				{
					for(int i=3;i<=12;i++)
					{
						tempChessManValue=_cChess->cChessArray[i*16+j];
						if(tempChessManValue!=0 && 
						   getChessManChinessName(tempChessManValue)==_chinessMove.from)
						{
							if((_chinessMove.chessMan == UBString("后") && !check) ||
							   (_chinessMove.chessMan ==UBString("前") && check))
							{
								chessManValue=tempChessManValue;
								chessManPosition=i*16+j;
								break;
							}
							check=true;
						}
					}
				}
				fromCol=chessManPosition%16;
				toPosition=chessManPosition;
				if(_chinessMove.act == UBString("平"))
				{
					destOrOff=destOrOff+2;
					toPosition=chessManPosition+destOrOff-fromCol;
				}
				else if(_chinessMove.act == UBString("进"))
				{
					if(chessManValue == blackKnightOne || chessManValue == blackKnightTwo)
					{
						destOrOff=destOrOff+2;
						if(destOrOff-fromCol==1)
							toPosition+=33;
						else if(destOrOff-fromCol==2)
							toPosition+=18;
						else if(destOrOff-fromCol==-1)
							toPosition+=31;
						else
							toPosition+=14;
					}
					else if(chessManValue == blackRookOne || chessManValue == blackRookTwo ||
						chessManValue == blackCannonOne || chessManValue == blackCannonTwo)
					{
						toPosition+=destOrOff*16;
					}
				}
				else if(_chinessMove.act == UBString("退"))
				{
					if(chessManValue == blackKnightOne || chessManValue == blackKnightTwo)
					{
						destOrOff=destOrOff+2;
						if(destOrOff-fromCol==1)
							toPosition-=31;
						else if(destOrOff-fromCol==2)
							toPosition-=14;
						else if(destOrOff-fromCol==-1)
							toPosition-=33;
						else
							toPosition-=18;
					}
					else if(chessManValue == blackRookOne || chessManValue == blackRookTwo ||
						chessManValue == blackCannonOne || chessManValue == blackCannonTwo)
					{
						toPosition-=destOrOff*16;
					}
				}
				
			}
			captureChessMan=_cChess->cChessArray[toPosition];
			temp.from=chessManPosition;
			temp.to=toPosition;
			temp.capture=captureChessMan;
		}
		//!isRedChessMan && !isRedOnBigEnd
		else
		{
			if(_chinessMove.from == UBString("卒"))
			{
				int tempChessManValue,col;
				int checkNum=0;
				int firstSameCol=0,firstSameNumber=0,secondSameCol=0,secondSameNumber=0;
				int firstChessManGroup[5];
				int secondChessManGroup[3];
				for(int j=3;j<=11;j++)
				{
					checkNum=0;
					for(int i=3;i<=12;i++)
					{
						tempChessManValue=_cChess->cChessArray[i*16+j];
						if(tempChessManValue!=0 && 
						   getChessManChinessName(tempChessManValue)==_chinessMove.from)
						{
							++checkNum;
							if(checkNum==1 && firstSameCol==0)
								firstChessManGroup[0]=i*16+j;
							if(checkNum==1 && firstSameCol!=0)
								secondChessManGroup[0]=i*16+j;
							if(checkNum >= 2)
							{
								if(firstSameCol==0 || firstSameCol==j)
								{
									firstSameCol=j;
									firstSameNumber=checkNum;
									firstChessManGroup[firstSameNumber-1]=i*16+j;
								}
								else
								{
									secondSameCol=j;
									secondSameNumber=checkNum;
									secondChessManGroup[secondSameNumber-1]=i*16+j;
								}
							}
						}
					}
				}
				if(_chinessMove.chessMan == UBString("前"))
				{
					chessManPosition=firstChessManGroup[0];
					fromCol=chessManPosition%16;
				}
				else if(_chinessMove.chessMan == UBString("中"))
				{
					chessManPosition=firstChessManGroup[1];
					fromCol=chessManPosition%16;
				}
				else if(_chinessMove.chessMan == UBString("后"))
				{
					chessManPosition=firstChessManGroup[firstSameNumber-1];
					fromCol=chessManPosition%16;
				}
				else
				{
					if(secondSameNumber==0)
					{
						chessManPosition=firstChessManGroup[getNumber(_chinessMove.chessMan)-1];
					}
					else
					{
						if(getNumber(_chinessMove.chessMan) <= secondSameNumber)
						   chessManPosition=secondChessManGroup[getNumber(_chinessMove.chessMan)-1];
						else
						   chessManPosition=firstChessManGroup[
									getNumber(_chinessMove.chessMan)-secondSameNumber-1];
					}
					fromCol=chessManPosition%16;
				}
				//已经找到兵的位置
				toPosition=chessManPosition;
				if(_chinessMove.act == UBString("平"))
				{
					destOrOff=12-destOrOff;
					toPosition=chessManPosition+destOrOff-fromCol;
				}
				else if(_chinessMove.act == UBString("进"))
				{
					toPosition-=destOrOff*16;
				}
				else
				{
					toPosition+=destOrOff*16;
				}
			}
			else
			{
				int tempChessManValue,col;
				bool check=false;
				for(int j=3;j<=11;j++)
				{
					for(int i=3;i<=12;i++)
					{
						tempChessManValue=_cChess->cChessArray[i*16+j];
						if(tempChessManValue!=0 && 
						   getChessManChinessName(tempChessManValue)==_chinessMove.from)
						{
							if((_chinessMove.chessMan == UBString("前") && !check) ||
							   (_chinessMove.chessMan ==UBString("后") && check))
							{
								chessManValue=tempChessManValue;
								chessManPosition=i*16+j;
								break;
							}
							check=true;
						}
					}
				}
				fromCol=chessManPosition%16;
				toPosition=chessManPosition;
				if(_chinessMove.act == UBString("平"))
				{
					destOrOff=12-destOrOff;
					toPosition=chessManPosition+destOrOff-fromCol;
				}
				else if(_chinessMove.act == UBString("进"))
				{
					if(chessManValue == blackKnightOne || chessManValue == blackKnightTwo)
					{
						destOrOff=12-destOrOff;
						if(destOrOff-fromCol==1)
							toPosition-=31;
						else if(destOrOff-fromCol==2)
							toPosition-=14;
						else if(destOrOff-fromCol==-1)
							toPosition-=33;
						else
							toPosition-=18;
					}
					else if(chessManValue == blackRookOne || chessManValue == blackRookTwo ||
						chessManValue == blackCannonOne || chessManValue == blackCannonTwo)
					{
						toPosition-=destOrOff*16;
					}
				}
				else if(_chinessMove.act == UBString("退"))
				{
					if(chessManValue == blackKnightOne || chessManValue == blackKnightTwo)
					{
						destOrOff=12-destOrOff;
						if(destOrOff-fromCol==1)
							toPosition+=33;
						else if(destOrOff-fromCol==2)
							toPosition+=18;
						else if(destOrOff-fromCol==-1)
							toPosition+=31;
						else
							toPosition+=14;
					}
					else if(chessManValue == blackRookOne || chessManValue == blackRookTwo ||
						chessManValue == blackCannonOne || chessManValue == blackCannonTwo)
					{
						toPosition+=destOrOff*16;
					}
				}
				
			}
			captureChessMan=_cChess->cChessArray[toPosition];
			temp.from=chessManPosition;
			temp.to=toPosition;
			temp.capture=captureChessMan;
		}
	}
	return temp;
}
//基本转换方法与中文纵线表示一致，所以可以直接借鉴中文表示法
//但是在多兵情形中，处理方法不一致
//此处我们以_move中的from为准，计算出前面的、后面的、两边的兵
WXFMove MoveMaker::translateToWXF(SmartPtr<CChessStruct> _cChess,const Move& _move)
{
	if(_cChess.getPtr() == NULL)
		throw UBException(-1,"CChessStruct object is null");
	if(_cChess->cChessArray[_move.from]==0)
		throw UBException(-1,"the cChess don't match the iccsMove");
	WXFMove wxfMove;
	ChinessMove _chinessMove=translateToChinessZX(_cChess,_move);
	if(isChessManChinessName(_chinessMove.chessMan))
	{
		wxfMove.chessMan=getChessManWXFName(_chinessMove.chessMan);
		try
		{
			wxfMove.source=(char)(getNumber(_chinessMove.from)+'0');
			wxfMove.dest=(char)(getNumber(_chinessMove.to)+'0');
		}catch(UBException& e)
		{
			wxfMove.source=(char)(getNumber(_chinessMove.from,0)+'0');
			wxfMove.dest=(char)(getNumber(_chinessMove.to,0)+'0');
		}
		if(_chinessMove.act == UBString("平"))
			wxfMove.signal='.';
		else if(_chinessMove.act == UBString("进"))
			wxfMove.signal='+';
		else
			wxfMove.signal='-';
	}
	else
	{
		if(_chinessMove.from != UBString("兵") && _chinessMove.from != UBString("卒"))
		{
			wxfMove.chessMan=getChessManWXFName(_chinessMove.from);
			if(_chinessMove.chessMan == UBString("前"))
				wxfMove.source='+';
			else
				wxfMove.source='-';
			if(_chinessMove.act == UBString("平"))
				wxfMove.signal='.';
			else if(_chinessMove.act == UBString("进"))
				wxfMove.signal='+';
			else
				wxfMove.signal='-';
			try
			{
				wxfMove.dest=(char)(getNumber(_chinessMove.to)+'0');
			}catch(UBException& e)
			{
				wxfMove.dest=(char)(getNumber(_chinessMove.to,0)+'0');
			}
		}
		else 
		{
			//进入该分支一定是卒，且该行不止一个卒
			int fromRow=_move.from/16;
			int fromCol=_move.from%16;
			int toRow=_move.to/16;
			int toCol=_move.to%16;
			int chessManValue=_cChess->cChessArray[_move.from];
			bool isRedOnBigEnd=false;
			bool isRedChessMan=false;
			int  firstColSameInSmallEnd=0;
			int  firstColSameInBigEnd=0;
			int  firstCol=fromCol;
			int secondColSameInBigEnd=0;
			int secondColSameInSmallEnd=0;
			if(_cChess->cChessManPosition[0] > 8*16)
				isRedOnBigEnd=true;
			if(chessManValue < 32)
				isRedChessMan=true;
			if(isRedChessMan)
			{
				for(int i=11;i<=15;i++)	
				{
					if((chessManValue-16) != i && _cChess->cChessManPosition[i]!=0 &&
								      _cChess->cChessManPosition[i]%16==fromCol)
					{
						if(_cChess->cChessManPosition[i] <_move.from)
							++firstColSameInSmallEnd;
						else
							++firstColSameInBigEnd;
					}
				}
				if(firstColSameInSmallEnd+firstColSameInBigEnd!=0)
				{
					for(int i=11;i<=15;i++)
					{
						int onePawnPosition=_cChess->cChessManPosition[i];
						bool isBigThanFrom=false;
						if(onePawnPosition%16 != fromCol)
						{
							if(onePawnPosition%16 > fromCol)
								isBigThanFrom=true;
							for(int j=i+1;j<=15;j++)
							{
								int anotherPawnPosition=_cChess->cChessManPosition[j];
								if(anotherPawnPosition%16 == onePawnPosition%16)
								{
									if(isBigThanFrom)
										++secondColSameInBigEnd;
									else
										++secondColSameInSmallEnd;
								}
							}
							if(secondColSameInBigEnd!=0)
							{
								++secondColSameInBigEnd;
								break;
							}
							if(secondColSameInSmallEnd!=0)
							{
								++secondColSameInSmallEnd;
								break;
							}
						}
					}
				}
			}
			else
			{
				for(int i=27;i<=31;i++)	
				{
					if((chessManValue-16) != i && _cChess->cChessManPosition[i]!=0 &&
								      _cChess->cChessManPosition[i]%16==fromCol)
					{
						if(_cChess->cChessManPosition[i] <_move.from)
							++firstColSameInSmallEnd;
						else
							++firstColSameInBigEnd;
					}
				}
				if(firstColSameInSmallEnd+firstColSameInBigEnd!=0)
				{
					for(int i=27;i<=31;i++)
					{
						int onePawnPosition=_cChess->cChessManPosition[i];
						bool isBigThanFrom=false;
						if(onePawnPosition%16 != fromCol)
						{
							if(onePawnPosition%16 > fromCol)
								isBigThanFrom=true;
							for(int j=i+1;j<=31;j++)
							{
								int anotherPawnPosition=_cChess->cChessManPosition[j];
								if(anotherPawnPosition%16 == onePawnPosition%16)
								{
									if(isBigThanFrom)
										++secondColSameInBigEnd;
									else
										++secondColSameInSmallEnd;
								}
							}
							if(secondColSameInBigEnd!=0)
							{
								++secondColSameInBigEnd;
								break;
							}
							if(secondColSameInSmallEnd!=0)
							{
								++secondColSameInSmallEnd;
								break;
							}
						}
					}
				}
			}
			if((isRedChessMan && isRedOnBigEnd) || (!isRedChessMan && !isRedOnBigEnd))
			{
				if(firstColSameInBigEnd+firstColSameInSmallEnd==1)
				{
					if(secondColSameInBigEnd+secondColSameInSmallEnd==0)
						wxfMove.chessMan='P';
					else
						wxfMove.chessMan=(char)(12-firstCol+'0');
					if(firstColSameInBigEnd==1)
						wxfMove.source='+';
					else
						wxfMove.source='-';
				}
				else if(firstColSameInBigEnd+firstColSameInSmallEnd==2)
				{
					if(secondColSameInBigEnd+secondColSameInSmallEnd==0)
						wxfMove.chessMan='P';
					else
						wxfMove.chessMan=(char)(12-firstCol+'0');
					if(firstColSameInSmallEnd==0)
						wxfMove.source='+';
					else if(firstColSameInSmallEnd==1)
						wxfMove.source=(char)(12-firstCol+'0');
					else if(firstColSameInSmallEnd==2)
						wxfMove.source='-';
				}else if(firstColSameInBigEnd+firstColSameInSmallEnd==3)
				{
					if(firstColSameInSmallEnd==0)
					{
						wxfMove.chessMan='+';
						wxfMove.source='+';
					}
					else if(firstColSameInSmallEnd==1)
					{
						wxfMove.chessMan='P';
						wxfMove.source='+';
					}
					else if(firstColSameInSmallEnd==2)
					{
						wxfMove.chessMan='P';
						wxfMove.source='-';
					}
					else
					{
						wxfMove.chessMan='-';
						wxfMove.source='-';
					}
				}else
				{
					if(firstColSameInSmallEnd==0)
					{
						wxfMove.chessMan='+';
						wxfMove.source='+';
					}
					else if(firstColSameInSmallEnd==1)
					{
						wxfMove.chessMan='P';
						wxfMove.source='+';
					}
					else if(firstColSameInSmallEnd==2)
					{
						wxfMove.chessMan='P';
						wxfMove.source=(char)(12-firstCol+'0');
					}
					else if(firstColSameInSmallEnd==3)
					{
						wxfMove.chessMan='P';
						wxfMove.source='-';
					}
					else
					{
						wxfMove.chessMan='-';
						wxfMove.source='-';
					}
				}
			}
			else if((isRedChessMan && !isRedOnBigEnd) || (!isRedChessMan && isRedOnBigEnd))
			{
				if(firstColSameInBigEnd+firstColSameInSmallEnd==1)
				{
					if(secondColSameInBigEnd+secondColSameInSmallEnd==0)
						wxfMove.chessMan='P';
					else
						wxfMove.chessMan=(char)(firstCol-2+'0');
					if(firstColSameInBigEnd==0)
						wxfMove.source='+';
					else
						wxfMove.source='-';
				}
				else if(firstColSameInBigEnd+firstColSameInSmallEnd==2)
				{
					if(secondColSameInBigEnd+secondColSameInSmallEnd==0)
						wxfMove.chessMan='P';
					else
						wxfMove.chessMan=(char)(firstCol-2+'0');
					if(firstColSameInSmallEnd==0)
						wxfMove.source='-';
					else if(firstColSameInSmallEnd==1)
						wxfMove.source=(char)(firstCol-2+'0');
					else if(firstColSameInSmallEnd==2)
						wxfMove.source='+';
				}else if(firstColSameInBigEnd+firstColSameInSmallEnd==3)
				{
					if(firstColSameInSmallEnd==3)
					{
						wxfMove.chessMan='+';
						wxfMove.source='+';
					}
					else if(firstColSameInSmallEnd==2)
					{
						wxfMove.chessMan='P';
						wxfMove.source='+';
					}
					else if(firstColSameInSmallEnd==1)
					{
						wxfMove.chessMan='P';
						wxfMove.source='-';
					}
					else
					{
						wxfMove.chessMan='-';
						wxfMove.source='-';
					}
				}else
				{
					if(firstColSameInSmallEnd==4)
					{
						wxfMove.chessMan='+';
						wxfMove.source='+';
					}
					else if(firstColSameInSmallEnd==3)
					{
						wxfMove.chessMan='P';
						wxfMove.source='+';
					}
					else if(firstColSameInSmallEnd==2)
					{
						wxfMove.chessMan='P';
						wxfMove.source=(char)(firstCol-2+'0');
					}
					else if(firstColSameInSmallEnd==1)
					{
						wxfMove.chessMan='P';
						wxfMove.source='-';
					}
					else
					{
						wxfMove.chessMan='-';
						wxfMove.source='-';
					}
				}
			}
			if(_chinessMove.act == UBString("平"))
				wxfMove.signal='.';
			else if(_chinessMove.act == UBString("进"))
				wxfMove.signal='+';
			else
				wxfMove.signal='-';
			try
			{
				wxfMove.dest=(char)(getNumber(_chinessMove.to)+'0');
			}catch(UBException& e)
			{
				wxfMove.dest=(char)(getNumber(_chinessMove.to,0)+'0');
			}
		}
	}
	return wxfMove;
}
//由于在WXF表示法中对于红方、黑方没有差别，所以这里的红方黑方从_cChess中获取
//在这里遵循部分中文纵线的规定
//对于多兵：搜索整个棋盘，获得兵的分布情况，然后分为是否为双列
Move MoveMaker::translateToMove(SmartPtr<CChessStruct> _cChess,const WXFMove& _wxfMove)
{
	if(_cChess.getPtr()==NULL)
		throw UBException(-1,"_cChess is null pointer..");
	Move _move;
	ChinessMove _chinessMove;
	bool isRed=_cChess->isRed;
	if(_wxfMove.source>='1' && _wxfMove.source<='9' && _wxfMove.chessMan != 'P' && (_wxfMove.chessMan < '1' || _wxfMove.chessMan > '9'))
	{
		if(isRed)
		{
			if(_wxfMove.chessMan == 'K')
				_chinessMove.chessMan=UBString("帅");
			else if(_wxfMove.chessMan == 'E')
				_chinessMove.chessMan=UBString("相");
			else if(_wxfMove.chessMan == 'A')
				_chinessMove.chessMan=UBString("仕");
			else if(_wxfMove.chessMan == 'R')
				_chinessMove.chessMan=UBString("车");
			else if(_wxfMove.chessMan == 'H')
				_chinessMove.chessMan=UBString("马");
			else if(_wxfMove.chessMan == 'C')
				_chinessMove.chessMan=UBString("炮");
			_chinessMove.from=getNumberStr(_wxfMove.source-'0');
			_chinessMove.to=getNumberStr(_wxfMove.dest-'0');
		}
		else
		{
			if(_wxfMove.chessMan == 'K')
				_chinessMove.chessMan=UBString("将");
			else if(_wxfMove.chessMan == 'E')
				_chinessMove.chessMan=UBString("象");
			else if(_wxfMove.chessMan == 'A')
				_chinessMove.chessMan=UBString("士");
			else if(_wxfMove.chessMan == 'R')
				_chinessMove.chessMan=UBString("车");
			else if(_wxfMove.chessMan == 'H')
				_chinessMove.chessMan=UBString("马");
			else if(_wxfMove.chessMan == 'C')
				_chinessMove.chessMan=UBString("炮");
			_chinessMove.from=getNumberStr(_wxfMove.source-'0',0);
			_chinessMove.to=getNumberStr(_wxfMove.dest-'0',0);
		}
		if(_wxfMove.signal == '.')
		{
			_chinessMove.act=UBString("平");
		}else if(_wxfMove.signal == '+')
		{
			_chinessMove.act=UBString("进");
		}else
		{
			_chinessMove.act=UBString("退");
		}
		_move=translateToMove(_cChess,_chinessMove);
	}
	else
	{
		//进入这里的都是前后、或者兵情况
		if(_wxfMove.chessMan != 'P' && (_wxfMove.chessMan < '1' || _wxfMove.chessMan > '9'))
		{
			if(_wxfMove.chessMan == 'R')
				_chinessMove.from=UBString("车");
			else if(_wxfMove.chessMan == 'H')
				_chinessMove.from=UBString("马");
			else if(_wxfMove.chessMan == 'C')
				_chinessMove.from=UBString("炮");
			if(_wxfMove.signal == '+')
				_chinessMove.act=UBString("进");
			else if(_wxfMove.signal == '-')
				_chinessMove.act=UBString("退");
			else if(_wxfMove.signal == '.')
				_chinessMove.act=UBString("平");
			if(_wxfMove.source == '+')
				_chinessMove.chessMan=UBString("前");
			else
				_chinessMove.chessMan=UBString("后");
			if(isRed)
				_chinessMove.to=getNumberStr(_wxfMove.dest-'0');
			else
				_chinessMove.to=getNumberStr(_wxfMove.dest-'0',0);
			_move=translateToMove(_cChess,_chinessMove);
		}
		else
		{
			//多兵的情况
			bool isRedOnBigEnd=false;
			if(_cChess->cChessManPosition[0] > 8*16)
				isRedOnBigEnd=true;
			int tempChessManValue,col;
			int checkNum=0;
			int firstSameCol=0,firstSameNumber=0,secondSameCol=0,secondSameNumber=0;
			int firstChessManGroup[5];
			int secondChessManGroup[3];
			if(isRed)
				_chinessMove.from=UBString("兵");
			else
				_chinessMove.from=UBString("卒");
			//这里是有P3.4可能是多兵，也可能是但兵，需要验证
			bool isRealMuti=true;
			if(_wxfMove.chessMan == 'P' && _wxfMove.source >= '1' && _wxfMove.source <= '9')
			{
				if((isRed && isRedOnBigEnd) || (!isRed && !isRedOnBigEnd))
					col=12-(_wxfMove.source-'0');
				else
					col=(_wxfMove.source-'0')+2;
				for(int i=3;i<=12;i++)
					if(getChessManChinessName(_cChess->cChessArray[i*16+col])==_chinessMove.from)
						++checkNum;
				if(checkNum >= 2)
					isRealMuti=true;
				else
					isRealMuti=false;
			}
			if(!isRealMuti)
			{
				if(isRed)
				{
					_chinessMove.from=getNumberStr(_wxfMove.source-'0');
					_chinessMove.to=getNumberStr(_wxfMove.dest-'0');
				}
				else
				{
					_chinessMove.from=getNumberStr(_wxfMove.source-'0',0);
					_chinessMove.to=getNumberStr(_wxfMove.dest-'0',0);
				}
				if(_wxfMove.signal == '.')
				{
					_chinessMove.act=UBString("平");
				}else if(_wxfMove.signal == '+')
				{
					_chinessMove.act=UBString("进");
				}else
				{
					_chinessMove.act=UBString("退");
				}
				_move=translateToMove(_cChess,_chinessMove);
			}
			else
			{
				for(int j=3;j<=11;j++)
				{
					checkNum=0;
					for(int i=3;i<=12;i++)
					{
						tempChessManValue=_cChess->cChessArray[i*16+j];
						if(tempChessManValue!=0 && 
						   getChessManChinessName(tempChessManValue)==_chinessMove.from)
						{
							++checkNum;
							if(checkNum==1 && firstSameCol==0)
								firstChessManGroup[0]=i*16+j;
							if(checkNum==1 && firstSameCol!=0)
								secondChessManGroup[0]=i*16+j;
							if(checkNum >= 2)
							{
								if(firstSameCol==0 || firstSameCol==j)
								{
									firstSameCol=j;
									firstSameNumber=checkNum;
									firstChessManGroup[firstSameNumber-1]=i*16+j;
								}
								else
								{
									secondSameCol=j;
									secondSameNumber=checkNum;
									secondChessManGroup[secondSameNumber-1]=i*16+j;
								}
							}
						}
					}
				}
				if((isRed && isRedOnBigEnd) || (!isRed && !isRedOnBigEnd))
				{
					if(_wxfMove.chessMan >= '1' && _wxfMove.chessMan <='9')
					{
						if((12-(_wxfMove.chessMan-'0')) == firstSameCol)
						{
							if(_wxfMove.source == '+')
								_move.from=firstChessManGroup[0];
							else if(_wxfMove.source == '-')
								_move.from=firstChessManGroup[firstSameNumber-1];
							else
								_move.from=firstChessManGroup[1];
						}
						else
						{
							if(_wxfMove.source == '+')
								_move.from=secondChessManGroup[0];
							else if(_wxfMove.source == '-')
								_move.from=secondChessManGroup[secondSameNumber-1];
							else
								_move.from=secondChessManGroup[1];
						}
					}
					else if(firstSameNumber == 2)
					{
						if(_wxfMove.source == '+')
							_move.from=firstChessManGroup[0];
						else
							_move.from=firstChessManGroup[1];
					}
					else if(firstSameNumber == 3)
					{
						if(_wxfMove.source == '+')
							_move.from=firstChessManGroup[0];
						else if(_wxfMove.source == '-')
							_move.from=firstChessManGroup[firstSameNumber-1];
						else
							_move.from=firstChessManGroup[1];
					}
					else if(firstSameNumber == 4)
					{
						if(_wxfMove.chessMan == '+')
							_move.from=firstChessManGroup[0];
						else if(_wxfMove.source == '+')
							_move.from=firstChessManGroup[1];
						else if(_wxfMove.chessMan == '-')
							_move.from=firstChessManGroup[3];
						else
							_move.from=firstChessManGroup[2];
					}
					else if(firstSameNumber == 5)
					{
						if(_wxfMove.chessMan == '+')
							_move.from=firstChessManGroup[0];
						else if(_wxfMove.source == '+')
							_move.from=firstChessManGroup[1];
						else if(_wxfMove.chessMan == '-')
							_move.from=firstChessManGroup[4];
						else if(_wxfMove.source == '-')
							_move.from=firstChessManGroup[3];
						else
							_move.from=firstChessManGroup[2];
					}
					if(_wxfMove.signal == '.')
						_move.to=_move.from+(12-(_wxfMove.dest-'0')-_move.from%16);
					else if(_wxfMove.signal == '+')
						_move.to=_move.from-16;
					else
						_move.to=_move.from+16;
				}
				else
				{
					if(_wxfMove.chessMan >= '1' && _wxfMove.chessMan <='9')
					{
						if(((_wxfMove.chessMan-'0')+2) == firstSameCol)
						{
							if(_wxfMove.source == '+')
								_move.from=firstChessManGroup[firstSameNumber-1];
							else if(_wxfMove.source == '-')
								_move.from=firstChessManGroup[0];
							else
								_move.from=firstChessManGroup[1];
						}
						else
						{
							if(_wxfMove.source == '+')
								_move.from=secondChessManGroup[secondSameNumber-1];
							else if(_wxfMove.source == '-')
								_move.from=secondChessManGroup[0];
							else
								_move.from=secondChessManGroup[1];
						}
					}
					else if(firstSameNumber == 2)
					{
						if(_wxfMove.source == '+')
							_move.from=firstChessManGroup[1];
						else
							_move.from=firstChessManGroup[0];
					}
					else if(firstSameNumber == 3)
					{
						if(_wxfMove.source == '+')
							_move.from=firstChessManGroup[firstSameNumber-1];
						else if(_wxfMove.source == '-')
							_move.from=firstChessManGroup[0];
						else
							_move.from=firstChessManGroup[1];
					}
					else if(firstSameNumber == 4)
					{
						if(_wxfMove.chessMan == '+')
							_move.from=firstChessManGroup[3];
						else if(_wxfMove.source == '+')
							_move.from=firstChessManGroup[2];
						else if(_wxfMove.chessMan == '-')
							_move.from=firstChessManGroup[0];
						else
							_move.from=firstChessManGroup[1];
					}
					else
					{
						if(_wxfMove.chessMan == '+')
							_move.from=firstChessManGroup[4];
						else if(_wxfMove.source == '+')
							_move.from=firstChessManGroup[3];
						else if(_wxfMove.chessMan == '-')
							_move.from=firstChessManGroup[0];
						else if(_wxfMove.source == '-')
							_move.from=firstChessManGroup[1];
						else
							_move.from=firstChessManGroup[2];
					}
					if(_wxfMove.signal == '.')
						_move.to=_move.from+((_wxfMove.dest-'0')+2-_move.from%16);
					else if(_wxfMove.signal == '+')
						_move.to=_move.from+16;
					else
						_move.to=_move.from-16;
				}
				_move.capture=_cChess->cChessArray[_move.to];
			}
		}	
	}
	return _move;
}
UBString MoveMaker::getChessManChinessName(int chessManValue)
{
	switch(chessManValue)
	{
		case redKing:return UBString("帅");
		case blackKing:return UBString("将");
		case redAdvisorOne:
		case redAdvisorTwo:return UBString("仕");
		case blackAdvisorOne:
		case blackAdvisorTwo:return UBString("士");
		case redBishopOne:
		case redBishopTwo:return UBString("相");
		case blackBishopOne:
		case blackBishopTwo:return UBString("象");
		case redKnightOne:
		case redKnightTwo:
		case blackKnightOne:
		case blackKnightTwo:return UBString("马");
		case redRookOne:
		case redRookTwo:
		case blackRookOne:
		case blackRookTwo:return UBString("车");
		case redCannonOne:
		case redCannonTwo:
		case blackCannonOne:
		case blackCannonTwo:return UBString("炮");
		case redPawnOne:
		case redPawnTwo:
		case redPawnThree:
		case redPawnFour:
		case redPawnFive:return UBString("兵");
		case blackPawnOne:
		case blackPawnTwo:
		case blackPawnThree:
		case blackPawnFour:
		case blackPawnFive:return UBString("卒");
		default:
			return UBString();
	}
}
char MoveMaker::getChessManWXFName(const UBString& _chinessName)
{
	if(_chinessName == UBString("帅") || _chinessName == UBString("将"))
		return 'K';
	else if(_chinessName == UBString("卒") || _chinessName == UBString("兵"))
		return 'P';
	else if(_chinessName == UBString("相") || _chinessName == UBString("象"))
		return 'E';
	else if(_chinessName == UBString ("仕") || _chinessName == UBString("士"))
		return 'A';
	else if(_chinessName == UBString("马"))
		return 'H';
	else if(_chinessName == UBString("车"))
		return 'R';
	else if(_chinessName == UBString("炮"))
		return 'C';
	else
		throw UBException(-1,"Not legal chiness name for WXF");
}
bool MoveMaker::isChessManChinessName(const UBString& _chessManName)
{
	if(_chessManName == UBString("帅") || _chessManName == UBString("仕") || _chessManName == UBString("相") ||
	   _chessManName == UBString("兵") || _chessManName == UBString("将") || _chessManName == UBString("士") || 
	   _chessManName == UBString("象") || _chessManName == UBString("卒") || _chessManName == UBString("马") || 
	   _chessManName == UBString("车") || _chessManName == UBString("炮") )
		return true;
	else
		return false;
}
bool MoveMaker::isChessManWXFName(char _wxfName)
{
	if(_wxfName == 'K' || _wxfName == 'P' || _wxfName == 'E' || _wxfName == 'A' ||
	   _wxfName == 'R' || _wxfName == 'H' || _wxfName == 'C')
		return true;
	else
		return false;
}
UBString MoveMaker::getNumberStr(int number)
{
	switch(number)
	{
		case 1:
			return UBString("一");
		case 2:
			return UBString("二");
		case 3:
			return UBString("三");
		case 4:
			return UBString("四");
		case 5:
			return UBString("五");
		case 6:
			return UBString("六");
		case 7:
			return UBString("七");
		case 8:
			return UBString("八");
		case 9:
			return UBString("九");
		default:
			return UBString();
	}
}
//第二个参数只是为了区别，形成重载
UBString MoveMaker::getNumberStr(int number,int diff)
{
	switch(number)
	{
		case 1:
			return UBString("1");
		case 2:
			return UBString("2");
		case 3:
			return UBString("3");
		case 4:
			return UBString("4");
		case 5:
			return UBString("5");
		case 6:
			return UBString("6");
		case 7:
			return UBString("7");
		case 8:
			return UBString("8");
		case 9:
			return UBString("9");
		default:
			return UBString();
	}
}
int MoveMaker::getNumber(const UBString& _number)
{
	if(_number == UBString("一"))
	{
		return 1;
	}
	else if(_number == UBString("二"))
	{
		return 2;
	}
	else if(_number == UBString("三"))
	{
		return 3;
	}
	else if(_number == UBString("四"))
	{
		return 4;
	}
	else if(_number == UBString("五"))
	{
		return 5;
	}
	else if(_number == UBString("六"))
	{
		return 6;
	}
	else if(_number == UBString("七"))
	{
		return 7;
	}
	else if(_number == UBString("八"))
	{
		return 8;
	}
	else if(_number == UBString("九"))
	{
		return 9;
	}
	else
		throw UBException(-1,"No legal UBString for number");
}
int MoveMaker::getNumber(const UBString& _number,int diff)
{
	if(_number == UBString("1"))
	{
		return 1;
	}
	else if(_number == UBString("2"))
	{
		return 2;
	}
	else if(_number == UBString("3"))
	{
		return 3;
	}
	else if(_number == UBString("4"))
	{
		return 4;
	}
	else if(_number == UBString("5"))
	{
		return 5;
	}
	else if(_number == UBString("6"))
	{
		return 6;
	}
	else if(_number == UBString("7"))
	{
		return 7;
	}
	else if(_number == UBString("8"))
	{
		return 8;
	}
	else if(_number == UBString("9"))
	{
		return 9;
	}
	else
		throw UBException(-1,"No legal UBString for number");
}
