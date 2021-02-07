H.264:H.264.o CChessStruct.o ChessManMove.o ChessSearch.o EvaluateSituation.o UBString.o UBException.o UBStack.h MoveMaker.o\
     UBHashTable.h UBRandom.o Zobrist.o UBTypeDef.h UBHashTable.h UBUCCI.o ChessRecordStruct.o Txt2Struct.o UBTieTree.o\
	UCCIEngine.o 
	g++ H.264.o CChessStruct.o ChessManMove.o ChessSearch.o EvaluateSituation.o UBString.o UBException.o MoveMaker.o\
	    UBRandom.o Zobrist.o UBUCCI.o ChessRecordStruct.o Txt2Struct.o UBTieTree.o UCCIEngine.o -o H.264
H.264.o:H.264.cpp
	g++ -c H.264.cpp -o H.264.o
CChessStruct.o:CChessStruct.cpp CChessStruct.h SmartPtr.h
	g++ -c CChessStruct.cpp -o CChessStruct.o
ChessManMove.o:ChessManMove.cpp ChessManMove.h
	g++ -c ChessManMove.cpp -o ChessManMove.o
ChessSearch.o:ChessSearch.cpp ChessSearch.h
	g++ -c ChessSearch.cpp -o ChessSearch.o
EvaluateSituation.o:EvaluateSituation.cpp EvaluateSituation.h
	g++ -c EvaluateSituation.cpp -o EvaluateSituation.o
UBString.o:UBString.h UBString.cpp
	g++ -c UBString.cpp -o UBString.o
UBException.o:UBException.h UBException.cpp
	g++ -c UBException.cpp -o UBException.o
MoveMaker.o:MoveMaker.h MoveMaker.cpp
	g++ -c MoveMaker.cpp -o MoveMaker.o
UBRandom.o:UBRandom.h UBRandom.cpp UBTypeDef.h
	g++ -c UBRandom.cpp -o UBRandom.o
Zobrist.o:Zobrist.h Zobrist.cpp UBTypeDef.h
	g++ -c Zobrist.cpp -o Zobrist.o
UBUCCI.o:UBUCCI.h UBUCCI.cpp
	g++ -c UBUCCI.cpp -o UBUCCI.o
ChessRecordStruct.o:./Book/ChessRecordStruct.h ./Book/ChessRecordStruct.cpp
	g++ -c ./Book/ChessRecordStruct.cpp -o ChessRecordStruct.o
Txt2Struct.o:./Book/Txt2Struct.h ./Book/Txt2Struct.cpp
	g++ -c ./Book/Txt2Struct.cpp -o Txt2Struct.o
UBTieTree.o:./Book/UBTieTree.h ./Book/UBTieTree.cpp
	g++ -c ./Book/UBTieTree.cpp -o UBTieTree.o
UCCIEngine.o:UCCIEngine.h UCCIEngine.cpp
	g++ -c UCCIEngine.cpp -o UCCIEngine.o
clean:
	$(RM) H.264
	$(RM) *.o
