/*
	注意这里有一个bug，就是在设置locale::global(Locale(""))之后，wcout与cout不可以混用，否则造成输出异常
	所以在report函数、以及searchTieTree函数中注意wcout的调用
	在cout输出中文时，屏蔽这两个函数
*/
#ifndef UBTIRTREE_H
#define UBTIETREE_H
#include <string>
#include <vector>
#include "Txt2Struct.h"
#include "../MoveMaker.h"
using namespace std;
enum NodeType
{
	ROOT_NODE,
	BRANCH_NODE,
};
struct NodeData
{
	wstring moveString;
	int	hitNumber;
	int	victoryScore;
	NodeData();
};
class BaseNode
{
protected:
	NodeData data;
	BaseNode *parent;
	BaseNode *brother;
	BaseNode *firstChild;
public:
	BaseNode();
	NodeData& getData();
	void setData(const NodeData& _data);
	virtual NodeType getNodeType() const=0;
	virtual BaseNode* getParent() const;
	virtual BaseNode* getBrother() const;
	virtual BaseNode* getFirstChild() const;
	void setParent(BaseNode *_parent);
	void setBrother(BaseNode *_brother);
	void setFirstChild(BaseNode *_child);
};
class RootNode:public BaseNode
{
public:
	NodeType getNodeType() const;
	BaseNode* getParent() const;
	BaseNode* getBrother() const;
	BaseNode* getFirstChild() const;
};
class BranchNode:public BaseNode
{
public:
	NodeType getNodeType() const;
	BaseNode* getParent() const;
	BaseNode* getBrother() const;
	BaseNode* getFirstChild() const;
};
class UBTieTree
{
private:
	RootNode *root;
	BaseNode *searchPos;
	bool	  firstForWin;
private:
	int randomForNoneMove() const;
public:
	UBTieTree(const vector<ChessRecordStruct>& _record,int maxSearchDepth=30);
	~UBTieTree();
	void buildTieTree(const vector<ChessRecordStruct>& _record,int maxSearchDepth=30);
	void deleteNode(BaseNode *node);
	wstring searchTieTree(const wstring& _moveString);
	ChinessMove searchTieTreeForMove(const ChinessMove& _chinessMove);
	void initSearchPos();
	void setFirstForWin(bool _forWin);
	void report() const;
};
#endif
