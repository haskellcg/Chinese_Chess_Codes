#ifndef CHESSRECORDSTRUCT_H
#define CHESSRECORDSTRUCT_H
#include <string>
#include <vector>
#include <iostream>
using namespace std;
class ChessRecordStruct
{
	private:
		wstring gameSequence;
		wstring gameName;
		wstring gameRoundsGlobal;
		wstring gameDate;
		wstring gameRedTeam;
		wstring gameRedName;
		wstring gameBlackTeam;
		wstring gameBlackName;
		wstring gameResult;
		wstring gameStepNumber;
		wstring gameECCOName;
		wstring gameSource;
		wstring gameRecorder;
		wstring gameLookTime;
		vector<wstring>	gameTextMove;	
	public:
		ChessRecordStruct();
        	void setGameSequence(const wstring& _gameSequence);
		wstring getGameSequence() const;
       	 	void setGameName(const wstring& _gameName);
		wstring getGameName() const;
        	void setGameRoundsGlobal(const wstring& _gameRoundsGlobal);
		wstring getGameRoundsGlobal() const;
        	void setGameDate(const wstring& _gameDate);
		wstring getGameDate() const;
        	void setGameRedTeam(const wstring& _gameRedTeam);
		wstring getGameRedTeam() const;
       		void setGameRedName(const wstring& _gameRedName);
		wstring getGameRedName() const;
        	void setGameBlackTeam(const wstring& _gameBlackTeam);
		wstring getGameBlackTeam() const;
        	void setGameBlackName(const wstring& _gameBlackName);
		wstring getGameBlackName() const;
        	void setGameResult(const wstring& _gameResult);
		wstring getGameResult() const;
        	void setGameStepNumber(const wstring& _gameStepNumber);
		wstring getGameStepNumber() const;
        	void setGameECCOName(const wstring& _gameECCOName);
		wstring getGameECCOName() const;
        	void setGameSource(const wstring& _gameSource);
		wstring getGameSource() const;
        	void setGameRecorder(const wstring& _gameRecorder);
		wstring getGameRecorder() const;
        	void setGameLookTime(const wstring& _gameLookTime);
		wstring getGameLookTime() const;
        	void addGameTextMove(const wstring& _gameTextMove);
		void clearGameTextMove();
		int  getGameTextMoveLength() const;
		wstring getGameTextMove(int _index) const;
};
#endif
