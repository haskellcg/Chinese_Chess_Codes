#include "UBString.h"
#include "UBException.h"
using std::cout;
using std::endl;
int UBString::strLen(const char* _ubData) const
{
	if(_ubData == NULL)
	{
		cout<<"NULL pointer _ubData ..."<<endl;
		return -1;
	}
	int len=0;
	while(*(_ubData+len)!='\0')
		++len;
	return len;
}
UBString::UBString()
{
	ubData=new char[initialDefaultSize];
	ubData[0]='\0';
	ubAllocateLen=initialDefaultSize;
	ubDataLen=0;
}
UBString::UBString(int capacity)
{
	if(capacity<0)
		throw UBException(-1,"capacity can't be nagative !");
	ubData=new char[capacity];
	ubData[0]='\0';
	ubAllocateLen=capacity;
	ubDataLen=0;
}
UBString::UBString(const char* chPtr)
{
	int chPtrLen=strLen(chPtr);
	if(chPtrLen==-1)
	{
		ubData=new char[initialDefaultSize];
		ubData[0]='\0';
		ubAllocateLen=initialDefaultSize;
		ubDataLen=0;
	}
	else
	{
		ubData=new char[chPtrLen*2];
		for(int i=0;i<chPtrLen;i++)	
		{
			*(ubData+i)=*(chPtr+i);
		}
		ubData[chPtrLen]='\0';
		ubAllocateLen=chPtrLen*2;
		ubDataLen=chPtrLen;
	}
}
UBString::UBString(const UBString& _string)
{
	this->ubAllocateLen=_string.ubAllocateLen;
	this->ubDataLen=_string.ubDataLen;
	this->ubData=new char[this->ubAllocateLen];
	for(int i=0;i<this->ubDataLen;i++)
	{
		*(this->ubData+i)=*(_string.ubData+i);
	}
	this->ubData[this->ubDataLen]='\0';
}
UBString::~UBString()
{
	if(ubData!=NULL)
		delete[] ubData;
}
char* UBString::getData() const
{
	return this->ubData;
}
int UBString::length() const
{
	return this->ubDataLen;
}
char UBString::at(int index) const
{
	if(index<0 || index>ubDataLen-1)
	{
		throw UBException(-1,"ERROR:At parameter index out of bound....");
	}
	return this->ubData[index];
}
void UBString::reverse()
{
	int firstPtr=0,secondPtr=this->ubDataLen-1;
	char temp;
	while(firstPtr < secondPtr)
	{
		temp=this->ubData[firstPtr];
		this->ubData[firstPtr]=this->ubData[secondPtr];
		this->ubData[secondPtr]=temp;
		++firstPtr;
		--secondPtr;
	}
}
char& UBString::operator[](int index) const
{
	if(index<0 || index>ubDataLen-1)
	{
		throw UBException(-1,"ERROR:At parameter index out of bound....");
	}
	return this->ubData[index];
}
UBString& UBString::operator=(const UBString& _string)
{
	if(this == &_string)
		return *this;
	if(_string.ubDataLen > this->ubAllocateLen-1)
	{
		delete[] this->ubData;
		do
		{
			this->ubAllocateLen=this->ubAllocateLen*2;
		}while(_string.ubDataLen > this->ubAllocateLen-1);
		this->ubData=new char[this->ubAllocateLen];
		for(int i=0;i<_string.ubDataLen;i++)
		{
			*(this->ubData+i)=*(_string.ubData+i);
		}
		this->ubData[_string.ubDataLen]='\0';
		this->ubDataLen=_string.ubDataLen;
	}
	else
	{
		for(int i=0;i<_string.ubDataLen;i++)
		{
			*(this->ubData+i)=*(_string.ubData+i);
		}
		this->ubData[_string.ubDataLen]='\0';
		this->ubDataLen=_string.ubDataLen;
	}
	return *this;
}
UBString& UBString::operator+=(const UBString& _string)
{
	int totalLen=this->ubDataLen + _string.ubDataLen;
	if(totalLen > this->ubAllocateLen-1)
	{
		char* temp=this->ubData;
		do
		{
			this->ubAllocateLen=this->ubAllocateLen*2;
		}while(totalLen > this->ubAllocateLen-1);
		this->ubData=new char[this->ubAllocateLen];
		for(int i=0;i<this->ubDataLen;i++)
		{
			*(this->ubData+i)=*(temp+i);
		}
		delete[] temp;
		for(int i=this->ubDataLen,j=0;i<totalLen;i++,j++)
		{
			*(this->ubData+i)=*(_string.ubData+j);
		}
		this->ubData[totalLen]='\0';
		this->ubDataLen=totalLen;
	}
	else
	{
		for(int i=this->ubDataLen,j=0;i<totalLen;i++,j++)
		{
			*(this->ubData+i)=*(_string.ubData+j);
		}
		this->ubData[totalLen]='\0';
		this->ubDataLen=totalLen;
	}
	return *this;
}
UBString& UBString::operator+=(char ch)
{
	int totalLen=this->ubDataLen+1;
	if(totalLen > this->ubAllocateLen-1)
	{
		char* temp=this->ubData;
		do
		{
			this->ubAllocateLen=this->ubAllocateLen*2;
		}while(totalLen > this->ubAllocateLen-1);
		this->ubData=new char[this->ubAllocateLen];
		for(int i=0;i<this->ubDataLen;i++)
		{
			*(this->ubData+i)=*(temp+i);
		}
		delete[] temp;
		this->ubData[this->ubDataLen]=ch;
		this->ubData[totalLen]='\0';
		this->ubDataLen=totalLen;
	}
	else
	{
		this->ubData[this->ubDataLen]=ch;
		this->ubData[totalLen]='\0';
		this->ubDataLen=totalLen;
	}
	return *this;
}
UBString operator+(const UBString& lstring,const UBString& rstring)
{
	int totalLen=lstring.ubDataLen+rstring.ubDataLen;
	UBString result(totalLen*2);
	for(int i=0;i<lstring.ubDataLen;i++)
	{
		*(result.ubData+i)=*(lstring.ubData+i);
	}
	for(int i=lstring.ubDataLen,j=0;i<totalLen;i++,j++)
	{
		*(result.ubData+i)=*(rstring.ubData+j);
	}
	result.ubData[totalLen]='\0';
	result.ubDataLen=totalLen;
	return result;
}
bool operator==(const UBString& lstring,const UBString& rstring)
{
	if(&lstring == &rstring)
		return true;
	if(lstring.ubDataLen != rstring.ubDataLen)
		return false;
	for(int i=0;i<lstring.ubDataLen;i++)
	{
		if(*(lstring.ubData+i) != *(rstring.ubData+i))
			return false;
	}
	return true;
}
bool operator!=(const UBString& lstring,const UBString& rstring)
{
	return (!operator==(lstring,rstring));
}
bool operator>(const UBString& lstring,const UBString& rstring)
{
	int minLen=(lstring.ubDataLen > rstring.ubDataLen ? rstring.ubDataLen : lstring.ubDataLen);
	for(int i=0;i<minLen;i++)
	{
		if(*(lstring.ubData+i) == *(rstring.ubData+i))
			continue;
		else if(*(lstring.ubData+i) > *(rstring.ubData+i))
				return true;
		else
				return false;
	}
	return lstring.ubDataLen>rstring.ubDataLen;
}
bool operator<(const UBString& lstring,const UBString& rstring)
{
	int minLen=(lstring.ubDataLen > rstring.ubDataLen ? rstring.ubDataLen : lstring.ubDataLen);
	for(int i=0;i<minLen;i++)
	{
		if(*(lstring.ubData+i) == *(rstring.ubData+i))
			continue;
		else if(*(lstring.ubData+i) < *(rstring.ubData+i))
				return true;
		else
				return false;
	}
	return lstring.ubDataLen<rstring.ubDataLen;
}
bool operator<=(const UBString& lstring,const UBString& rstring)
{
	return (!operator>(lstring,rstring));
}
bool operator>=(const UBString& lstring,const UBString& rstring)
{
	return (!operator<(lstring,rstring));
}
ostream& operator<<(ostream& os,const UBString& _string)
{
	os<<_string.ubData;	
	return os;
}
istream& operator>>(istream& is,UBString& _string)
{
	char ch;
	//清空数据
	_string.ubDataLen=0;
	do
	{
		is>>ch;
		if(ch=='\n')
			break;
		_string.ubData[_string.ubDataLen++]=ch;
	}while(true);
	return is;
}
UBString UBString::toString(int value)
{
	UBString result(16);		//初始容量为16，足以应付一般的整数
	bool isPositive=true;
	char ch;
	if(value<0)
	{
		isPositive=false;
		value=-value;
	}
	do
	{
		ch=(char)(value%10+'0');
		result+=ch;
		value=value/10;
	}while(value!=0);
	if(!isPositive)
		result+='-';
	result.reverse();
	return result;
}
int UBString::parseInt(const UBString& _string)
{
	int result=0;
	bool isPositive=true;
	char ch;
	for(int i=0;i<_string.ubDataLen;i++)
	{
		ch=_string.ubData[i];
		if(i==0 && ch=='-')
		{
			isPositive=false;
		}
		else if(ch>='0' && ch<='9')
		{
			result=result*10+(int)(ch-'0');
		}
		else
			throw UBException(-1,"This String can't parse to int ....");
	}
	return result;
}
