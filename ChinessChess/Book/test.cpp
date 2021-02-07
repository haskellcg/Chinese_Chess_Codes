#include "Txt2Struct.h"
#include "UBTieTree.h"
#include <iostream>
#include <string>
#include <ctime>
using namespace std;
int main()
{
	Txt2Struct convertObj;
	string baseStr="./MyStdio/Book_";
	string extStr=".txt";
	time_t time_ub;
	time_ub=time(NULL);
	cout<<"载入开局库:"<<asctime(localtime(&time_ub));
	for(int i=1;i<=30;i++)
	{
		string fileName=baseStr+UBString::toString(i).getData()+extStr;
		convertObj.convertFunction(fileName);
	}
	time_ub=time(NULL);
	cout<<"载入结束时间:"<<asctime(localtime(&time_ub));
	UBTieTree tieTree(convertObj.getStructVector());
	//tieTree.report();
	tieTree.setFirstForWin(false);
	//wcout<<tieTree.searchTieTree(L"")<<endl;
	ChinessMove _move;
	_move.chessMan="兵";
	_move.from="九";
	_move.act="进";
	_move.to="一";
	time_ub=time(NULL);
	cout<<"开始时间"<<asctime(localtime(&time_ub));
	_move=tieTree.searchTieTreeForMove(_move);
	cout<<"兵九进一  应着："<<_move.chessMan<<_move.from<<_move.act<<_move.to<<endl;
	time_ub=time(NULL);
	cout<<"结束时间"<<asctime(localtime(&time_ub));
	//tieTree.report();
	_move.chessMan="马";
	_move.from="八";
	_move.act="进";
	_move.to="九";
	time_ub=time(NULL);
	cout<<"开始时间"<<asctime(localtime(&time_ub));
	_move=tieTree.searchTieTreeForMove(_move);
	cout<<"马八进九  应着："<<_move.chessMan<<_move.from<<_move.act<<_move.to<<endl;
	time_ub=time(NULL);
	cout<<"结束时间"<<asctime(localtime(&time_ub));
	//tieTree.report();
	//wcout<<tieTree.searchTieTree(L"象3进5")<<endl;
	return 0;
}
