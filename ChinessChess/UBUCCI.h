#ifndef UBUNTU_UCCI_H
#define UBUNTU_UCCI_H
#include "UBString.h"
#include "MoveMaker.h"
#define BUFFER_LENGTH 20
#define OPTION_LENGTH 10
#define MOVES_LENGTH 70 
extern UBString formatBuffer[BUFFER_LENGTH];
extern int      formatBufferLen;
bool isLegalSpecialChar(char ch);
bool isNumber(char ch);
bool isLetter(char ch);
bool analyString(const UBString& _string);

struct UCCI_CMD_STRUCT
{
	UBString cmdName;

	UCCI_CMD_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct ID_REPLY_STRUCT
{
	UBString replyName;
	UBString replyType;
	UBString replyContent;

	ID_REPLY_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct Var_Value
{
	UBString varCMDName;
	UBString value;
};
struct OPTION_REPLY_STRUCT
{
	UBString replyName;
	UBString replyType;
	UBString replyParaTypeCMDName;
	UBString replyParaTypeContent;
	UBString replyMinCMDName;
	UBString replyMinValue;
	UBString replyMaxCMDName;
	UBString replyMaxValue;
	Var_Value varPair[OPTION_LENGTH];
	int	  varPairLen;
	UBString replyDefaultCMDName;
	UBString replyDefaultValue;
	
	OPTION_REPLY_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct UCCIOK_REPLY_STRUCT
{
	UBString replyName;

	UCCIOK_REPLY_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct ISREADY_CMD_STRUCT
{
	UBString cmdName;
	
	ISREADY_CMD_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct READYOK_REPLY_STRUCT
{
	UBString replyName;

	READYOK_REPLY_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct SETOPTION_CMD_STRUCT
{
	UBString cmdName;
	UBString cmdType;
	UBString cmdContent;

	SETOPTION_CMD_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct POSITION_CMD_STRUCT
{
	UBString cmdName;
	UBString fenCMDName;
	UBString fenString;
	UBString movesCMDName;
	UBString movesForFen[MOVES_LENGTH];
	int	 movesForFenLen;
	
	POSITION_CMD_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct BANMOVES_CMD_STRUCT
{
	UBString cmdName;
	UBString banmoves[MOVES_LENGTH];
	int	 banmovesLen;
	
	BANMOVES_CMD_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct MODE_DEPTH
{
	UBString modeName;
	//可以为infinite
	UBString modeDepth;
};
struct MODE_NODES
{
	UBString modeName;
	UBString modeNodeNumber;
};
struct MODE_TIME
{
	UBString modeName;
	UBString restTime;
	UBString movesToGoCMDName;
	UBString restSteps;
	UBString incrementCMDName;
	UBString incrementForStep;
	UBString oppTimeCMDName;
	UBString oppRestTime;
	UBString oppMovesToGoCMDName;
	UBString oppRestSteps;
	UBString oppIncrementCMDName;
	UBString oppIncrementForStep;
};
struct GO_CMD_STRUCT
{
	UBString cmdName;
	UBString cmdType;
	MODE_DEPTH depthMode;
	MODE_NODES nodesMode;
	MODE_TIME  timeMode;
	int	 cmdModeIndex;
	
	GO_CMD_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct INFO_TIME_NODE
{
	UBString timeCMDName;
	UBString usedTime;
	UBString nodesCMDName;
	UBString searchedNodes;
};
struct INFO_DEPTH
{
	UBString depthCMDName;
	UBString searchedDepth;
	UBString scoreCMDName;
	UBString scoreForNow;
	UBString pvCMDName;
	UBString searchedMove[MOVES_LENGTH];
	int	 searchedMoveLen;
};
struct INFO_CURRENTMOVE
{
	UBString currentCMDName;
	UBString currentMove;
};
struct INFO_MESSAGE
{
	UBString messageCMDName;
	UBString messageContent;
};
struct INFO_REPLY_STRUCT
{
	UBString 		replyName;
	INFO_TIME_NODE 		infoTimeNode;
	INFO_DEPTH     		infoDepth;
	INFO_CURRENTMOVE	infoCurrentMove;
	INFO_MESSAGE		infoMessage;
	int			infoModeIndex;

	INFO_REPLY_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct PONDERHIT_CMD_STRUCT
{
	UBString cmdName;
	UBString drawOption;

	PONDERHIT_CMD_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct STOP_CMD_STRUCT
{
	UBString cmdName;
	
	STOP_CMD_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct BESTMOVE_REPLY_STRUCT
{
	UBString replyName;
	UBString bestMove;
	UBString ponderCMDName;
	UBString goodMove;
	UBString drawOption;

	BESTMOVE_REPLY_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct NOBESTMOVE_REPLY_STRUCT
{
	UBString replyName;
	
	NOBESTMOVE_REPLY_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct PROBE_CMD_STRUCT
{
	UBString cmdName;
	UBString fenCMDName;
	UBString fenString;
	UBString movesCMDName;
	UBString probeMoves[MOVES_LENGTH];
	int 	 probeMovesLen;

	PROBE_CMD_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct POPHASH_REPLY_STRUCT
{
	UBString replyName;
	UBString bestMoveCMDName;
	UBString bestMove;
	UBString lowerBoundCMDName;
	UBString betaValue;
	UBString depthForLowCMDName;
	UBString depthForLow;
	UBString upperBoundCMDName;
	UBString alphaValue;
	UBString depthForUpCMDName;
	UBString depthForUp;

	POPHASH_REPLY_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct QUIT_CMD_STRUCT
{
	UBString cmdName;

	QUIT_CMD_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
struct BYE_REPLY_STRUCT
{
	UBString replyName;
	
	BYE_REPLY_STRUCT();
	bool setMember(const UBString& _ucciCMD);
	UBString toString() const;
};
#endif
