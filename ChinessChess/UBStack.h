#ifndef UBUNTU_STACK_H
#define UBUNTU_STACK_H
#include "UBException.h"
const int UBStackInitialCapacity=128;
//T支持复制
//Top指向第一个空闲的地址
template<typename T>
class UBStack
{
	private:
		T*		ubStackData;
		int		ubStackTop;
		int		ubStackCapacity;
	public:
		UBStack()
		{
			this->ubStackCapacity=UBStackInitialCapacity;
			this->ubStackData=new T[ubStackCapacity];
			this->ubStackTop=0;
		}
		UBStack(int _capacity)
		{
			if(_capacity<0)
				throw UBException(-1,"UBStack capacity can't be nagative !");
			this->ubStackCapacity=_capacity;
			this->ubStackData=new T[_capacity];
			this->ubStackTop=0;
		}
		UBStack(const UBStack& _stack)
		{
			this->ubStackCapacity=_stack.ubStackCapacity;
			this->ubStackTop=_stack.ubStackTop;
			this->ubStackData=new T[_stack.ubStackCapacity];
			for(int i=0;i<_stack.ubStackTop;i++)
			{
				*(this->ubStackData+i)=*(_stack.ubStackData+i);
			}
		}
		~UBStack()
		{
			if(ubStackData!=NULL)
				delete[] ubStackData;
		}
		//通用操作
		T& getTop() const
		{
			if(this->ubStackTop ==0)
			{
				throw UBException(-1,"UBStack has no element..");
			}
			return this->ubStackData[this->ubStackTop-1];
		}
		T pop()
		{
			if(this->ubStackTop ==0)
			{
				throw UBException(-1,"UBStack has no element..");
			}
			return this->ubStackData[--this->ubStackTop];
		}
		void push(const T& _item)
		{
			if(this->ubStackTop > this->ubStackCapacity-1)
			{
				T* temp=this->ubStackData;
				do
				{
					this->ubStackCapacity*=2;
				}while(this->ubStackTop > this->ubStackCapacity-1);
				this->ubStackData=new T[this->ubStackCapacity];
				for(int i=0;i<this->ubStackTop;i++)
				{
					*(this->ubStackData+i)=*(temp+i);
				}
				delete[] temp;
				this->ubStackData[this->ubStackTop++]=_item;
			}
			else
			{
				this->ubStackData[this->ubStackTop++]=_item;
			}
		}
		//获得成员
		int numberOfElement() const
		{
			return this->ubStackTop;
		}
};
#endif
