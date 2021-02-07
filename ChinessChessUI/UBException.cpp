#include "UBException.h"
UBException::UBException(int _errorCode,const UBString& _errorMsg):errorCode(_errorCode),errorMsg(_errorMsg)
{}
ostream& operator<<(ostream& os,const UBException& _exception)
{
	os<<"ErrorCode:"<<_exception.errorCode<<"ErrorMsg:"<<_exception.errorMsg;
	return os;
}
int UBException::getErrorCode() const
{
	return this->errorCode;
}
void UBException::setErrorCode(int _errorCode)
{
	this->errorCode=_errorCode;
}
UBString UBException::getErrorMsg() const
{
	return this->errorMsg;
}
void UBException::setErrorMsg(const UBString& _errorMsg)
{
	this->errorMsg=_errorMsg;
}
