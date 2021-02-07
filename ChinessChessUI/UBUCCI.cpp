#include "UBUCCI.h"
#include "UBException.h"
//Note:
//	所有的setMember函数并没有实现检测数据类型的合法性，只是简单的检测数据格式的合法性
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                全局变量以及函数，因为不同的结构都要用，但是如果在UBString中实现，则返回数组则是一个比较麻烦的选择 
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
UBString formatBuffer[BUFFER_LENGTH];
int      formatBufferLen=0;
bool isLegalSpecialChar(char ch)
{
	if(ch == '/' || ch == '-')
		return true;
	else
		return false;
}
bool isNumber(char ch)
{
	if(ch >= '0' && ch <= '9')
		return true;
	else
		return false;
}
bool isLetter(char ch)
{
	if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return true;
	else
		return false;
}
bool analyString(const UBString& _string)
{
	formatBufferLen=0;
	for(int i=0;i<BUFFER_LENGTH;i++)
		formatBuffer[i]="";
	char ch;
	for(int i=0;i<_string.length();i++)
	{
		if(formatBufferLen > (BUFFER_LENGTH-1))
			throw UBException(-1,"Out of Bound......");
		ch=_string.at(i);
		if(ch == ' ')
			++formatBufferLen;
		else if(isLetter(ch) || isNumber(ch) || isLegalSpecialChar(ch))
		{
			formatBuffer[formatBufferLen]+=ch;
		}
		else
		{
			throw UBException(-1,"非法字符");
		}
	}
	++formatBufferLen;
	return true;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 UCCI_CMD_STRUCT 
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
UCCI_CMD_STRUCT::UCCI_CMD_STRUCT():cmdName("ucci")
{}
bool UCCI_CMD_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen == 1) && (formatBuffer[0] == UBString("ucci")))
		return true;
	else
		throw UBException(-1,"illegal format fro ucci command");
	return false;
}
UBString UCCI_CMD_STRUCT::toString() const
{
	return cmdName;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 ID_REPLY_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
ID_REPLY_STRUCT::ID_REPLY_STRUCT():replyName("id"),replyType(9)
{}
bool ID_REPLY_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen > 2) && (formatBuffer[0] == UBString("id")))
	{
		replyType=formatBuffer[1];
		replyContent+=formatBuffer[2];
		for(int i=3;i<formatBufferLen;i++)
		{
			replyContent+=' ';
			replyContent+=formatBuffer[i];
		}
		return true;
	}
	else
		throw UBException(-1,"illegal format id reply");
	return false;
}
UBString ID_REPLY_STRUCT::toString() const
{
	return replyName+" "+replyType+" "+replyContent;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 OPTION_REPLY_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
OPTION_REPLY_STRUCT::OPTION_REPLY_STRUCT():replyName("option"),replyParaTypeCMDName("type"),varPairLen(0)
{}
bool OPTION_REPLY_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen >= 4) && (formatBuffer[0] == UBString("option")))
	{
		varPairLen=0;
		replyType=formatBuffer[1];
		replyParaTypeContent=formatBuffer[3];
		int index=4;
		while(index < formatBufferLen)
		{
			if(formatBuffer[index] == UBString("min"))
			{
				++index;
				if(index >= formatBufferLen)
					throw UBException(-1,"no min value");
				replyMinCMDName="min";
				replyMinValue=formatBuffer[index];
			}
			else if(formatBuffer[index] == UBString("max"))
			{
				++index;
				if(index >= formatBufferLen)
					throw UBException(-1,"No Max Value");
				replyMaxCMDName="max";
				replyMaxValue=formatBuffer[index];
			}
			else if(formatBuffer[index] == UBString("var"))
			{
				++index;
				if(index >= formatBufferLen)
					throw UBException(-1,"no Var Value");
				if(varPairLen >= OPTION_LENGTH)
					throw UBException(-1,"out of bound of option varPair");
				varPair[varPairLen].varCMDName="var";
				varPair[varPairLen].value=formatBuffer[index];
				++varPairLen;
			}
			else if(formatBuffer[index] == UBString("default"))
			{
				++index;
				if(index >= formatBufferLen)
					throw UBException(-1,"no default value");
				replyDefaultCMDName="default";
				replyDefaultValue=formatBuffer[index];
			}
			else
				throw UBException(-1,"illegal option for option...");
			++index;
		}
		return true;
	}
	else
		throw UBException(-1,"illegal option format...");
	return false;
}
UBString OPTION_REPLY_STRUCT::toString() const
{
	UBString result;
	result+=replyName;
	result+=' ';
	result+=replyType;	
	result+=' ';
	result+=replyParaTypeCMDName;
	result+=' ';
	result+=replyParaTypeContent;
	if(replyMinCMDName != "")
	{
		result+=' ';
		result+=replyMinCMDName;
		result+=' ';
		result+=replyMinValue;
	}
	if(replyMaxCMDName != "")
	{
		result+=' ';
		result+=replyMaxCMDName;
		result+=' ';
		result+=replyMaxValue;
	}
	for(int i=0;i<varPairLen;i++)
	{
		result+=' ';
		result+=varPair[i].varCMDName;
		result+=' ';
		result+=varPair[i].value;
	}
	if(replyDefaultCMDName != "")
	{
		result+=' ';
		result+=replyDefaultCMDName;
		result+=' ';
		result+=replyDefaultValue;
	}
	return result;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 UCCIOK_REPLY_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
UCCIOK_REPLY_STRUCT::UCCIOK_REPLY_STRUCT():replyName("ucciok")
{}
bool UCCIOK_REPLY_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen == 1) && (formatBuffer[0] == UBString("ucciok")))
		return true;
	else
		throw UBException(-1,"illegal ucciok format ...");
	return false;
}
UBString UCCIOK_REPLY_STRUCT::toString() const
{
	return replyName;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 ISREADY_CMD_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
ISREADY_CMD_STRUCT::ISREADY_CMD_STRUCT():cmdName("isready")
{}
bool ISREADY_CMD_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen == 1) && (formatBuffer[0] == UBString("isready")))
		return true;
	else
		throw UBException(-1,"illegal isready format");
	return false;
}
UBString ISREADY_CMD_STRUCT::toString() const
{
	return cmdName;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 READY_REPLY_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
READYOK_REPLY_STRUCT::READYOK_REPLY_STRUCT():replyName("readyok")
{}
bool READYOK_REPLY_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen == 1) && (formatBuffer[0] == UBString("readyok")))
		return true;
	else
		throw UBException(-1,"illegal readok format");
	return false;
}
UBString READYOK_REPLY_STRUCT::toString() const
{
	return replyName;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 SETOPTION_CMD_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
SETOPTION_CMD_STRUCT::SETOPTION_CMD_STRUCT():cmdName("setoption")
{}
bool SETOPTION_CMD_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen >= 2) && (formatBufferLen <= 3) && (formatBuffer[0] == UBString("setoption")))
	{
		cmdType=formatBuffer[1];
		if(formatBufferLen == 3)
		{
			cmdContent=formatBuffer[2];
		}
		return true;
	}
	else
		throw UBException(-1,"illegal setoption format ....");
	return false;
}
UBString SETOPTION_CMD_STRUCT::toString() const
{
	UBString result;
	result+=cmdName;
	result+=' ';
	result+=cmdType;
	if(cmdContent != "")
	{
		result+=' ';
		result+=cmdContent;
	}
	return result;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 POSITION_CMD_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
POSITION_CMD_STRUCT::POSITION_CMD_STRUCT():cmdName("position")
{}
bool POSITION_CMD_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen >= 2) && (formatBuffer[0] == UBString("position")))
	{
		movesForFenLen=0;
		int index=1;
		if(formatBuffer[1] == UBString("startpos"))
		{
			fenCMDName=formatBuffer[1];
			++index;
		}
		else if(formatBuffer[1] == UBString("fen"))
		{
			if(formatBufferLen < 8)
				throw UBException(-1,"illegal fen string ....");
			fenCMDName=formatBuffer[1];
			fenString+=formatBuffer[2];
			for(int i=3;i<=7;i++)
			{
				fenString+=' ';
				fenString+=formatBuffer[i];
			}
			index+=7;
		}
		else
			throw UBException(-1,"illegal position format");
		if(index < formatBufferLen)
		{
			if(formatBuffer[index] != UBString("moves"))
				throw UBException(-1,"should be moves there");
			movesCMDName=formatBuffer[index];
			++index;
			if(index >= formatBufferLen)
				throw UBException(-1,"there is moves cmd ,but no move");
			while(index < formatBufferLen)
			{
				if(movesForFenLen >= MOVES_LENGTH)
					throw UBException(-1,"moves Array has no room");
				movesForFen[movesForFenLen]=formatBuffer[index];
				++index;
				++movesForFenLen;
			}
		}
		return true;
	}
	else
		throw UBException(-1,"illegal format position");
	return false;
}
UBString POSITION_CMD_STRUCT::toString() const
{
	UBString result;
	result+=cmdName;
	result+=' ';
	result+=fenCMDName;
	if(fenString != "")
	{
		result+=' ';
		result+=fenString;
	}
	if(movesCMDName != "")
	{
		result+=' ';
		result+=movesCMDName;
	}
	for(int i=0;i<movesForFenLen;i++)
	{
		result+=' ';
		result+=movesForFen[i];
	}
	return result;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 BANMOVES_CMD_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
BANMOVES_CMD_STRUCT::BANMOVES_CMD_STRUCT():cmdName("banmoves")
{}
bool BANMOVES_CMD_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen >= 2) && (formatBuffer[0] == UBString("banmoves")))
	{
		banmovesLen=0;
		for(int i=1;i<formatBufferLen;i++)
		{
			if(banmovesLen >= MOVES_LENGTH)
				throw UBException(-1,"banmoves array out of bound....");
			banmoves[banmovesLen]=formatBuffer[i];
			++banmovesLen;
		}
	}
	else
		throw UBException(-1,"illegal banmoves format....");
	return false;
}
UBString BANMOVES_CMD_STRUCT::toString() const
{
	UBString result;
	result+=cmdName;
	for(int i=0;i<banmovesLen;i++)
	{
		result+=' ';
		result+=banmoves[i];
	}
	return result;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 GO_CMD_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
GO_CMD_STRUCT::GO_CMD_STRUCT():cmdName("go"),cmdModeIndex(0)
{}
bool GO_CMD_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen >= 3) && (formatBuffer[0] == UBString("go")))
	{
		int index=1;
		if((formatBuffer[index] == UBString("ponder")) || (formatBuffer[index] == UBString("draw")))
		{
			cmdType=formatBuffer[index];
			++index;
		}
		if(formatBuffer[index] == UBString("depth"))
		{
			++index;
			if(index != (formatBufferLen-1))
				throw UBException(-1,"illegal depth value....");
			cmdModeIndex=0;
			depthMode.modeName=formatBuffer[index-1];
			depthMode.modeDepth=formatBuffer[index];
		}
		else if(formatBuffer[index] == UBString("nodes"))
		{
			++index;
			if(index != (formatBufferLen-1))
				throw UBException(-1,"illegal nodes value");
			cmdModeIndex=1;
			nodesMode.modeName=formatBuffer[index-1];
			nodesMode.modeNodeNumber=formatBuffer[index];
		}
		else if(formatBuffer[index] == UBString("time"))
		{
			cmdModeIndex=2;
			timeMode.modeName=formatBuffer[index];
			++index;
			if(index >= formatBufferLen)
				throw UBException(-1,"illegal time value...");
			timeMode.restTime=formatBuffer[index];
			++index;
			while(index < formatBufferLen)
			{
				if(formatBuffer[index] == UBString("movestogo"))
				{
					timeMode.movesToGoCMDName=formatBuffer[index];
					++index;
					if(index >= formatBufferLen)
						throw UBException(-1,"illegal movestogo value ....");
					timeMode.restSteps=formatBuffer[index];
				}
				else if(formatBuffer[index] == UBString("increment"))
				{
					timeMode.incrementCMDName=formatBuffer[index];
					++index;
					if(index >= formatBufferLen)
						throw UBException(-1,"illegal increment value");
					timeMode.incrementForStep=formatBuffer[index];
				}
				else if(formatBuffer[index] == UBString("opptime"))
				{
					timeMode.oppTimeCMDName=formatBuffer[index];
					++index;
					if(index >= formatBufferLen)
						throw UBException(-1,"illegal opptime value..");
					timeMode.oppRestTime=formatBuffer[index];
				}
				else if(formatBuffer[index] == UBString("oppmovestogo"))
				{
					timeMode.oppMovesToGoCMDName=formatBuffer[index];
					++index;
					if(index >= formatBufferLen)
						throw UBException(-1,"illegal oppmovestogo value..");
					timeMode.oppRestSteps=formatBuffer[index];
				}
				else if(formatBuffer[index] == UBString("oppincrement"))
				{
					timeMode.oppIncrementCMDName=formatBuffer[index];
					++index;
					if(index >= formatBufferLen)
						throw UBException(-1,"illegal oppincrement value..");
					timeMode.oppIncrementForStep=formatBuffer[index];
				}
				else
					throw UBException(-1,"unknow go time parameter ....");
				++index;
			}
		}
		else
			throw UBException(-1,"unknow mode for go command ....");
		return true;
	}
	else
		throw UBException(-1,"illegal go command format");
	return false;
}
UBString GO_CMD_STRUCT::toString() const
{
	UBString result;
	result+=cmdName;
	if(cmdType != "")
	{
		result+=' ';
		result+=cmdType;
	}
	switch(cmdModeIndex)
	{
		case 0:
			result+=' ';
			result+=depthMode.modeName;
			result+=' ';
			result+=depthMode.modeDepth;
			break;
		case 1:
			result+=' ';
			result+=nodesMode.modeName;
			result+=' ';
			result+=nodesMode.modeNodeNumber;
			break;
		case 2:
			result+=' ';
			result+=timeMode.modeName;
			result+=' ';
			result+=timeMode.restTime;
			if(timeMode.movesToGoCMDName != "")
			{
				result+=' ';
				result+=timeMode.movesToGoCMDName;
				result+=' ';
				result+=timeMode.restSteps;
			}
			if(timeMode.incrementCMDName != "")
			{
				result+=' ';
				result+=timeMode.incrementCMDName;
				result+=' ';
				result+=timeMode.incrementForStep;
			}
			if(timeMode.oppTimeCMDName != "")
			{
				result+=' ';
				result+=timeMode.oppTimeCMDName;
				result+=' ';
				result+=timeMode.oppRestTime;
			}
			if(timeMode.oppMovesToGoCMDName != "")
			{
				result+=' ';
				result+=timeMode.oppMovesToGoCMDName;
				result+=' ';
				result+=timeMode.oppRestSteps;
			}
			if(timeMode.oppIncrementCMDName != "")
			{
				result+=' ';
				result+=timeMode.oppIncrementCMDName;
				result+=' ';
				result+=timeMode.oppIncrementForStep;
			}
			break;
		default:
			throw UBException(-1,"cmdModeIndex is illegal....");
	}
	return result;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 INFO_REPLY_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
INFO_REPLY_STRUCT::INFO_REPLY_STRUCT():replyName("info"),infoModeIndex(0)
{}
bool INFO_REPLY_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen >= 3) && (formatBuffer[0] == UBString("info")))
	{
		int index=1;
		if(formatBuffer[index] == UBString("time"))
		{
			if(formatBufferLen != 5)
				throw UBException(-1,"illegal info time option format..");
			infoModeIndex=0;
			infoTimeNode.timeCMDName=formatBuffer[index];
			++index;
			infoTimeNode.usedTime=formatBuffer[index];
			++index;
			if(formatBuffer[index] != UBString("nodes"))
				throw UBException(-1,"here should be node");
			infoTimeNode.nodesCMDName=formatBuffer[index];
			++index;
			infoTimeNode.searchedNodes=formatBuffer[index];
		}
		else if(formatBuffer[index] == UBString("depth"))
		{
			infoDepth.searchedMoveLen=0;
			infoModeIndex=1;
			infoDepth.depthCMDName=formatBuffer[index];
			++index;
			if(index >= formatBufferLen)
				throw UBException(-1,"info depth no value");
			infoDepth.searchedDepth=formatBuffer[index];
			++index;
			if(index < formatBufferLen)
			{
				if(formatBuffer[index] == UBString("score"))
				{
					++index;
					if(index >= formatBufferLen)
						throw UBException(-1,"info depth score no value");
					infoDepth.scoreCMDName=formatBuffer[index-1];
					infoDepth.scoreForNow=formatBuffer[index];
				}
				if(formatBuffer[index] == UBString("pv"))
				{
					++index;
					if(index >= formatBufferLen)
						throw UBException(-1,"info depth pv no value");
					infoDepth.pvCMDName=formatBuffer[index-1];
					while(index < formatBufferLen)
					{
						if(infoDepth.searchedMoveLen >= MOVES_LENGTH)
							throw UBException(-1,"searchedMove array overflow....");
						infoDepth.searchedMove[infoDepth.searchedMoveLen]=formatBuffer[index];
						++index;
						++infoDepth.searchedMoveLen;
					}
				}
			}
		}
		else if(formatBuffer[index] == UBString("currmove"))
		{
			++index;
			if(index != (formatBufferLen-1))
				throw UBException(-1,"info currmove no value");
			infoModeIndex=2;
			infoCurrentMove.currentCMDName=formatBuffer[index-1];
			infoCurrentMove.currentMove=formatBuffer[index];
		}
		else if(formatBuffer[index] == UBString("message"))
		{
			++index;
			if(index != (formatBufferLen-1))
				throw UBException(-1,"info message no value");
			infoModeIndex=3;
			infoMessage.messageCMDName=formatBuffer[index-1];
			infoMessage.messageContent=formatBuffer[index];
		}
		else
			throw UBException(-1,"unknow info type reply");
		return true;
	}
	else
		throw UBException(-1,"illegal info reply format...");
	return false;
}
UBString INFO_REPLY_STRUCT::toString() const
{
	UBString result;
	result+=replyName;
	switch(infoModeIndex)
	{
		case 0:
			result+=' ';
			result+=infoTimeNode.timeCMDName;
			result+=' ';
			result+=infoTimeNode.usedTime;
			result+=' ';
			result+=infoTimeNode.nodesCMDName;
			result+=' ';
			result+=infoTimeNode.searchedNodes;
			break;
		case 1:
			result+=' ';
			result+=infoDepth.depthCMDName;
			result+=' ';
			result+=infoDepth.searchedDepth;
			if(infoDepth.scoreCMDName != "")
			{
				result+=' ';
				result+=infoDepth.scoreCMDName;
				result+=' ';
				result+=infoDepth.scoreForNow;
			}
			if(infoDepth.pvCMDName != "")
			{
				result+=' ';
				result+=infoDepth.pvCMDName;
				for(int i=0;i<infoDepth.searchedMoveLen;i++)
				{
					result+=' ';
					result+=infoDepth.searchedMove[i];
				}
			}
			break;
		case 2:
			result+=' ';
			result+=infoCurrentMove.currentCMDName;
			result+=' ';
			result+=infoCurrentMove.currentMove;
			break;
		case 3:
			result+=' ';
			result+=infoMessage.messageCMDName;
			result+=' ';
			result+=infoMessage.messageContent;
			break;
		default:
			throw UBException(-1,"unknow infoModeIndex .....");
	}
	return result;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 PONDERHIT_CMD_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
PONDERHIT_CMD_STRUCT::PONDERHIT_CMD_STRUCT():cmdName("ponderhit")
{}
bool PONDERHIT_CMD_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen >= 1) && (formatBufferLen <= 2) && (formatBuffer[0] == UBString("ponderhit")))
	{
		if(formatBufferLen == 2)
		{
			if(formatBuffer[1] != UBString("draw"))
			{
				drawOption=formatBuffer[1];
			}
			else
				throw UBException(-1,"unknow parameter for ponderhit command.");
		}
		return true;
	}
	else
		throw UBException(-1,"illegal ponderhit cmd format..");
	return false;
}
UBString PONDERHIT_CMD_STRUCT::toString() const
{
	UBString result;
	result+=cmdName;
	if(drawOption != "")
	{
		result+=' ';
		result+=drawOption;
	}
	return result;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 STOP_CMD_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
STOP_CMD_STRUCT::STOP_CMD_STRUCT():cmdName("stop")
{}
bool STOP_CMD_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen == 1) && (formatBuffer[0] == UBString("stop")))
	{
		return true;
	}
	else
		throw UBException(-1,"stop command format not legal.");
	return false;
}
UBString STOP_CMD_STRUCT::toString() const
{
	return this->cmdName;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 BESTMOVE_REPLY_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
BESTMOVE_REPLY_STRUCT::BESTMOVE_REPLY_STRUCT():replyName("bestmove")
{}
bool BESTMOVE_REPLY_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen >= 2) && (formatBuffer[0] == UBString("bestmove")))
	{
		int index=1;
		bestMove=formatBuffer[index];
		++index;
		if(index < formatBufferLen)
		{
			if(formatBuffer[index] == UBString("ponder"))
			{
				++index;
				if(index >= formatBufferLen)
					throw UBException(-1,"bestmove ponder no value");
				ponderCMDName=formatBuffer[index-1];
				goodMove=formatBuffer[index];
				++index;
			}
		}
		if(index < formatBufferLen)
		{
			if((index == (formatBufferLen-1)) && 
			   (formatBuffer[index] == UBString("draw") || formatBuffer[index] == UBString("resign")))
			{
				drawOption=formatBuffer[index];
			}
			else
				throw UBException(-1,"here should be draw/resign");
		}
		return true;
	}
	else
		throw UBException(-1,"bestmove command format illegal");
	return false;
}
UBString BESTMOVE_REPLY_STRUCT::toString() const
{
	UBString result;
	result+=replyName;
	result+=' ';
	result+=bestMove;
	if(ponderCMDName != "")
	{
		result+=' ';
		result+=ponderCMDName;
		result+=' ';
		result+=goodMove;
	}
	if(drawOption != "")
	{
		result+=' ';
		result+=drawOption;
	}
	return result;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 NOBESTMOVE_REPLY_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
NOBESTMOVE_REPLY_STRUCT::NOBESTMOVE_REPLY_STRUCT():replyName("nobestmove")
{}
bool NOBESTMOVE_REPLY_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen == 1) && (formatBuffer[0] == UBString("nobestmove")))
	{
		return true;
	}
	else
		throw UBException(-1,"illegal nobestmove reply format..");
	return false;
}
UBString NOBESTMOVE_REPLY_STRUCT::toString() const
{
	return replyName;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 PROBE_CMD_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
PROBE_CMD_STRUCT::PROBE_CMD_STRUCT():cmdName("probe")
{}
bool PROBE_CMD_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen >= 2) && (formatBuffer[0] == UBString("probe")))
	{
		probeMovesLen=0;
		int index=1;
		if(formatBuffer[index] == UBString("startpos"))
		{
			fenCMDName=formatBuffer[index];
			++index;
		}
		else if(formatBuffer[index] == UBString("fen"))
		{
			if(formatBufferLen < 8)
				throw UBException(-1,"illegal format fenString");
			fenCMDName=formatBuffer[index];
			++index;
			fenString+=formatBuffer[2];
			for(int i=3;i<8;i++)
			{
				fenString+=' ';
				fenString+=formatBuffer[i];
			}
			index+=6;
		}
		else
			throw UBException(-1,"unknow probe parameter..");
		if(index < formatBufferLen)
		{
			if(formatBuffer[index] != UBString("moves"))
				throw UBException(-1,"here should be moves cmd");
			movesCMDName=formatBuffer[index];
			++index;
			if(index >= formatBufferLen)
				throw UBException(-1,"moves cmd No value");
			while(index < formatBufferLen)
			{
				if(probeMovesLen >= MOVES_LENGTH)
					throw UBException(-1,"probemoves array has overflow..");
				probeMoves[probeMovesLen]=formatBuffer[index];
				++probeMovesLen;
				++index;
			}
		}
		return true;
	}
	else
		throw UBException(-1,"illegal probe command format..");
	return false;
}
UBString PROBE_CMD_STRUCT::toString() const
{
	UBString result;
	result+=cmdName;
	result+=' ';
	result+=fenCMDName;
	if(fenString != "")
	{
		result+=' ';
		result+=fenString;
	}
	if(movesCMDName != "")
	{
		result+=' ';
		result+=movesCMDName;
		for(int i=0;i<probeMovesLen;i++)
		{
			result+=' ';
			result+=probeMoves[i];
		}
	}
	return result;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 POPHASH_REPLY_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
POPHASH_REPLY_STRUCT::POPHASH_REPLY_STRUCT():replyName("pophash")
{}
bool POPHASH_REPLY_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen >= 1) && (formatBuffer[0] == UBString("pophash")))
	{
		int index=1;
		while(index < formatBufferLen)
		{
			if(formatBuffer[index] == UBString("bestmove"))
			{
				bestMoveCMDName=formatBuffer[index];
				++index;
				if(index >= formatBufferLen)
					throw UBException(-1,"pophash bestmove no value");
				bestMove=formatBuffer[index];
			}
			else if(formatBuffer[index] == UBString("lowerbound"))
			{
				lowerBoundCMDName=formatBuffer[index];
				++index;
				if(index >= formatBufferLen)
					throw UBException(-1,"pophash lowerbound no value");
				betaValue=formatBuffer[index];
				++index;
				if((index >= formatBufferLen) || (formatBuffer[index] != UBString("depth")))
					throw UBException(-1,"here should be depth");
				depthForLowCMDName=formatBuffer[index];
				++index;
				if(index >= formatBufferLen)
					throw UBException(-1,"pophash lowerbound depth no value");
				depthForLow=formatBuffer[index];
			}
			else if(formatBuffer[index] == UBString("upperbound"))
			{
				upperBoundCMDName=formatBuffer[index];
				++index;
				if(index >= formatBufferLen)
					throw UBException(-1,"pophash upperbound no value");
				alphaValue=formatBuffer[index];
				++index;
				if((index >= formatBufferLen) || (formatBuffer[index] != UBString("depth")))
					throw UBException(-1,"here should be depth");
				depthForUpCMDName=formatBuffer[index];
				++index;
				if(index >= formatBufferLen)
					throw UBException(-1,"pophash lowerbound depth no value");
				depthForUp=formatBuffer[index];
			}
			++index;
		}
		return true;
	}
	else
		throw UBException(-1,"pophash reply illegal format..");
	return false;
}
UBString POPHASH_REPLY_STRUCT::toString() const
{
	UBString result;
	result+=replyName;
	if(bestMoveCMDName != "")
	{
		result+=' ';
		result+=bestMoveCMDName;
		result+=' ';
		result+=bestMove;
	}
	if(lowerBoundCMDName != "")
	{
		result+=' ';
		result+=lowerBoundCMDName;
		result+=' ';
		result+=betaValue;
		result+=' ';
		result+=depthForLowCMDName;
		result+=' ';
		result+=depthForLow;
	}
	if(upperBoundCMDName != "")
	{
		result+=' ';
		result+=upperBoundCMDName;
		result+=' ';
		result+=alphaValue;
		result+=' ';
		result+=depthForUpCMDName;
		result+=' ';
		result+=depthForUp;
	}
	return result;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 QUIT_CMD_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
QUIT_CMD_STRUCT::QUIT_CMD_STRUCT():cmdName("quit")
{}
bool QUIT_CMD_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen == 1) && (formatBuffer[0] == UBString("quit")))
	{
		return true;
	}
	else
		throw UBException(-1,"illegal quit command format");
	return false;
}
UBString QUIT_CMD_STRUCT::toString() const
{
	return this->cmdName;
}
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
////                 BYE_REPLY_STRUCT
////---------------------------------------------------------------------------
////---------------------------------------------------------------------------
BYE_REPLY_STRUCT::BYE_REPLY_STRUCT():replyName("bye")
{}
bool BYE_REPLY_STRUCT::setMember(const UBString& _ucciCMD)
{
	analyString(_ucciCMD);
	if((formatBufferLen == 1) && (formatBuffer[0] == UBString("bye")))
	{
		return true;
	}
	else
		throw UBException(-1,"illegal bye reply format.");
	return false;
}
UBString BYE_REPLY_STRUCT::toString() const
{
	return this->replyName;
}
