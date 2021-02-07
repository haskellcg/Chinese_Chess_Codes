test:test.o ChessRecordStruct.o Txt2Struct.o UBTieTree.o MoveMaker.o ChessManMove.o UBString.o UBException.o CChessStruct.o
	g++ -o test test.o ChessRecordStruct.o Txt2Struct.o UBTieTree.o MoveMaker.o ChessManMove.o UBString.o \
				UBException.o CChessStruct.o
test.o:test.cpp
	g++ -c test.cpp -o test.o
ChessRecordStruct.o:ChessRecordStruct.h ChessRecordStruct.cpp
	g++ -c ChessRecordStruct.cpp -o ChessRecordStruct.o
Txt2Struct.o:Txt2Struct.h Txt2Struct.cpp
	g++ -c Txt2Struct.cpp -o Txt2Struct.o
UBTieTree.o:UBTieTree.h UBTieTree.cpp
	g++ -c UBTieTree.cpp -o UBTieTree.o
MoveMaker.o:../MoveMaker.h ../MoveMaker.cpp
	g++ -c ../MoveMaker.cpp -o MoveMaker.o
ChessManMove.o:../ChessManMove.h ../ChessManMove.cpp
	g++ -c ../ChessManMove.cpp -o ChessManMove.o
UBString.o:../UBString.h ../UBString.cpp
	g++ -c ../UBString.cpp -o UBString.o
UBException.o:../UBException.h ../UBException.cpp
	g++ -c ../UBException.cpp -o UBException.o
CChessStruct.o:../CChessStruct.h ../CChessStruct.cpp
	g++ -c ../CChessStruct.cpp -o CChessStruct.o
clean:
	rm -f *.o
	rm -f test
	rm -f testGDB
testGDB:test.cpp ChessRecordStruct.cpp Txt2Struct.cpp UBTieTree.cpp ../MoveMaker.cpp ../ChessManMove.cpp ../UBString.cpp\
								../UBException.cpp ../CChessStruct.cpp
	g++ -o testGDB test.cpp ChessRecordStruct.cpp Txt2Struct.cpp UBTieTree.cpp ../MoveMaker.cpp ../ChessManMove.cpp\
							../UBString.cpp ../UBException.cpp ../CChessStruct.cpp -g
