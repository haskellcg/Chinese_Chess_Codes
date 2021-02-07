#ifndef UCCIENGINE_H
#define UCCIENGINE_H
#include "UBUCCI.h"
#include <sys/types.h>
#include <sys/stat.h>
//for mkfifo
#include <unistd.h>
//for read and write
#include <fcntl.h>
//for open
#include "ChessSearch.h"
#define FIFONAME "/tmp/UCCIEngineFIFO"
#define BUFFERSIZE	200
enum UCCIEngineState
{
	BOOT_STATE,
	FREE_STATE,
	THINK_STATE
};
enum UCCIEngineCMD
{
	UCCI_CMD,
	ID_REPLY,
	UCCIOK_REPLY,
	GO_CMD,
	BESTMOVE_REPLY,
	QUIT_CMD,
	BYE_REPLY,
	POSITION_CMD,
	UNKNOW_CMD
};
class UCCIEngine
{
	private:
		UCCIEngineState state;
		UBString fileName;
		ChessSearch *searchEngine;
	private:
		UBString lastFenString;
		int lastMoveLen;
	private:
		UCCIEngineCMD parseStrToEnumCMD(const UBString& _cmdHeaderString) const;
		void initSearchEngine();
		void setSearchModeByStruct();
	public:
		UCCIEngine();
		~UCCIEngine();
		void handleIOFunction();
};
#endif
