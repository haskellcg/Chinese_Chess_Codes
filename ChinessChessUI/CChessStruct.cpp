#include "CChessStruct.h"
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;
using std::setw;
bool CChessStruct::isLegalFENStr(const char* FENStr) const
{
	if(FENStr==NULL)
		return false;
	int row=10;
	int column=9;
	bool isNumber=false;
	int index=0;
	int chessManNumber[14]={1,2,2,2,2,2,5,
				1,2,2,2,2,2,5};
	while(*(FENStr+index)!=' ')
	{
		switch(*(FENStr+index))	
		{
			case 'K':--chessManNumber[0];
				 if(chessManNumber[0]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case 'A':--chessManNumber[1];
				 if(chessManNumber[1]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case 'B':--chessManNumber[2];
				 if(chessManNumber[2]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case 'N':--chessManNumber[3];
				 if(chessManNumber[3]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case 'R':--chessManNumber[4];
				 if(chessManNumber[4]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case 'C':--chessManNumber[5];
				 if(chessManNumber[5]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case 'P':--chessManNumber[6];
				 if(chessManNumber[6]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case 'k':--chessManNumber[7];
				 if(chessManNumber[7]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case 'a':--chessManNumber[8];
				 if(chessManNumber[8]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case 'b':--chessManNumber[9];
				 if(chessManNumber[9]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case 'n':--chessManNumber[10];
				 if(chessManNumber[10]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case 'r':--chessManNumber[11];
				 if(chessManNumber[11]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case 'c':--chessManNumber[12];
				 if(chessManNumber[12]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case 'p':--chessManNumber[13];
				 if(chessManNumber[13]<0)
					return false;
				 --column;	
				 if(column<0)
					return false;
				 isNumber=false;
				 break;
			case '1':if(isNumber) return false;
				 isNumber=true;
				 column=column-1;
				 if(column<0)
					return false;
				 break;
			case '2':if(isNumber) return false;
				 isNumber=true;
				 column=column-2;
				 if(column<0)
					return false;
				 break;
			case '3':if(isNumber) return false;
				 isNumber=true;
				 column=column-3;
				 if(column<0)
					return false;
				 break;
			case '4':if(isNumber) return false;
				 isNumber=true;
				 column=column-4;
				 if(column<0)
					return false;
				 break;
			case '5':if(isNumber) return false;
				 isNumber=true;
				 column=column-5;
				 if(column<0)
					return false;
				 break;
			case '6':if(isNumber) return false;
				 isNumber=true;
				 column=column-6;
				 if(column<0)
					return false;
				 break;
			case '7':if(isNumber) return false;
				 isNumber=true;
				 column=column-7;
				 if(column<0)
					return false;
				 break;
			case '8':if(isNumber) return false;
				 isNumber=true;
				 column=column-8;
				 if(column<0)
					return false;
				 break;
			case '9':if(isNumber) return false;
				 isNumber=true;
				 column=column-9;
				 if(column<0)
					return false;
				 break;
			case '/':if(column!=0)
					return false;
				 column=9;
				 --row;
				 if(row==0)
					return false;
				 isNumber=false;
				 break;
			default:return false;	
		}
		++index;
	}
	if(row!=1 || column!=0)
		return false;
	++index;
	if(*(FENStr+index)!='w'  &&  *(FENStr+index)!='b')
		return false;
	++index;
	if(*(FENStr+index)!=' ')
		return false;
	++index;
	if(*(FENStr+index)!='-')
		return false;
	++index;
	if(*(FENStr+index)!=' ')
		return false;
	++index;
	if(*(FENStr+index)!='-')
		return false;
	++index;
	if(*(FENStr+index)!=' ')
		return false;
	++index;
	if(!isLegalInteger(&index,FENStr))
		return false;
	if(*(FENStr+index)!=' ')
		return false;
	++index;
	if(!isLegalInteger(&index,FENStr))
		return false;
	if(*(FENStr+index)!='\0')
		return false;
	return true;
}
//此处整型长度不超过8位
bool CChessStruct::isLegalInteger(int* index,const char* str) const
{
	int len=0;
	int intLen=8;
	if(*index<0)	
		return false;
	while(*(str+len)!='\0')
		++len;
	if(*index>len-1)
		return false;
	while(*(str+*index)>='0' && *(str+*index)<='9')
	{
		++(*index);
		--intLen;
		if(intLen<0)
			return false;
	}
	if(intLen==8) return false;
	return true;

}
int CChessStruct::getIntegerFromStr(int* index,const char* str) const
{
	int base=10;
	int result=0;
	while(*(str+*index)>='0' && *(str+*index)<='9')
	{
		result=result*base+(*(str+*index)-'0');
		++*index;
	}
	return result;
}
void CChessStruct::integerToStr(int integer,int* index,char* str) const
{
	char antiResult[20];
	int i=0;
	while(integer/10!=0)
	{
		antiResult[i++]=(char)(integer%10+'0');
		integer=integer/10;
	}
	antiResult[i++]=(char)(integer+'0');
	antiResult[i]='\0';
	--i;
	while(i>=0)
	{
		*(str+*index)=antiResult[i];
		++*index;
		--i;
	}
}
int CChessStruct::getChessManSubScript(char manCh) const
{
	switch(manCh)
	{
		case 'k':return 0;
		case 'a':return 1;
		case 'b':return 2;
		case 'n':return 3;
		case 'r':return 4;
		case 'c':return 5;
		case 'p':return 6;
		case 'K':return 7;
		case 'A':return 8;
		case 'B':return 9;
		case 'N':return 10;
		case 'R':return 11;
		case 'C':return 12;
		case 'P':return 13;
	}
    return 0;
}
int CChessStruct::getSameNumber(int i,int j,const int* buffer) const
{
	int temp=*(buffer+i*16+j);
	int number=1;
	while(++j<=11 && *(buffer+i*16+j)==temp)
	{
		++number;
	}
	return number;
}
CChessStruct::CChessStruct()
{
	char normalFEN[]="rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 0";
	initializeFromFEN(normalFEN);
}
CChessStruct::CChessStruct(const char* FENStr)
{
	if(isLegalFENStr(FENStr))
	{
		initializeFromFEN(FENStr);
	}
	else
	{
		char normalFEN[]="rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 0";
		initializeFromFEN(normalFEN);
	}
}
bool CChessStruct::initializeFromFEN(const char* FENStr)
{
	if(!isLegalFENStr(FENStr))	
		return false;
	int index=0;
	int i=3,j=3;
	int chessManValue[]={blackKing,blackAdvisorOne,blackBishopOne,blackKnightOne,blackRookOne,blackCannonOne,blackPawnOne,
			     redKing,redAdvisorOne,redBishopOne,redKnightOne,redRookOne,redCannonOne,redPawnOne};
	for(;index<256;index++) cChessArray[index]=0;
	for(index=0;index<32;index++) cChessManPosition[index]=0;
	index=0;
	while(*(FENStr+index)!=' ')
	{
		if((*(FENStr+index)>='a' && *(FENStr+index)<='z') || (*(FENStr+index)>='A' && *(FENStr+index)<='Z'))
		{
			cChessArray[i*16+j]=chessManValue[getChessManSubScript(*(FENStr+index))];
			++j;
			++chessManValue[getChessManSubScript(*(FENStr+index))];
		}
		if(*(FENStr+index)>='1' && *(FENStr+index)<='9')
		{
			int step=*(FENStr+index)-'0';
			j=j+step;
		}
		if(*(FENStr+index)=='/')
		{
			++i;
			j=3;	
		}
		++index;
	}
	++index;
	if(*(FENStr+index)=='b')
		isRed=false;
	else
		isRed=true;
	index=index+6;
	captureStepNumber=getIntegerFromStr(&index,FENStr);
	++index;
	roundsNumber=getIntegerFromStr(&index,FENStr);

	i=3;j=3;
	int value=0,base=i*16+j;
	for(;i<=12;i++)
	{
		for(;j<=11;j++)
		{
			value=cChessArray[base];
			if(value)
			{
				cChessManPosition[value-16]=base;
			}
			if(j!=11)
				++base;
		}
		base+=8;
		j=3;
	}
	return true;
}
SmartPtr<char> CChessStruct::toFENStr() const
{
	char* FENBuffer=new char[100];
	int index=0,i=3,j=3; 
	while(i<=12)
	{
		int mutiBase=i*16;
		while(j<=11)
		{
			char result;
			switch(cChessArray[mutiBase+j])
			{
				case redKing:			result='K';break;
				case redAdvisorOne:
				case redAdvisorTwo:		result='A';break;
				case redBishopOne:
				case redBishopTwo:		result='B';break;
				case redKnightOne:
				case redKnightTwo:		result='N';break;
				case redRookOne:
				case redRookTwo:		result='R';break;
				case redCannonOne:
				case redCannonTwo:		result='C';break;
				case redPawnOne:
				case redPawnTwo:
				case redPawnThree:
				case redPawnFour:
				case redPawnFive:		result='P';break;
				case blackKing:			result='k';break;
				case blackAdvisorOne:
				case blackAdvisorTwo:		result='a';break;
				case blackBishopOne:
				case blackBishopTwo:		result='b';break;
				case blackKnightOne:
				case blackKnightTwo:		result='n';break;
				case blackRookOne:
				case blackRookTwo:		result='r';break;
				case blackCannonOne:
				case blackCannonTwo:		result='c';break;
				case blackPawnOne:
				case blackPawnTwo:
				case blackPawnThree:
				case blackPawnFour:
				case blackPawnFive:		result='p';break;
				case 0:				int number;
								number=getSameNumber(i,j,cChessArray);
								result=(char)('0'+number);
								j=j+number-1;
								break;
				default:
								return makeSmartPtr<char>(0);
			}
			FENBuffer[index]=result;
			++index;
			++j;
		}
		if(i<12)
		{
			FENBuffer[index]='/';
			++index;
		}
		++i;
		j=3;
	}
	FENBuffer[index]=' ';
	++index;
	FENBuffer[index]=(isRed==0?'b':'w');
	++index;
	FENBuffer[index]=' ';
	++index;
	FENBuffer[index]='-';
	++index;
	FENBuffer[index]=' ';
	++index;
	FENBuffer[index]='-';
	++index;
	FENBuffer[index]=' ';
	++index;
	integerToStr(captureStepNumber,&index,FENBuffer);
	FENBuffer[index]=' ';
	++index;
	integerToStr(roundsNumber,&index,FENBuffer);
	FENBuffer[index]='\0';
	return makeSmartPtr(FENBuffer);
}
void CChessStruct::reverseStruct()
{
	int temp,source,dest;
	int value=0,base=3*16+3;
	int i,j;
	for(i=0;i<=4;i++)
		for(j=0;j<=8;j++)
		{
			source=(i+3)*16+(j+3);
			dest=(9-i+3)*16+(8-j+3);
			temp=cChessArray[source];
			cChessArray[source]=cChessArray[dest];
			cChessArray[dest]=temp;
		}
	
	for(i=3;i<=12;i++)
	{
		for(j=3;j<=11;j++)
		{
			value=cChessArray[base];
			if(value)
			{
				cChessManPosition[value-16]=base;
			}
			if(j!=11)
				++base;
		}
		base+=8;
		j=3;
	}
}
void CChessStruct::reverseAll()
{
	reverseStruct();
	isRed=!isRed;
}
bool CChessStruct::isTurnRed() const
{
	return this->isRed;
}
int CChessStruct::stepNumberFromLastCapture() const
{
	return this->captureStepNumber;
}
int CChessStruct::getRoundsNumber() const
{
	return this->roundsNumber;
}
void CChessStruct::report() const
{
	for(int i=3;i<=12;i++)
	{
		for(int j=3;j<=11;j++)
		{
			cout<<" "<<setw(4)<<(int)cChessArray[i*16+j]<<" ";
		}
		cout<<endl;	
	}	
	cout<<"红黑："<<isRed<<endl;
	cout<<"吃子步数："<<captureStepNumber<<endl;
	cout<<"回合数："<<roundsNumber<<endl;
	for(int i=0;i<32;i++)
	{
		cout<<"["<<setw(2)<<(i+16)<<"]"<<":"<<setw(3)<<(int)cChessManPosition[i]<<" ";
		if(i==15)
			cout<<endl;
	}
	cout<<endl;
}
