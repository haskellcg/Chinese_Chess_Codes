#ifndef SMARTPTR_H
#define SMARTPTR_H
template<typename T>
class PtrCounter
{
	private:
		int quoteCounter;
		T*	ptrData;
	public:
		PtrCounter(T* _ptrData)
		{
			this->ptrData=_ptrData;
			this->quoteCounter=1;
		}
		~PtrCounter()
		{
			if(this->ptrData!=0)
				delete this->ptrData; 
		}

		T* getPtr() const
		{
			return this->ptrData;
		}
		int getCounter() const
		{
			return this->quoteCounter;
		}
		void addCounter()
		{
			++this->quoteCounter;
		}
		void subCounter()
		{
			--this->quoteCounter;
		}
		void resetCounter()
		{
			this->quoteCounter=0;
		}
};
//智能指针
template<typename T>
class SmartPtr
{
	private:
		PtrCounter<T> * counter;
	public:
		explicit SmartPtr(T* _ptr)
		{
			this->counter=new PtrCounter<T>(_ptr);
		}
		SmartPtr(const SmartPtr& _otherPtr)
		{
			_otherPtr.counter->addCounter();
			this->counter=_otherPtr.counter;
		}
		~SmartPtr()
		{
			this->counter->subCounter();
			if(this->counter->getCounter()==0)
			{
				delete this->counter;
			}
		}

		SmartPtr& operator=(const SmartPtr& _rightPtr)
		{
			if(this==&_rightPtr)
			return *this;
			_rightPtr.counter->addCounter();
			this->counter->subCounter();
			if(this->counter->getCounter()==0)
				delete this->counter;
			this->counter=_rightPtr.counter;
			return *this;
		}
		T* getPtr() const
		{
			return this->counter->getPtr();
		}
		T* operator->() const
		{
			return this->counter->getPtr();
		}
		T& operator*() const
		{
			return *(this->counter->getPtr());
		}
		int quoteNumber() const
		{
			return this->counter->getCounter();
		}
};
template<typename T>
SmartPtr<T> makeSmartPtr(T* _ptr)
{
	return SmartPtr<T>(_ptr);
}
#endif
