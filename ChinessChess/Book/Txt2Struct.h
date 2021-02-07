#ifndef TXT2STRUCT_H
#define TXT2STRUCT_H
#include "ChessRecordStruct.h"
class Txt2Struct
{
	private:
		wstring tempSequence;
		wstring tempName;
		wstring tempRoundsGlobal;
		wstring tempDate;
		wstring tempRedTeam;
		wstring tempRedName;
		wstring tempBlackTeam;
		wstring tempBlackName;
		wstring tempResult;
		wstring tempStepNumber;
		wstring tempECCOName;
		wstring tempSource;
		wstring tempRecorder;
		wstring tempLookTime;
		vector<wstring> tempTextMove;
		vector<ChessRecordStruct> convertResult;
	private:
		wchar_t convertNumberWChar(wchar_t wch) const;
	public:
		void convertFunction(const string& utf_8_FileName);
		vector<ChessRecordStruct>& getStructVector();
		int getResultLength() const;
		ChessRecordStruct getStructAt(int _index);
};
#endif
