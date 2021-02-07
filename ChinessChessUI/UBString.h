#ifndef UBUNTU_STRING_H
#define UBUNTU_STRING_H
#include <iostream>
using std::ostream;
using std::istream;
//字符串类
class UBString
{
	private:
		char* 		ubData;
		int		ubDataLen;
		int		ubAllocateLen;
	private:
		int strLen(const char* _ubData) const;
	public:
		//构造函数
		UBString();
		UBString(int capacity);
		UBString(const char* chPtr);
		UBString(const UBString& _string);
		~UBString();
		//获得成员的函数
		char* getData() const;
		int length() const;
		char at(int index) const;
		void reverse();
		//重载运算符函数
		char&	operator[](int index) const; 
		UBString& operator=(const UBString& _string);
		UBString& operator+=(const UBString& _string);
		UBString& operator+=(char ch);
		friend UBString operator+(const UBString& lString,const UBString& rString);
		friend bool operator==(const UBString& lString,const UBString& rString);
		friend bool operator!=(const UBString& lString,const UBString& rString);
		friend bool operator>(const UBString& lString,const UBString& rString);
		friend bool operator<(const UBString& lString,const UBString& rString);
		friend bool operator>=(const UBString& lString,const UBString& rString);
		friend bool operator<=(const UBString& lString,const UBString& rString);
		friend ostream& operator<<(ostream& os,const UBString& _string);
		friend istream& operator>>(istream& in,UBString& _string);
		//Parse函数
		static UBString toString(int value);
		static int parseInt(const UBString& _string);
};
const int initialDefaultSize=32;
extern UBString operator+(const UBString& lString,const UBString& rString);
extern bool operator==(const UBString& lString,const UBString& rString);
extern bool operator!=(const UBString& lString,const UBString& rString);
extern bool operator>(const UBString& lString,const UBString& rString);
extern bool operator<(const UBString& lString,const UBString& rString);
extern bool operator>=(const UBString& lString,const UBString& rString);
extern bool operator<=(const UBString& lString,const UBString& rString);
extern ostream& operator<<(ostream& os,const UBString& _string);
extern istream& operator>>(istream& in,UBString& _string);
#endif
