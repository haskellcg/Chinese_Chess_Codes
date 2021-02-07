#include "Txt2Struct.h"
#include <fstream>
using namespace std;
wchar_t Txt2Struct::convertNumberWChar(wchar_t wch) const
{
	wchar_t sourceChar[9]={L'１',L'２',L'３',L'４',L'５',L'６',L'７',L'８',L'９'};
	wchar_t destChar[9]={'1','2','3','4','5','6','7','8','9'};
	for(int i=0;i<9;i++)
	{
		if(wch == sourceChar[i])
			return destChar[i];
	}
	return wch;
}
void Txt2Struct::convertFunction(const string& utf_8_FileName)
{
	locale::global(locale(""));
	wifstream wfin;
	wfin.imbue(locale(""));
	wcout.imbue(locale(""));
	wfin.open(utf_8_FileName.c_str());
	wchar_t wch;
	if(wfin.get(wch))
	{
		//略过Boom以及第一行
		while(wfin.get(wch))
		{
			if(wch == '\n')
				break;
		}
		while(wfin.get(wch))
		{
			ChessRecordStruct tempStruct;
			wstring tempStr;
			//game sequence
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr == L"...")
				tempStr=tempSequence;
			else
				tempSequence=tempStr;
			tempStruct.setGameSequence(tempStr);
			//game name
			tempStr=L"";
			wfin.get(wch);
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr == L"...")
				tempStr=tempName;
			else
				tempName=tempStr;
			tempStruct.setGameName(tempStr);
			//game round in global
			tempStr=L"";
			wfin.get(wch);
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr == L"...")
				tempStr=tempRoundsGlobal;
			else
				tempRoundsGlobal=tempStr;
			tempStruct.setGameRoundsGlobal(tempStr);
			//game date
			tempStr=L"";
			wfin.get(wch);
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr == L"...")
				tempStr=tempDate;
			else
				tempDate=tempStr;
			tempStruct.setGameDate(tempStr);
			//game red team
			tempStr=L"";
			wfin.get(wch);
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr == L"...")
				tempStr=tempRedTeam;
			else
				tempRedTeam=tempStr;
			tempStruct.setGameRedTeam(tempStr);
			//game red name
			tempStr=L"";
			wfin.get(wch);
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr == L"...")
				tempStr=tempRedName;
			else
				tempRedName=tempStr;
			tempStruct.setGameRedName(tempStr);
			//game result
			tempStr=L"";
			wfin.get(wch);
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr == L"...")
				tempStr=tempResult;
			else
				tempResult=tempStr;
			tempStruct.setGameResult(tempStr);
			//black name
			tempStr=L"";
			wfin.get(wch);
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr == L"...")
				tempStr=tempBlackName;
			else
				tempBlackName=tempStr;
			tempStruct.setGameBlackName(tempStr);
			//black team
			tempStr=L"";
			wfin.get(wch);
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr == L"...")
				tempStr=tempBlackTeam;
			else
				tempBlackTeam=tempStr;
			tempStruct.setGameBlackTeam(tempStr);
			//step number
			tempStr=L"";
			wfin.get(wch);
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr == L"...")
				tempStr=tempStepNumber;
			else
				tempStepNumber=tempStr;
			tempStruct.setGameStepNumber(tempStr);
			//ECCO name
			tempStr=L"";
			wfin.get(wch);
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr==L"...")
				tempStr=tempECCOName;
			else
				tempECCOName=tempStr;
			tempStruct.setGameECCOName(tempStr);
			//source
			tempStr=L"";
			wfin.get(wch);
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr == L"...")
				tempStr=tempSource;
			else
				tempSource=tempStr;
			tempStruct.setGameSource(tempStr);
			//recorder
			tempStr=L"";
			wfin.get(wch);
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr == L"...")
				tempStr=tempRecorder;
			else
				tempRecorder=tempStr; 
			tempStruct.setGameRecorder(tempStr);
			//Look time
			tempStr=L"";
			wfin.get(wch);
			while(wch != '\t')
			{
				tempStr+=wch;
				wfin.get(wch);
			}
			if(tempStr == L"...")
				tempStr=tempLookTime;
			else
				tempLookTime=tempStr; 
			tempStruct.setGameLookTime(tempStr);
			//text move
			wfin.get(wch); 		//"
			while(true)
			{
				tempStr=L"";
				wfin.get(wch);
				while(wch != ' ')
					wfin.get(wch);
				for(int i=0;i<4;i++)
				{
					wfin.get(wch);
					wch=convertNumberWChar(wch);
					tempStr+=wch;
				}
				tempStruct.addGameTextMove(tempStr);
				wfin.get(wch);
				bool hasQuote=false;
				while(wch != '\n')
				{
					if(wch == '"')
					{
						hasQuote=true;
					}
					wfin.get(wch);
				}
				if(hasQuote)
					break;
			}
			convertResult.push_back(tempStruct);
		}
	}
	wfin.close();
}
vector<ChessRecordStruct>& Txt2Struct::getStructVector()
{
	return this->convertResult;
}
int Txt2Struct::getResultLength() const
{
	return this->convertResult.size();
}
ChessRecordStruct Txt2Struct::getStructAt(int _index)
{
	if(_index>=0 && _index<this->convertResult.size())
	{
		return convertResult[_index];
	}
	return ChessRecordStruct();
}
