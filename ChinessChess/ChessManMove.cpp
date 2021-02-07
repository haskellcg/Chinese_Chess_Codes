#include "ChessManMove.h"
#include "UBException.h"
#include <iostream>
using std::cout;
using std::endl;
int kingMoveOff[4]={-16,1,16,-1};
int advisorMoveOff[4]={-17,-15,17,15};
int bishopMoveOff[4]={-34,-30,34,30};
int bishopEye[4]={-17,-15,17,15};
int knightMoveOff[8]={14,-18,-33,-31,-14,18,33,31};
int knightLeg[8]={-1,-1,-16,-16,1,1,16,16};
int rookMoveOff[4]={-1,-16,1,16};
int cannonMoveOff[4]={-1,-16,1,16};
int pawnMoveOff[2][3]={
			{-1,-16,1},
			{1,16,-1}
			};					//默认第一组为高端地址
void ChessManMove::initializeLegalPosition()
{
	//第一组在数组的高地址端
	//采用对称方法，i----16-i  j----j
	for(int i=0;i<256;i++)
		legalPosition[0][i]=legalPosition[1][i]=0;

	int knight_cannon_rook=cannonLegalPosVal | knightLegalPosVal | rookLegalPosVal;
	int base=3*16+3;
	for(int i=3;i<=12;i++)
	{
		for(int j=3;j<=11;j++)
		{
			legalPosition[0][base]=legalPosition[0][base] | knight_cannon_rook;
			legalPosition[1][base]=legalPosition[1][base] | knight_cannon_rook;
			++base;
		}
		base+=7;
	}
	base=10*16+6;
	for(int i=10;i<=12;i++)
	{
		for(int j=6;j<=8;j++)
		{
			legalPosition[0][base]=legalPosition[0][base] | kingLegalPosVal;
			legalPosition[1][240-base+2*j]=legalPosition[1][240-base+2*j] | kingLegalPosVal;
			++base;
		}
		base+=13;
	}
	base=10*16+6;
	int advisorLegalPosOff[5]={0,2,15,15,2};
	for(int i=0;i<5;i++)
	{
		base=base+advisorLegalPosOff[i];
		legalPosition[0][base]=legalPosition[0][base] | advisorLegalPosVal;
		int j=base%16;
		legalPosition[1][240-base+2*j]=legalPosition[1][240-base+2*j] | advisorLegalPosVal;
	}
	base=8*16+5;
	int bishopLegalPosOff[7]={0,4,26,4,4,26,4};
	for(int i=0;i<7;i++)
	{
		base=base+bishopLegalPosOff[i];
		legalPosition[0][base]=legalPosition[0][base] | bishopLegalPosVal;
		int j=base%16;
		legalPosition[1][240-base+2*j]=legalPosition[1][240-base+2*j] | bishopLegalPosVal;
	}
	base=8*16+3;
	int somePawnLegalPosOff[10]={0,2,2,2,2,8,2,2,2,2};
	for(int i=0;i<10;i++)
	{
		base=base+somePawnLegalPosOff[i];
		legalPosition[0][base]=legalPosition[0][base] | pawnLegalPosVal;
		int j=base%16;
		legalPosition[1][240-base+2*j]=legalPosition[1][240-base+2*j] | pawnLegalPosVal;
	}
	base=3*16+3;
	for(int i=3;i<=7;i++)
	{
		for(int j=3;j<=11;j++)
		{
			legalPosition[0][base]=legalPosition[0][base] | pawnLegalPosVal;
			legalPosition[1][240-base+2*j]=legalPosition[1][240-base+2*j] | pawnLegalPosVal;
			++base;
		}
		base+=7;
	}
}
bool ChessManMove::isInLegalPosition(int source,int chessManPosVal,int position) const
{
	if(position<0 || position>255 || source<0 || source>255)
	{
		cout<<"ERROR:Out of cChessArray index...."<<endl;
		return false;
	}
	if(cChess->cChessArray[source]==0)
	{
		cout<<"No Chess Man In "<<source<<endl;
		return false;
	}
	//判断是否红方对应legalPosition的第一组
	int index=0;
	bool isRedChessMan=false;
	if(cChess->cChessArray[source] <32)
		isRedChessMan=true;
	if(cChess->cChessManPosition[0] > 8*16)
	{
		index=(isRedChessMan==true?0:1);
	}
	else
	{
		index=(isRedChessMan==true?1:0);
	}
	return (chessManPosVal & legalPosition[index][position]);
}
void ChessManMove::saveLegalMove(int _from,int _to,int _capture)
{
	if(_from<0 || _from>255 || _to<0 || _to>255 || _capture<0 || (_capture>0 && _capture<16) || _capture>47)
	{
		cout<<"ERROR:Out of Bound...."<<endl;
		return;
	}
	this->moveArray[moveArrayLen].from=_from;
	this->moveArray[moveArrayLen].to=_to;
	this->moveArray[moveArrayLen].capture=_capture;
	++moveArrayLen;
}
//前提：position处有棋子
bool ChessManMove::isSelfSideChessMan(int source,int position) const
{
	if(position<0 || position>255 || source<0 || source>255)
	{
		cout<<"ERROR:Out of cChessArray index...."<<endl;
		return false;
	}
	bool isPositionRedChessMan=(cChess->cChessArray[position] < 32);
	bool isSourceRedChessMan=(cChess->cChessArray[source]<32);
	return (isSourceRedChessMan == isPositionRedChessMan);
}
bool ChessManMove::isGeneral(int position) const
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index...."<<endl;
		return false;
	}
	return (cChess->cChessArray[position]==redKing || cChess->cChessArray[position]==blackKing);
}
bool ChessManMove::genKnightMove(bool capture,int position,bool* returnCheck)
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index...."<<endl;
		return false;
	}
	if(cChess->cChessArray[position] != redKnightOne &&
	   cChess->cChessArray[position] != redKnightTwo &&
	   cChess->cChessArray[position] != blackKnightOne &&
	   cChess->cChessArray[position] != blackKnightTwo)
	{
		cout<<"ERROR:Not Is Knight in "<<position<<endl;
		return false; 
	}
	if(!isInLegalPosition(position,knightLegalPosVal,position))
		return false;
	int dest=0,leg=0;
	bool flag=false;
	*returnCheck=false;
	for(int i=0;i<8;i++)
	{
		dest=position+knightMoveOff[i];
		leg=position+knightLeg[i];
		if(!isInLegalPosition(position,knightLegalPosVal,dest))
			continue;
		if(cChess->cChessArray[leg] !=0)
			continue;
		if(cChess->cChessArray[dest]!=0)
		{
			if(isSelfSideChessMan(position,dest))
				continue;
			else
			{
				if(capture)
				{
					saveLegalMove(position,dest,cChess->cChessArray[dest]);
					if(isGeneral(dest))
						*returnCheck=true;
					flag=true;
				}
			}
		}
		else
		{
			if(!capture)
			{
				saveLegalMove(position,dest,0);
				flag=true;
			}
		}
	}
	return flag;
}
bool ChessManMove::genBishopMove(bool capture,int position,bool* returnCheck)
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index...."<<endl;
		return false;
	}
	if(cChess->cChessArray[position] != redBishopOne &&
	   cChess->cChessArray[position] != redBishopTwo &&
	   cChess->cChessArray[position] != blackBishopOne &&
	   cChess->cChessArray[position] != blackBishopTwo)
	{
		cout<<"ERROR:Not Is Bishop in "<<position<<endl;
		return false; 
	}
	if(!isInLegalPosition(position,bishopLegalPosVal,position))
		return false;
	int dest=0,eye=0;
	bool flag=false;
	*returnCheck=false;
	for(int i=0;i<4;i++)
	{
		dest=position+bishopMoveOff[i];
		eye=position+bishopEye[i];
		if(!isInLegalPosition(position,bishopLegalPosVal,dest))
			continue;
		if(cChess->cChessArray[eye]!=0)
			continue;
		if(cChess->cChessArray[dest]!=0)
		{
			if(!isSelfSideChessMan(position,dest))
			{
				if(capture)
				{
					saveLegalMove(position,dest,cChess->cChessArray[dest]);
					flag=true;
				}
			}
		}
		else
		{
			if(!capture)
			{
				saveLegalMove(position,dest,0);
				flag=true;
			}
		}
	}
	return flag;
}
bool ChessManMove::genKingMove(bool capture,int position,bool* returnCheck)
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index...."<<endl;
		return false;
	}
	if(cChess->cChessArray[position] != redKing &&
	   cChess->cChessArray[position] != blackKing)
	{
		cout<<"ERROR:Not Is King in "<<position<<endl;
		return false; 
	}
	if(!isInLegalPosition(position,kingLegalPosVal,position))
		return false;
	int dest=0;
	bool flag=false;
	*returnCheck=false;
	for(int i=0;i<4;i++)
	{
		dest=position+kingMoveOff[i];
		if(!isInLegalPosition(position,kingLegalPosVal,dest))
			continue;
		if(cChess->cChessArray[dest]!=0)
		{
			if(!isSelfSideChessMan(position,dest))
			{
				if(capture)
				{
					saveLegalMove(position,dest,cChess->cChessArray[dest]);
					flag=true;
				}
			}
		}
		else
		{
			if(!capture)
			{
				saveLegalMove(position,dest,0);
				flag=true;
			}
		}
	}
	return flag;
}
bool ChessManMove::genAdvisorMove(bool capture,int position,bool* returnCheck)
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index...."<<endl;
		return false;
	}
	if(cChess->cChessArray[position] != redAdvisorOne &&
	   cChess->cChessArray[position] != redAdvisorTwo &&
	   cChess->cChessArray[position] != blackAdvisorOne &&
	   cChess->cChessArray[position] != blackAdvisorTwo)
	{
		cout<<"ERROR:Not Is Advisor in "<<position<<endl;
		return false; 
	}
	if(!isInLegalPosition(position,advisorLegalPosVal,position))
		return false;
	int dest=0;
	bool flag=false;
	*returnCheck=false;
	for(int i=0;i<4;i++)
	{
		dest=position+advisorMoveOff[i];
		if(!isInLegalPosition(position,advisorLegalPosVal,dest))
			continue;
		if(cChess->cChessArray[dest]!=0)
		{
			if(!isSelfSideChessMan(position,dest))
			{
				if(capture)
				{
					saveLegalMove(position,dest,cChess->cChessArray[dest]);
					flag=true;
				}
			}
		}
		else
		{
			if(!capture)
			{
				saveLegalMove(position,dest,0);
				flag=true;
			}
		}
	}
	return flag;
}
bool ChessManMove::genRookMove(bool capture,int position,bool* returnCheck)
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index...."<<endl;
		return false;
	}
	if(cChess->cChessArray[position] != redRookOne &&
	   cChess->cChessArray[position] != redRookTwo &&
	   cChess->cChessArray[position] != blackRookOne &&
	   cChess->cChessArray[position] != blackRookTwo)
	{
		cout<<"ERROR:Not Is Rook in "<<position<<endl;
		return false; 
	}
	if(!isInLegalPosition(position,rookLegalPosVal,position))
		return false;
	int direction=0,dest=0;
	bool flag=false;
	*returnCheck=false;
	for(int i=0;i<4;i++)
	{
		direction=rookMoveOff[i];
		dest=position+direction;
		while(isInLegalPosition(position,rookLegalPosVal,dest))
		{
			if(cChess->cChessArray[dest]==0 && !capture)
			{
				saveLegalMove(position,dest,0);
				flag=true;
			}
			else if(cChess->cChessArray[dest]!=0)
			{
				if(!isSelfSideChessMan(position,dest) && capture)
				{
					saveLegalMove(position,dest,cChess->cChessArray[dest]);
					if(isGeneral(dest))
						*returnCheck=true;
					flag=true;
				}
				break;
			}
			dest+=direction;
		}
	}
	return flag;
}
bool ChessManMove::genCannonMove(bool capture,int position,bool* returnCheck)
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index...."<<endl;
		return false;
	}
	if(cChess->cChessArray[position] != redCannonOne &&
	   cChess->cChessArray[position] != redCannonTwo &&
	   cChess->cChessArray[position] != blackCannonOne &&
	   cChess->cChessArray[position] != blackCannonTwo)
	{
		cout<<"ERROR:Not Is Cannon in "<<position<<endl;
		return false; 
	}
	if(!isInLegalPosition(position,cannonLegalPosVal,position))
		return false;
	int direction=0,dest=0;
	bool flag=false;
	*returnCheck=false;
	for(int i=0;i<4;i++)
	{
		direction=cannonMoveOff[i];
		dest=position+direction;
		while(isInLegalPosition(position,cannonLegalPosVal,dest))
		{
			if(cChess->cChessArray[dest]==0 && !capture)
			{
				saveLegalMove(position,dest,0);
				flag=true;
			}
			else if(cChess->cChessArray[dest]!=0)
			{
				if(!capture)
					break;
				else
				{
					dest+=direction;
					while(isInLegalPosition(position,cannonLegalPosVal,dest))
					{
						if(cChess->cChessArray[dest]!=0 && !isSelfSideChessMan(position,dest))
						{
							saveLegalMove(position,dest,cChess->cChessArray[dest]);
							if(isGeneral(dest))
								*returnCheck=true;
							flag=true;
							break;
						}
						dest+=direction;
					}
				}
				break;
			}
			dest+=direction;
		}
	}
	return flag;
}
bool ChessManMove::genPawnMove(bool capture,int position,bool* returnCheck)
{
	if(position<0 || position>255)
	{
		cout<<"ERROR:Out of cChessArray index...."<<endl;
		return false;
	}
	if(cChess->cChessArray[position] < redPawnOne ||
	   cChess->cChessArray[position] > redPawnFive &&
	   cChess->cChessArray[position] < blackPawnOne ||
	   cChess->cChessArray[position] > blackPawnFive)
	{
		cout<<"ERROR:Not Is Pawn in "<<position<<endl;
		return false; 
	}
	if(!isInLegalPosition(position,pawnLegalPosVal,position))
		return false;
	bool redIsOnBigEnd,pawnIsRed,isCrossRiver;
	int index;
	int dest=0;
	bool flag=false;
	*returnCheck=false;
	if(cChess->cChessManPosition[0] > 8*16)
		redIsOnBigEnd=true;
	else
		redIsOnBigEnd=false;
	pawnIsRed=(cChess->cChessArray[position]<32)?true:false;
	index=(pawnIsRed == redIsOnBigEnd)?0:1;
	if(index==0)
		isCrossRiver=(position/16 <= 7);
	else
		isCrossRiver=(position/16 >= 8);
	//上面代码检测兵是否为红方、红方是否为局面数组的高端地址，从而判断兵的走向以及是否过河
	if(isCrossRiver)
	{
		for(int i=0;i<3;i++)
		{
			dest=position+pawnMoveOff[index][i];
			if(!isInLegalPosition(position,pawnLegalPosVal,dest))
				continue;
			if(cChess->cChessArray[dest]!=0)
			{
				if(!isSelfSideChessMan(position,dest) && capture)
				{
					saveLegalMove(position,dest,cChess->cChessArray[dest]);
					if(isGeneral(dest))
						*returnCheck=true;
					flag=true;
				}
			}
			else
			{
				if(!capture)
				{
					saveLegalMove(position,dest,0);
					flag=true;
				}
			}
		}
	}
	else
	{
		if(index==0)
			dest=position-16;
		else
			dest=position+16;
		if(!isInLegalPosition(position,pawnLegalPosVal,dest))
			return false;	
		if(cChess->cChessArray[dest]!=0)
		{
			if(!isSelfSideChessMan(position,dest) && capture)
			{
				saveLegalMove(position,dest,cChess->cChessArray[dest]);
				flag=true;
			}
		}
		else
		{
			if(!capture)
			{
				saveLegalMove(position,dest,0);
				flag=true;
			}
		}
	}
	return flag;
}
bool ChessManMove::isSameMove(const Move& lmove,const Move& rmove)
{
	return ((lmove.from == rmove.from) && (lmove.to == rmove.to) && (lmove.capture == rmove.capture));
}
void ChessManMove::moveArraySort()
{
	int noCaptureMoveIndex=-1;
	int bestMoveIndex=-1;
	int killerMoveIndex[5]={-1,-1,-1,-1,-1};
	int headIndex=0;
	for(int i=0;i<moveArrayLen;i++)
	{
		if(moveArray[i].capture==0 && noCaptureMoveIndex==-1)
			noCaptureMoveIndex=i;
		for(int j=0;j<killerMoveLen;j++)
		{
			if(isSameMove(moveArray[i],killerMove[j]))
				killerMoveIndex[j]=i;
		}
		if(isSameMove(moveArray[i],bestMove))
			bestMoveIndex=i;
	}
	Move temp;
	//交换bestMove
	if(bestMoveIndex == -1)
	{
	}
	else
	{
		//以下的判断都是为了保证吃子走法连续
		//首先判断要替换的走法不重复
		//如果只有吃子走法、只有非吃子走法、要替换的部分已经超过界限，则直接替换，应为这样不会影响整体的排列
		//否则就必须显赫边界的节点替换，然后与headIndex替换
		//注意在第二种中noCaptureMoveIndex变量的递增
		if(bestMoveIndex >= noCaptureMoveIndex)
		{
			if((noCaptureMoveIndex != -1) && (noCaptureMoveIndex != 0) && (headIndex < noCaptureMoveIndex))
			{
				if(bestMoveIndex != noCaptureMoveIndex)
				{
					temp=moveArray[noCaptureMoveIndex];
					moveArray[noCaptureMoveIndex]=moveArray[bestMoveIndex];
					moveArray[bestMoveIndex]=temp;
				}

				temp=moveArray[headIndex];
				moveArray[headIndex]=moveArray[noCaptureMoveIndex];
				moveArray[noCaptureMoveIndex]=temp;
				if(noCaptureMoveIndex == (moveArrayLen-1))
					noCaptureMoveIndex=-1;
				else
					++noCaptureMoveIndex;
			}
			else
			{
				if(bestMoveIndex != headIndex)
				{
					temp=moveArray[headIndex];
					moveArray[headIndex]=moveArray[bestMoveIndex];
					moveArray[bestMoveIndex]=temp;
				}
			}
		}
		else
		{
				temp=moveArray[0];
				moveArray[0]=moveArray[bestMoveIndex];
				moveArray[bestMoveIndex]=temp;
		}
	}
	//headIndex+1，只是用于替换的指针，不用检测其
	++headIndex;
	//交换killerMove
	for(int i=0;i<killerMoveLen;i++)
	{
		if((killerMoveIndex[i] != -1) && !isSameMove(killerMove[i],bestMove))
		{
			if(killerMoveIndex[i] >= noCaptureMoveIndex)
			{
				if((noCaptureMoveIndex != -1) && (noCaptureMoveIndex != 0) && (headIndex < noCaptureMoveIndex))
				{
					if(killerMoveIndex[i] != noCaptureMoveIndex)
					{
						temp=moveArray[killerMoveIndex[i]];
						moveArray[killerMoveIndex[i]]=moveArray[noCaptureMoveIndex];
						moveArray[noCaptureMoveIndex]=temp;
					}
					
					temp=moveArray[headIndex];
					moveArray[headIndex]=moveArray[noCaptureMoveIndex];
					moveArray[noCaptureMoveIndex]=temp;
					if(noCaptureMoveIndex == (moveArrayLen-1))
						noCaptureMoveIndex=-1;
					else
						++noCaptureMoveIndex;
				}
				else
				{
					if(killerMoveIndex[i] != headIndex)
					{
						temp=moveArray[headIndex];
						moveArray[headIndex]=moveArray[noCaptureMoveIndex];
						moveArray[noCaptureMoveIndex]=temp;
					}
				}
			}
			else
			{
				temp=moveArray[headIndex];
				moveArray[headIndex]=moveArray[noCaptureMoveIndex];
				moveArray[noCaptureMoveIndex]=temp;
			}
			++headIndex;
		}
	}
}
ChessManMove::ChessManMove():kingLegalPosVal(1),knightLegalPosVal(2),rookLegalPosVal(2),cannonLegalPosVal(2),
		             advisorLegalPosVal(4),bishopLegalPosVal(8),pawnLegalPosVal(16),cChess(NULL)
{
	CChessStruct* temp=new CChessStruct;
	this->cChess=makeSmartPtr(temp);
	this->moveArrayLen=0;
	this->killerMoveLen=0;
	this->checkingChessManPositionLen=0;
	this->bestMove.from=this->bestMove.to=this->bestMove.capture=0;
	initializeLegalPosition();
}
ChessManMove::ChessManMove(SmartPtr<CChessStruct> _cChess):kingLegalPosVal(1),knightLegalPosVal(2),rookLegalPosVal(2),cannonLegalPosVal(2),
						  advisorLegalPosVal(4),bishopLegalPosVal(8),pawnLegalPosVal(16),cChess(_cChess)
{
	if(cChess.getPtr() == NULL)
	{
		CChessStruct* temp=new CChessStruct(NULL);
		this->cChess=makeSmartPtr(temp);
	}
	this->moveArrayLen=0;
	this->killerMoveLen=0;
	this->checkingChessManPositionLen=0;
	this->bestMove.from=this->bestMove.to=this->bestMove.capture=0;
	initializeLegalPosition();
}
void ChessManMove::bindStruct(SmartPtr<CChessStruct> _cChess)
{
	if(_cChess.getPtr() != NULL)
	{
		this->cChess=_cChess;
		this->moveArrayLen=0;
		this->checkingChessManPositionLen=0;
	}
}
void ChessManMove::genAllMove()
{
	this->moveArrayLen=0;
	int index;
	int position=0;
	genMoveFunPtr funPtrArray[16]={
					&ChessManMove::genKingMove,&ChessManMove::genAdvisorMove,
					&ChessManMove::genAdvisorMove,&ChessManMove::genBishopMove,
					&ChessManMove::genBishopMove,&ChessManMove::genKnightMove,
					&ChessManMove::genKnightMove,&ChessManMove::genRookMove,
					&ChessManMove::genRookMove,&ChessManMove::genCannonMove,
					&ChessManMove::genCannonMove,&ChessManMove::genPawnMove,
					&ChessManMove::genPawnMove,&ChessManMove::genPawnMove,
					&ChessManMove::genPawnMove,&ChessManMove::genPawnMove
					};
	index=(cChess->isRed?0:16);
	for(int off=0;off<16;off++)
	{
		position=cChess->cChessManPosition[index+off];
		if(position!=0)
		{
			bool isCheck;
			(this->*funPtrArray[off])(true,position,&isCheck);
		}
	}
	for(int off=0;off<16;off++)
	{
		position=cChess->cChessManPosition[index+off];
		if(position!=0)
		{
			bool isCheck;
			(this->*funPtrArray[off])(false,position,&isCheck);
		}
	}
	//排序所有走法，包括成员中记录的最佳走法、以及杀手走法
	moveArraySort();
}
//产生所有吃子走法
void ChessManMove::genCaptureMove()
{
	this->moveArrayLen=0;
	int index;
	int position=0;
	genMoveFunPtr funPtrArray[16]={
					&ChessManMove::genKingMove,&ChessManMove::genAdvisorMove,
					&ChessManMove::genAdvisorMove,&ChessManMove::genBishopMove,
					&ChessManMove::genBishopMove,&ChessManMove::genKnightMove,
					&ChessManMove::genKnightMove,&ChessManMove::genRookMove,
					&ChessManMove::genRookMove,&ChessManMove::genCannonMove,
					&ChessManMove::genCannonMove,&ChessManMove::genPawnMove,
					&ChessManMove::genPawnMove,&ChessManMove::genPawnMove,
					&ChessManMove::genPawnMove,&ChessManMove::genPawnMove
					};
	index=(cChess->isRed?0:16);
	for(int off=0;off<16;off++)
	{
		position=cChess->cChessManPosition[index+off];
		if(position!=0)
		{
			bool isCheck;
			(this->*funPtrArray[off])(true,position,&isCheck);
		}
	}
	moveArraySort();
}
//检测是否将军，并且将军的棋子放在对应的数组中
bool ChessManMove::check()
{
	//为了不影响已生成的走法数组，这里重用私有函数时将moveArrayLen设为128,即源走法数组不超过128,且吃子走法不超过128个
	int oldMoveArrayLen=this->moveArrayLen;
		this->moveArrayLen=128;
		//为了计算对方对本方的将军，所以需要对isRed取反
		this->cChess->isRed=!cChess->isRed;
		int index;
		int position=0;
		this->checkingChessManPositionLen=0;
		genMoveFunPtr funPtrArray[16]={
						&ChessManMove::genKingMove,&ChessManMove::genAdvisorMove,
						&ChessManMove::genAdvisorMove,&ChessManMove::genBishopMove,
						&ChessManMove::genBishopMove,&ChessManMove::genKnightMove,
						&ChessManMove::genKnightMove,&ChessManMove::genRookMove,
						&ChessManMove::genRookMove,&ChessManMove::genCannonMove,
						&ChessManMove::genCannonMove,&ChessManMove::genPawnMove,
						&ChessManMove::genPawnMove,&ChessManMove::genPawnMove,
						&ChessManMove::genPawnMove,&ChessManMove::genPawnMove
						};
		index=(cChess->isRed?0:16);
		for(int off=0;off<16;off++)
		{
			position=cChess->cChessManPosition[index+off];
			if(position!=0)
			{
				bool isCheck;
				(this->*funPtrArray[off])(true,position,&isCheck);
				if(isCheck)
				{
					checkingChessManPosition[checkingChessManPositionLen]=position;
					++checkingChessManPositionLen;
				}
			}
		}
		
	this->cChess->isRed=!cChess->isRed;
	this->moveArrayLen=oldMoveArrayLen;
	return (checkingChessManPositionLen!=0);
}
bool ChessManMove::generalOpposite() const
{
	int redRow,redCol,blackRow,blackCol;
	int off,dest;
	redRow=cChess->cChessManPosition[0]/16;
	redCol=cChess->cChessManPosition[0]%16;
	blackRow=cChess->cChessManPosition[16]/16;
	blackCol=cChess->cChessManPosition[16]%16;
	if(redCol!=blackCol)
		return false;
	off=(redRow>blackRow?16:-16);
	dest=cChess->cChessManPosition[16] + off;
	while(dest < cChess->cChessManPosition[0])
	{
		if(cChess->cChessArray[dest]!=0)
			return false;
		dest+=off;
	}
	return true;
}
int ChessManMove::getCheckingChessManPosition(int index) const
{
	if(index<0 || index>checkingChessManPositionLen-1)
	{
		cout<<"ERROR:Out of index ..."<<endl;
		return -1;
	}
	return checkingChessManPosition[128+index];
}
int ChessManMove::getMoveArrayLen() const
{
	return this->moveArrayLen;
}
bool ChessManMove::getMove(int index,Move* _move) const
{
	if(index<0 || index>moveArrayLen-1)
	{
		cout<<"ERROR:Out of Index ..."<<endl;
		return false;
	}
	_move->from=moveArray[index].from;
	_move->to=moveArray[index].to;
	_move->capture=moveArray[index].capture;
	return true;
}
void ChessManMove::clearMoveArray()
{
	this->moveArrayLen=0;
}
void ChessManMove::setBestMove(const Move& _move)
{
	this->bestMove=_move;
}
//置换策略：FIFO
void ChessManMove::setKillerMove(const Move& _move)
{
	for(int i=0;i<killerMoveLen;i++)
	{
		if(isSameMove(killerMove[i],_move))
			return;
	}
	if(killerMoveLen>=5)
	{
		for(int i=0;i<killerMoveLen-1;i++)
			killerMove[i]=killerMove[i+1];
		killerMove[4]=_move;
	}
	else
	{
		killerMove[killerMoveLen++]=_move;
	}
}
void ChessManMove::clearKillerMove()
{
	this->killerMoveLen=0;
}
void ChessManMove::reportMove() const
{
	cout<<"moveArrayLen:"<<moveArrayLen<<endl;
	for(int i=0;i<moveArrayLen;i++)
	{
		cout<<"# "<<moveArray[i].from<<" "<<moveArray[i].to<<" "<<moveArray[i].capture<<"   ";
		if(i%3==0)
			cout<<endl;
	}
	cout<<endl<<"quoteNumber:"<<cChess.quoteNumber()<<endl;
}
