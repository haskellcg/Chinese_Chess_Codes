#include "UBTieTree.h"
#include <iostream>
#include <string>
#include <cstring>
//for memset
#include <cstdlib>
//for mbstowcs  and wcstombs
#include <ctime>
//for random time seed
using namespace std;
NodeData::NodeData():hitNumber(0),victoryScore(0)
{}
BaseNode::BaseNode():parent(NULL),brother(NULL),firstChild(NULL)
{}
NodeData& BaseNode::getData()
{
	return this->data;
}
void BaseNode::setData(const NodeData& _data)
{
	this->data=_data;
}
BaseNode* BaseNode::getParent() const
{
	return NULL;
}
BaseNode* BaseNode::getBrother() const
{
	return NULL;
}
BaseNode* BaseNode::getFirstChild() const
{
	return NULL;
}
void BaseNode::setParent(BaseNode *_parent)
{
	this->parent=_parent;
}
void BaseNode::setBrother(BaseNode *_brother)
{
	this->brother=_brother;
}
void BaseNode::setFirstChild(BaseNode *_child)
{
	this->firstChild=_child;
}
NodeType RootNode::getNodeType() const
{
	return ROOT_NODE;
}
BaseNode* RootNode::getParent() const
{
	return NULL;
}
BaseNode* RootNode::getBrother() const
{
	return NULL;
}
BaseNode* RootNode::getFirstChild() const
{
	return this->firstChild;
}
NodeType BranchNode::getNodeType() const
{
	return BRANCH_NODE;
}
BaseNode* BranchNode::getParent() const
{
	return this->parent;
}
BaseNode* BranchNode::getBrother() const
{
	return this->brother;
}
BaseNode* BranchNode::getFirstChild() const
{
	return this->firstChild;
}
int UBTieTree::randomForNoneMove() const
{
	BaseNode *nextNode=root->getFirstChild();
	int count=0;
	while(nextNode != NULL)
	{
		++count;
		nextNode=nextNode->getBrother();
	}
	if(count == 0)
		return 0;
	else
	{
		time_t nowTime=time(NULL);
		srand(nowTime);
		return (rand()%count + 1);
	}
}
UBTieTree::UBTieTree(const vector<ChessRecordStruct>& _record,int maxSearchDepth):root(NULL)
{
	buildTieTree(_record,maxSearchDepth);
}
UBTieTree::~UBTieTree()
{
	deleteNode(root);
}
void UBTieTree::buildTieTree(const vector<ChessRecordStruct>& _record,int maxSearchDepth)
{
	root=new RootNode;
	int searchDepth=0;
	for(int i=0;i<_record.size();i++)
	{
		ChessRecordStruct tempStruct=_record[i];
		bool isVictory=(tempStruct.getGameResult() == L"先胜");
		bool isTie=(tempStruct.getGameResult() == L"先和");
		searchDepth=((tempStruct.getGameTextMoveLength() > maxSearchDepth)?maxSearchDepth:tempStruct.getGameTextMoveLength());
		BaseNode *topNode=root;
		for(int j=0;j<searchDepth;j++)
		{
			wstring tempMoveString=tempStruct.getGameTextMove(j);
			bool hasHit=false;
			BaseNode *nextNode=topNode->getFirstChild();
			if(nextNode != NULL)
			{
				do
				{
					NodeData &nextData=nextNode->getData();
					if(nextData.moveString == tempMoveString)
					{
						hasHit=true;
						nextData.hitNumber+=1;
						int factor=((j%2==0)?1:-1);
						if(isVictory)
							nextData.victoryScore+=1*factor;
						else if(!isTie)
							nextData.victoryScore-=1*factor;
						break;
					}
					if(nextNode->getBrother() == NULL)
						break;
					else
						nextNode=nextNode->getBrother();
				}while(true);
			}
			if(!hasHit)
			{
				NodeData tempData;tempData.moveString=tempMoveString;
						  tempData.hitNumber=1;
						  int factor=((j%2==0)?1:-1);
						  if(isVictory)
							tempData.victoryScore=1*factor;
						  else if(!isTie)
							tempData.victoryScore=-1*factor;
				BaseNode *newNode=new BranchNode;
				newNode->setData(tempData);
				newNode->setParent(topNode);
				if(nextNode == NULL)
					topNode->setFirstChild(newNode);
				else
					nextNode->setBrother(newNode);
				nextNode=newNode;
			}
			topNode=nextNode;
		}
	}
	//设置搜索起点
	searchPos=root;
	firstForWin=true;
}
void UBTieTree::deleteNode(BaseNode *node)
{
	if(node == NULL)
		return;
	BaseNode *_firstChild=node->getFirstChild();
	if(_firstChild == NULL)
	{
		//wcout<<"delete........"<<node->getData().moveString<<endl;
		delete node;
	}
	else
	{
		BaseNode *nextChild=_firstChild->getBrother();
		deleteNode(_firstChild);
		while(nextChild != NULL)
		{
			BaseNode *forDelete=nextChild;
			nextChild=nextChild->getBrother();
			deleteNode(forDelete);
		}
		//wcout<<"delete branch..."<<node->getData().moveString<<endl;
		delete node;
	}
}
wstring UBTieTree::searchTieTree(const wstring& _moveString)
{
	if(_moveString == L"")
	{
		searchPos=root;
		BaseNode *bestNode=NULL;
		int       bestScore=-1000;
		BaseNode *nextNode=searchPos->getFirstChild();
		if(nextNode == NULL)
		{
			return wstring();
		}
		else
		{
			int limitCount=randomForNoneMove();
			//wcout<<"Limit:"<<limitCount<<endl;
			int count=1;
			do
			{
				NodeData& nextData=nextNode->getData();
				if(firstForWin && ((nextData.hitNumber+nextData.victoryScore) > bestScore))
				{
					bestScore=nextData.hitNumber+nextData.victoryScore;
					bestNode=nextNode;
				}
				if(!firstForWin)
				{
					bestNode=nextNode;
				}
				if(count == limitCount)
					break;
				++count;
				if(nextNode->getBrother() == NULL)
					break;
				else
					nextNode=nextNode->getBrother();
			}while(true);
			searchPos=bestNode;
			return bestNode->getData().moveString;
		}
	}
	else
	{
		BaseNode *nextNode=searchPos->getFirstChild();
		if(nextNode == NULL)
		{
			return wstring();
		}
		else
		{
			bool	  hasHit=false;
			do
			{
				NodeData& nextData=nextNode->getData();
				if(_moveString == nextData.moveString)
				{
					hasHit=true;
					break;
				}
				if(nextNode->getBrother() == NULL)
					break;
				else
					nextNode=nextNode->getBrother();
			}while(true);
			if(!hasHit)
				return wstring();
			//find enemy move
			BaseNode *bestNode=NULL;
			int bestScore=-1000;
			nextNode=nextNode->getFirstChild();
			if(nextNode == NULL)
				return wstring();
			else
			{
				while(true)
				{
					NodeData& nextData=nextNode->getData();
					if((nextData.hitNumber+nextData.victoryScore) > bestScore)
					{
						bestScore=nextData.hitNumber+nextData.victoryScore;
						bestNode=nextNode;
					}
					if(nextNode->getBrother() == NULL)
					{
						break;
					}
					else
					{
						nextNode=nextNode->getBrother();
					}
				}

				searchPos=bestNode;
				return bestNode->getData().moveString;
			}
		}
	}
	return wstring();
}
ChinessMove UBTieTree::searchTieTreeForMove(const ChinessMove& _move)
{
	char mbsVector[20];
	wchar_t wcsVector[20];
	UBString _moveString=_move.chessMan+_move.from+_move.act+_move.to;
	memset(wcsVector,0,20);
	mbstowcs(wcsVector,_moveString.getData(),20);

	wstring _resultString=searchTieTree(wcsVector);

	ChinessMove _resultMove;
	if(_resultString == L"")
		return _resultMove;
	memset(mbsVector,0,20);
	wcstombs(mbsVector,_resultString.substr(0,1).data(),20);
	_resultMove.chessMan=mbsVector;
	memset(mbsVector,0,20);
	wcstombs(mbsVector,_resultString.substr(1,1).data(),20);
	_resultMove.from=mbsVector;
	memset(mbsVector,0,20);
	wcstombs(mbsVector,_resultString.substr(2,1).data(),20);
	_resultMove.act=mbsVector;
	memset(mbsVector,0,20);
	wcstombs(mbsVector,_resultString.substr(3,1).data(),20);
	_resultMove.to=mbsVector;
	return _resultMove;
}
void UBTieTree::initSearchPos()
{
	this->searchPos=root;
}
void UBTieTree::setFirstForWin(bool _forWin)
{
	this->firstForWin=_forWin;
}
void UBTieTree::report() const
{
	BaseNode *nextNode=NULL;
	if(searchPos == root)
		nextNode=root->getFirstChild();
	else
		nextNode=searchPos->getParent()->getFirstChild();
	while(nextNode != NULL)
	{
		NodeData &nextData=nextNode->getData();
		wcout<<nextData.moveString<<" "<<nextData.hitNumber<<" "<<nextData.victoryScore<<endl;
		nextNode=nextNode->getBrother();
	}
	int count=1;
	nextNode=root;
	while(nextNode->getFirstChild() != NULL)
	{
		++count;
		nextNode=nextNode->getFirstChild();
	}
	wcout<<"depth:"<<count<<endl;
}
