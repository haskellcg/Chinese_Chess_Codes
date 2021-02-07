#include "CChessStruct.h"
#include "ChessManMove.h"
#include "ChessSearch.h"
#include "EvaluateSituation.h"
#include "UBStack.h"
#include "UBRandom.h"
#include "UBHashTable.h"
#include "MoveMaker.h"
#include "UBUCCI.h"
#include "UCCIEngine.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdio>
using std::cout;
using std::endl;
using std::setw;
int main()
{
/*
	{
		try{
		UCCIEngine engine;
		engine.handleIOFunction();
*/
/*
		CChessStruct* obj=new CChessStruct;
		SmartPtr<CChessStruct> ptr_obj=makeSmartPtr(obj);
		ChinessMove testL;
		testL.chessMan="炮";
		testL.from="二";
		testL.act="进";
		testL.to="七";
		Move _move=MoveMaker::translateToMove(ptr_obj,testL);
		MoveMaker moveMaker(ptr_obj);
		moveMaker.makeMove(_move);
		moveMaker.unMakeMove();
		moveMaker.makeMove(_move);
		moveMaker.unMakeMove();
		moveMaker.makeMove(_move);
		moveMaker.unMakeMove();
		moveMaker.makeMove(_move);
		moveMaker.unMakeMove();
		ptr_obj->report();
*/
/*
		}catch(UBException &e)
		{
			cout<<"catch UBException:"<<e.getErrorMsg()<<endl;
		}
	}
*/
/*
	CChessStruct* obj=new CChessStruct("4k4/3P1P3/3P5/3P1P3/9/9/9/9/9/4K4 w - - 0 0");
	SmartPtr<CChessStruct> ptr_obj=makeSmartPtr(obj);
	obj->report();
	Move temp;
	temp.from=86; temp.to=85;temp.capture=0;
	ChinessMove _chinessMove=MoveMaker::translateToChinessZX(ptr_obj,temp);
	WXFMove	_wxfMove=MoveMaker::translateToWXF(ptr_obj,temp);
	cout<<_chinessMove.chessMan<<_chinessMove.from<<_chinessMove.act<<_chinessMove.to<<endl;
	cout<<_wxfMove.chessMan<<_wxfMove.source<<_wxfMove.signal<<_wxfMove.dest<<endl;
	temp=MoveMaker::translateToMove(ptr_obj,_wxfMove);
	cout<<temp.from<<" "<<temp.to<<" "<<temp.capture<<" "<<endl;
*/
/*
	UBHashTable<int,int> hashTB(10,&hashInt,&isSameInt);
	UBHashNode<int,int> temp;
	temp.nodeKey=1;temp.nodeValue=12345;
	cout<<hashTB.insertNode(temp)<<endl;
	temp.nodeKey=11;temp.nodeValue=1223232;
	cout<<hashTB.insertNode(temp)<<endl;
	cout<<hashTB.capacity()<<endl;
	cout<<hashTB.activeDataLength()<<endl;
	cout<<hashTB.unActiveDataLength()<<endl;
	cout<<hashTB.deletedDataLength()<<endl;
*/
/*
	UBRandom random((uint32_ub)1000000);
	for(int i=0;i<100;i++)
	{
		cout<<setw(15)<<random.randomLinearBranch()<<"  ";
		if(i%6==0)
			cout<<endl;
	}
*/
	const char* arrOpen="r1bakabr1/9/1cn3n1c/p1p1p1R1p/6p2/2P6/P3P1P1P/1C2C1N2/9/RNBAKAB2 b - - 0 1";
	const char* arrMiddle="2bakab2/9/1c2c1n2/p5p1p/2P4R1/1C2p1P2/P3Pr2P/2N1BC3/4A4/3K1AB2 b - - 0 1";
	const char* arrEnd="9/2P6/3k5/4c4/2b6/9/4N4/8B/3n1p3/3K5 b - - 0 1";
	CChessStruct* objOpen=new CChessStruct(arrOpen);
	CChessStruct* objMiddle=new CChessStruct(arrMiddle);
	CChessStruct* objEnd=new CChessStruct(arrEnd);
	SmartPtr<CChessStruct> ptr_obj=makeSmartPtr(objOpen);
	ptr_obj->report();
	EvaluateSituation eva(ptr_obj);
	eva.setKnowledgeLevel(moreKnowledgeLevel);
	eva.setSubjective(otherPessimisticSubjective);
	cout<<"open + Eva:"<<eva.evaluate()<<endl;
	eva.setSubjective(normalSubjective);
	cout<<"open 0 Eva:"<<eva.evaluate()<<endl;
	eva.setSubjective(otherOptimisticSubjective);
	cout<<"open - Eva:"<<eva.evaluate()<<endl;

	ptr_obj=makeSmartPtr(objMiddle);
	ptr_obj->report();
	EvaluateSituation eva2(ptr_obj);
	eva2.setKnowledgeLevel(moreKnowledgeLevel);
	eva2.setSubjective(otherPessimisticSubjective);
	cout<<"Middle + Eva:"<<eva2.evaluate()<<endl;
	eva2.setSubjective(normalSubjective);
	cout<<"Middle 0 Eva:"<<eva2.evaluate()<<endl;
	eva2.setSubjective(otherOptimisticSubjective);
	cout<<"Middle - Eva:"<<eva2.evaluate()<<endl;

	ptr_obj=makeSmartPtr(objEnd);
	ptr_obj->report();
	EvaluateSituation eva3(ptr_obj);
	eva3.setKnowledgeLevel(moreKnowledgeLevel);
	eva3.setSubjective(otherPessimisticSubjective);
	cout<<"End + Eva:"<<eva3.evaluate()<<endl;
	eva3.setSubjective(normalSubjective);
	cout<<"End 0 Eva:"<<eva3.evaluate()<<endl;
	eva3.setSubjective(otherOptimisticSubjective);
	cout<<"End - Eva:"<<eva3.evaluate()<<endl;
/*
	SmartPtr<char> ptr1=obj->toFENStr();
	cout<<ptr1.getPtr()<<endl;
	CChessStruct* temp=NULL;
	ChessManMove mobj(makeSmartPtr(temp));
	mobj.bindStruct(ptr_obj);
	Move tempMove;tempMove.from=90;tempMove.to=86;tempMove.capture=0;
	mobj.setBestMove(tempMove);
	tempMove.from=103;tempMove.to=151;tempMove.capture=0;
	mobj.setKillerMove(tempMove);
	tempMove.from=90;tempMove.to=86;tempMove.capture=0;
	mobj.setKillerMove(tempMove);
	tempMove.from=90;tempMove.to=83;tempMove.capture=0;
	mobj.setKillerMove(tempMove);
	tempMove.from=103;tempMove.to=167;tempMove.capture=0;
	mobj.setKillerMove(tempMove);
	tempMove.from=101;tempMove.to=117;tempMove.capture=0;
	mobj.setKillerMove(tempMove);
	tempMove.from=90;tempMove.to=138;tempMove.capture=0;
	mobj.setKillerMove(tempMove);
	mobj.genAllMove();
	mobj.reportMove();
	cout<<"将军:"<<mobj.check()<<endl;
	EvaluateSituation eva(ptr_obj);
	eva.setKnowledgeLevel(moreKnowledgeLevel);
	cout<<"Eva:"<<eva.evaluate()<<endl;
*/
/*
	UBStack<int> stack;
	stack.push(1);
	stack.push(11);
	stack.push(111);
	stack.push(1111);
	cout<<stack.numberOfElement()<<endl;
	cout<<stack.pop()<<endl;
	cout<<stack.pop()<<endl;
	cout<<stack.pop()<<endl;
	cout<<stack.pop()<<endl;
	try
	{
		stack.pop();
	}
	catch(UBException& e)
	{
		cout<<e.getErrorCode()<<endl;
		cout<<e.getErrorMsg()<<endl;
	}
*/
/*
	ChessSearch alphaSearch(ptr_obj);
	ChessManMove mobj(ptr_obj);
	mobj.genAllMove();
	mobj.reportMove();
	//alphaSearch.setSearchMode(TimeSearch);
	//alphaSearch.setBookMode(false);
	time_t start_t;
	time_t end_t;
	tm *tmStruct;
	start_t=time(NULL);
	alphaSearch.alphaBetaSearch();
	alphaSearch.makeMoveForSearch(alphaSearch.getBestMove());
	Move blackMove;
	blackMove.from=107;blackMove.to=123;
	alphaSearch.makeMoveForSearch(blackMove);
	ptr_obj->report();
	alphaSearch.alphaBetaSearch();
	end_t=time(NULL);
	tmStruct=localtime(&start_t);
	cout<<"Start_Time:"<<tmStruct->tm_hour<<":"<<tmStruct->tm_min<<":"<<tmStruct->tm_sec<<endl;
	cout<<"StartTime:"<<asctime(tmStruct)<<endl;
	tmStruct=localtime(&end_t);
	cout<<"End_Time:"<<tmStruct->tm_hour<<":"<<tmStruct->tm_min<<":"<<tmStruct->tm_sec<<endl;
	cout<<"EndTime:"<<asctime(tmStruct)<<endl;
	cout<<"RESULT:"<<endl;
	cout<<"Final Value:"<<alphaSearch.getFinalValue()<<endl;
	cout<<"Best_Move:"<<alphaSearch.getBestMove().from<<"->"<<alphaSearch.getBestMove().to<<":"<<alphaSearch.getBestMove().capture<<endl;
	cout<<"总共搜索节点："<<alphaSearch.getTotalNodes()<<endl;
	ptr_obj->report();
*/
	/*
	cout<<"Final Value:"<<alphaSearch.getFinalValue()<<endl;
	Move _bestMove=alphaSearch.getBestMove();
	cout<<"Best Move:"<<_bestMove.from<<" "<<_bestMove.to<<" "<<_bestMove.capture<<endl;
	*/
/*
	OPTION_REPLY_STRUCT obj;
	obj.setMember("option usermili type int min 100 max 1000 var 1 var 2 var 4 var 6 var 9 default 1000");
	cout<<obj.toString()<<endl;
*/
	return 0;
}
