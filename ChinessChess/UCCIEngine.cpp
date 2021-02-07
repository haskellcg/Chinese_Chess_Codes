#include "UCCIEngine.h"
#include <cstring>
//for memset
#include <iostream>
using std::cout;
using std::endl;
UCCIEngineCMD UCCIEngine::parseStrToEnumCMD(const UBString& _cmdHeaderString) const
{
	int nowSize=8;
	UBString headerString[]={"ucci","id","ucciok","go","bestmove","quit","bye","position"};
	UCCIEngineCMD enumCMD[]={UCCI_CMD,ID_REPLY,UCCIOK_REPLY,GO_CMD,BESTMOVE_REPLY,QUIT_CMD,BYE_REPLY,POSITION_CMD};
	bool found=false;
	for(int i=0;i<nowSize;i++)
	{
		if(_cmdHeaderString == headerString[i])
			return enumCMD[i];
	}
	return UNKNOW_CMD;
}
void UCCIEngine::initSearchEngine()
{
	CChessStruct *startStruct=new CChessStruct;
	SmartPtr<CChessStruct> cChess=makeSmartPtr(startStruct);
	searchEngine=new ChessSearch(cChess);
}
void UCCIEngine::setSearchModeByStruct()
{
	if(searchEngine!=NULL)
	{
		SmartPtr<CChessStruct> chessPtr=searchEngine->cChess;
		int count=0;
		for(int i=0;i<32;i++)
		{
			if(chessPtr->cChessManPosition[i] != 0)
				++count;
		}
		if(count>=24)
		{
			if(searchEngine->alphaSearchMode == TimeSearch)
				return;
			cout<<"$>> set searchMode to : TimeSearch"<<endl;
			searchEngine->setSearchMode(TimeSearch);
		}
		else
		{
			if(searchEngine->alphaSearchMode == DepthSearch)
				return;
			cout<<"$>> set searchMode to : DepthSearch"<<endl;
			searchEngine->setSearchMode(DepthSearch);
		}
	}
}
UCCIEngine::UCCIEngine():state(BOOT_STATE),fileName(FIFONAME),searchEngine(NULL),lastMoveLen(0)
{
	if(access(fileName.getData(),F_OK) == -1)
	{
		mkfifo(fileName.getData(),0777);
	}
}
UCCIEngine::~UCCIEngine()
{
	if(searchEngine!=NULL)
		delete searchEngine;
	unlink(fileName.getData());
}
void UCCIEngine::handleIOFunction()
{
	int fifoHandle;

	char buffer[BUFFERSIZE];
	int bufferSize=BUFFERSIZE;
	int ret=0;
	fifoHandle=open(fileName.getData(),O_RDONLY);
	memset(buffer,0,bufferSize);
	ret=read(fifoHandle,buffer,bufferSize);
	close(fifoHandle);
	analyString(UBString(buffer));
	bool hasQuitSignal=false;
	while(true)
	{
		if(formatBufferLen != 0)
		{
			UCCIEngineCMD parseRet=parseStrToEnumCMD(formatBuffer[0]);
			switch(parseRet)
			{
				case UCCI_CMD:
						{
							if(state != BOOT_STATE)
							{
								throw UBException(-1,"Can't call ucci cmd in not BOOT_STATE");
								break;
							}
							ID_REPLY_STRUCT id_reply;
							id_reply.replyType="name";id_reply.replyContent="H.264";
							cout<<id_reply.toString()<<endl;
							id_reply.replyType="author";id_reply.replyContent="Mr.WDGB";
							cout<<id_reply.toString()<<endl;
							id_reply.replyType="copyright";id_reply.replyContent="www.xjtu.edu.cn";
							cout<<id_reply.toString()<<endl;
							id_reply.replyType="user";id_reply.replyContent="Mr.WDGB";
							cout<<id_reply.toString()<<endl;
							cout<<"$>> start initializing search Engine................."<<endl;
							initSearchEngine();
							cout<<"$>> initializing search Engine success !"<<endl;
							cout<<"$>> replying ucciok.........."<<endl;
							UCCIOK_REPLY_STRUCT ucciok_reply;
							fifoHandle=open(fileName.getData(),O_WRONLY);
							ret=write(fifoHandle,ucciok_reply.toString().getData(),ucciok_reply.toString().length());
							close(fifoHandle);
							cout<<"$>> reply ucciok end !"<<endl;
							state=FREE_STATE;
							cout<<"$>> state FREE_STATE !"<<endl;
						}
						break;
				case GO_CMD:
						{
							if(state != FREE_STATE)
							{
								throw UBException(-1,"Can't call go cmd in not FREE_STATE");
								break;
							}
							state=THINK_STATE;
							cout<<"$>> state THINK_STATE "<<endl;
							cout<<"$>> start thinking search for bestmove"<<endl;
							time_t startTime;
							time_t endTime;
							startTime=time(NULL);
							cout<<"$>> start time:"<<asctime(localtime(&startTime));
							//设置搜索策略
							this->setSearchModeByStruct();
							searchEngine->alphaBetaSearch();
							endTime=time(NULL);
							cout<<"$>> end thinking search !"<<endl;
							cout<<"$>> end time:"<<asctime(localtime(&endTime));
							ICCSMove _result;
							_result=MoveMaker::translateToICCS(searchEngine->getBestMove());
							BESTMOVE_REPLY_STRUCT bestmove_reply;
							bestmove_reply.bestMove+=_result.fromX;
			 				bestmove_reply.bestMove+=_result.fromY;
							bestmove_reply.bestMove+=_result.toX;
							bestmove_reply.bestMove+=_result.toY;
							cout<<"$>> reply bestmove "<<bestmove_reply.bestMove<<"......."<<endl;
							fifoHandle=open(fileName.getData(),O_WRONLY);
							ret=write(fifoHandle,bestmove_reply.toString().getData(),bestmove_reply.toString().length());
							close(fifoHandle);
							cout<<"$>> reply end !"<<endl;
							state=FREE_STATE;
							cout<<"$>> state FREE_STATE "<<endl;
							searchEngine->moveMaker.makeMove(searchEngine->getBestMove());
							if(searchEngine->getBestMove().capture !=0)
								lastMoveLen=0;
							else
								lastMoveLen+=1;
							cout<<"now FEN:"<<UBString(this->searchEngine->cChess->toFENStr().getPtr())<<endl;
						}
						break;
				case QUIT_CMD:
						{
							if(state != FREE_STATE)
							{
								throw UBException(-1,"Can't call quit cmd in not FREE_STATE");
								break;
							}
							cout<<"$>> reply bye ......."<<endl;
							BYE_REPLY_STRUCT bye_reply;
							fifoHandle=open(fileName.getData(),O_WRONLY);
							ret=write(fifoHandle,bye_reply.toString().getData(),bye_reply.toString().length());
							close(fifoHandle);
							cout<<"$>> reply bye end !"<<endl;
							hasQuitSignal=true;
							cout<<"$>> set QuitSignal ...."<<endl;
						}
						break;
				case POSITION_CMD:
						{
							if(state != FREE_STATE)
							{
								throw UBException(-1,"Can't call position cmd in not FREE_STATE");
								break;
							}
							POSITION_CMD_STRUCT position_cmd;
							position_cmd.setMember(UBString(buffer));
							UBString _fenString;
							if(position_cmd.fenCMDName == "fen")
								_fenString=position_cmd.fenString;
							else if(position_cmd.fenCMDName == "startpos")
							{
								_fenString=UBString(CChessStruct().toFENStr().getPtr());
							}
							//出现新局面，则只设置局面
							if((lastFenString == ""))
							{
								//该情况是刚载入引擎，设置局面
								//考虑设置开局、残局的情况
								cout<<"$>> initializing start struct ...."<<endl;
								lastFenString=_fenString;
								if(_fenString != UBString(searchEngine->cChess->toFENStr().getPtr()))
								{
									cout<<"$>> bind new CChessStruct ..."<<endl;
									CChessStruct *newChess=new CChessStruct(_fenString.getData());
									SmartPtr<CChessStruct> chessPtr=makeSmartPtr(newChess);
									searchEngine->bindStruct(chessPtr);
								}
								lastMoveLen=0;
							}
							else 
							{
								if(lastFenString != _fenString)
								{
									cout<<"For Debug ..........................................."<<endl;
									searchEngine->cChess->report();
									cout<<"lastFenString:"<<lastFenString<<endl;
									cout<<"_fenString:"<<_fenString<<endl;
									cout<<"now FEN:"<<UBString(this->searchEngine->cChess->toFENStr().getPtr())<<endl;
									cout<<"For Debug ..........................................."<<endl;
									//该情况必然走了几回合，设置局面
									//考虑吃子情况、以及设置残局
									if(_fenString == UBString(this->searchEngine->cChess->toFENStr().getPtr()) && (position_cmd.movesForFenLen!=0))
									{
										//这里是吃子后的不同，所以只要做出移动即可
										lastFenString=_fenString;
										UBString _iccsMove=position_cmd.movesForFen[0];
										ICCSMove _iccs;	_iccs.fromX=_iccsMove.at(0);
												_iccs.fromY=_iccsMove.at(1);
												_iccs.toX=_iccsMove.at(2);
												_iccs.toY=_iccsMove.at(3);	
										Move _move;
										_move=MoveMaker::translateToMove(this->searchEngine->cChess,_iccs);
										cout<<"$>> makemove "<<_move.from<<"->"<<_move.to<<":"<<_move.capture<<endl;
										this->searchEngine->moveMaker.makeMove(_move);
										lastMoveLen=1;
									}
									else
									{
										//这里是要设置局面，可能与现在局面相同，但是考虑到搜索树以及其他已经存在的变量应该初始化，所以调用bind
										cout<<"$>> reset start struct in half...."<<endl;
										lastFenString=_fenString;
										CChessStruct *newStruct=new CChessStruct(_fenString.getData());
										SmartPtr<CChessStruct> newChess=makeSmartPtr(newStruct);
										searchEngine->bindStruct(newChess);
										cout<<"$>> rebind complete !..."<<endl;
										lastMoveLen=0;
									}
								}
								else
								{
									//局面相等，但是也可能是设置局面
									if(position_cmd.movesForFenLen==0)
									{
										//该情况为设置局面
										cout<<"$>> reset start struct in no capture...."<<endl;
										CChessStruct *newStruct=new CChessStruct(_fenString.getData());
										SmartPtr<CChessStruct> newChess=makeSmartPtr(newStruct);
										searchEngine->bindStruct(newChess);
										cout<<"$>> rebind complete !..."<<endl;
										lastMoveLen=0;
									}
									else
									{
										//该情况就是正常的未吃子，设置走法的字串
										if(position_cmd.movesForFenLen==(lastMoveLen+1))
										{
											UBString _iccsMove=position_cmd.movesForFen[lastMoveLen];
											ICCSMove _iccs;	_iccs.fromX=_iccsMove.at(0);
													_iccs.fromY=_iccsMove.at(1);
													_iccs.toX=_iccsMove.at(2);
													_iccs.toY=_iccsMove.at(3);	
											Move _move;
											_move=MoveMaker::translateToMove(this->searchEngine->cChess,_iccs);
											cout<<"$>> makemove "<<_move.from<<"->"<<_move.to<<":"<<_move.capture<<endl;
											this->searchEngine->moveMaker.makeMove(_move);
											lastMoveLen+=1;
										}
										else
										{
											throw UBException(-1,"more or less move added...");
										}
									}
								}
							}
							//为了防止连续写管道的字串对挤到一块，每个case必须回写一个字串，让对方等待
							UBString noneStr="Just For Wait";
							fifoHandle=open(fileName.getData(),O_WRONLY);
							ret=write(fifoHandle,noneStr.getData(),noneStr.length());
							close(fifoHandle);
						}
						break;
				case UNKNOW_CMD:
						{
						}
						break;
				default:
						{
						}
						break;
			}
		}
		if(hasQuitSignal)
			break;
	
		fifoHandle=open(fileName.getData(),O_RDONLY);
		memset(buffer,0,bufferSize);
		ret=read(fifoHandle,buffer,bufferSize);
		close(fifoHandle);
		analyString(UBString(buffer));
	}
}
