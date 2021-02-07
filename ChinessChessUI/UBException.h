#ifndef UBUNTU_EXCEPTION_H
#define UBUNTU_EXCEPTION_H
#include "UBString.h"
class UBException
{
	private:
		int		errorCode;
		UBString	errorMsg;
	public:
		UBException(int _errorCode,const UBString& _errorMsg);
		friend ostream& operator<<(ostream& os,const UBException& _exception);
		int getErrorCode() const;
		void setErrorCode(int _errorCode);
		UBString getErrorMsg() const;
		void setErrorMsg(const UBString& _errorMsg);
};
extern ostream& operator<<(ostream& os,const UBException& _exception);
#endif
