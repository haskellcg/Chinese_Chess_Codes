#include "ChessRecordStruct.h"
ChessRecordStruct::ChessRecordStruct()
{}

void ChessRecordStruct::setGameSequence(const wstring &_gameSequence)
{
    this->gameSequence=_gameSequence;
}

wstring ChessRecordStruct::getGameSequence() const
{
    return this->gameSequence;
}

void ChessRecordStruct::setGameName(const wstring &_gameName)
{
    this->gameName=_gameName;
}

wstring ChessRecordStruct::getGameName() const
{
    return this->gameName;
}

void ChessRecordStruct::setGameRoundsGlobal(const wstring &_gameRoundsGlobal)
{
    this->gameRoundsGlobal=_gameRoundsGlobal;
}

wstring ChessRecordStruct::getGameRoundsGlobal() const
{
    return this->gameRoundsGlobal;
}

void ChessRecordStruct::setGameDate(const wstring &_gameDate)
{
    this->gameDate=_gameDate;
}

wstring ChessRecordStruct::getGameDate() const
{
    return this->gameDate;
}

void ChessRecordStruct::setGameRedTeam(const wstring &_gameRedTeam)
{
    this->gameRedTeam=_gameRedTeam;
}

wstring ChessRecordStruct::getGameRedTeam() const
{
    return this->gameRedTeam;
}

void ChessRecordStruct::setGameRedName(const wstring &_gameRedName)
{
    this->gameRedName=_gameRedName;
}

wstring ChessRecordStruct::getGameRedName() const
{
    return this->gameRedName;
}

void ChessRecordStruct::setGameBlackTeam(const wstring &_gameBlackTeam)
{
    this->gameBlackTeam=_gameBlackTeam;
}

wstring ChessRecordStruct::getGameBlackTeam() const
{
    return this->gameBlackTeam;
}

void ChessRecordStruct::setGameBlackName(const wstring &_gameBlackName)
{
    this->gameBlackName=_gameBlackName;
}

wstring ChessRecordStruct::getGameBlackName() const
{
    return this->gameBlackName;
}

void ChessRecordStruct::setGameResult(const wstring &_gameResult)
{
    this->gameResult=_gameResult;
}

wstring ChessRecordStruct::getGameResult() const
{
    return this->gameResult;
}

void ChessRecordStruct::setGameStepNumber(const wstring &_gameStepNumber)
{
    this->gameStepNumber=_gameStepNumber;
}

wstring ChessRecordStruct::getGameStepNumber() const
{
    return this->gameStepNumber;
}

void ChessRecordStruct::setGameECCOName(const wstring &_gameECCOName)
{
    this->gameECCOName=_gameECCOName;
}

wstring ChessRecordStruct::getGameECCOName() const
{
    return this->gameECCOName;
}

void ChessRecordStruct::setGameSource(const wstring &_gameSource)
{
    this->gameSource=_gameSource;
}

wstring ChessRecordStruct::getGameSource() const
{
    return this->gameSource;
}

void ChessRecordStruct::setGameRecorder(const wstring &_gameRecorder)
{
    this->gameRecorder=_gameRecorder;
}

wstring ChessRecordStruct::getGameRecorder() const
{
    return this->gameRecorder;
}

void ChessRecordStruct::setGameLookTime(const wstring &_gameLookTime)
{
    this->gameLookTime=_gameLookTime;
}

wstring ChessRecordStruct::getGameLookTime() const
{
    return this->gameLookTime;
}

void ChessRecordStruct::addGameTextMove(const wstring &_gameTextMove)
{
    wstring problemStr[8]={L"前仕",L"前士",L"后仕",L"后士",L"前象",L"后象",L"前相",L"后相"};
    wstring moveSub=_gameTextMove.substr(0,2);
    for(int i=0;i<8;i++)
    {
	if(moveSub == problemStr[i])
	{
		wchar_t chessManName=moveSub[1];
		for(int j=gameTextMove.size()-1;j>=0;j--)
		{
			wstring _textMove=gameTextMove[j];
			if(_textMove[0] == chessManName)
			{
				moveSub=wstring()+chessManName+_textMove[3];
				break;
			}
		}
		break;
	}
    }
    this->gameTextMove.push_back(moveSub+_gameTextMove.substr(2,2));
}

void ChessRecordStruct::clearGameTextMove()
{
    this->gameTextMove.clear();
}

int ChessRecordStruct::getGameTextMoveLength() const
{
    return this->gameTextMove.size();
}

wstring ChessRecordStruct::getGameTextMove(int _index) const
{
    if(_index>=0 && _index<this->gameTextMove.size())
    {
        return this->gameTextMove[_index];
    }
    return wstring();
}
