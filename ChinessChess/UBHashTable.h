#ifndef UBUNTU_HASHTABLE_H
#define UBUNTU_HASHTABLE_H
#include "UBException.h"
//表示哈希表中槽的状态
enum UBHashNodeState
{
	ubHashNode_Active,
	ubHashNode_Deleted,
	ubHashNode_UnActive
};
//节点，其中包含了键值
template <typename Key,typename Value>
struct UBHashNode
{
	Key		nodeKey;
	Value		nodeValue;
	UBHashNodeState	nodeState;
	UBHashNode*	next;
	UBHashNode():next(NULL),nodeState(ubHashNode_UnActive)
	{}
	UBHashNode(const Key& _key,const Value& _value,UBHashNodeState _nodeState,UBHashNode* _next):nodeKey(_key),nodeValue(_value),
										       nodeState(_nodeState),
										       next(_next)
	{}
};
//哈希表
//最后的函数只会用到节点的键、值
template <typename Key,typename Value>
class UBHashTable
{
	//提供哈希函数
	typedef	int			(*UBHashFunc)(const Key&);
	//提供判断冲突函数
	typedef bool			(*IsSame)(const UBHashNode<Key,Value>&,const UBHashNode<Key,Value>&);
	private:
		UBHashNode<Key,Value>*	ubHashTB;
		int 			ubHashTBCapacity;
		UBHashFunc		ubHash;
		IsSame			ubSame;
	private:
		void deleteUBHashTB();
	public:
		UBHashTable(int _ubHashTBCapacity,UBHashFunc _ubHash,IsSame _ubSame);
		UBHashTable(const UBHashTable& _ubHashTable);
		~UBHashTable();
		UBHashTable& operator=(const UBHashTable& _ubHashTable);
		Value&	operator[](const UBHashNode<Key,Value>& _limitEntity);
		bool isContain(const UBHashNode<Key,Value>& _limitEntity);
		int getValue(const UBHashNode<Key,Value>& _limitEntity,Value *_value);
		bool insertNode(const UBHashNode<Key,Value>& _limitEntity);
		bool updateNode(const UBHashNode<Key,Value>& _limitEntity);
		bool deleteNode(const UBHashNode<Key,Value>& _limitEntity);
		void clearHashTable();
		int capacity() const;
		int activeDataLength() const;
		int unActiveDataLength() const;
		int deletedDataLength() const;
};
//删除哈希表项，遍历每一个数组项，对于每一个项删除其中的链表
//linkP:指向要删除的链表表项
//linkQ:指向要删除的下一个表项
template <typename Key,typename Value>
void UBHashTable<Key,Value>::deleteUBHashTB()
{
	UBHashNode<Key,Value>* mainP,*linkP,*linkQ;
	for(int i=0;i<ubHashTBCapacity;i++)
	{
		mainP=ubHashTB+i;
		linkP=mainP->next;
		while(linkP != NULL)
		{
			linkQ=linkP->next;
			delete linkP;
			linkP=linkQ;
		}
	}
	delete[] ubHashTB;
}
template <typename Key,typename Value>
UBHashTable<Key,Value>::UBHashTable(int _ubHashTBCapacity,UBHashFunc _ubHash,IsSame _ubSame):ubHashTBCapacity(_ubHashTBCapacity),
												     ubHash(_ubHash),
												     ubSame(_ubSame)
{
	ubHashTB=new UBHashNode<Key,Value>[ubHashTBCapacity];
}
template <typename Key,typename Value>
UBHashTable<Key,Value>::UBHashTable(const UBHashTable& _ubHashTable):ubHashTBCapacity(_ubHashTable.ubHashTBCapacity),
								     ubHash(_ubHashTable.ubHash),
								     ubSame(_ubHashTable.ubSame)
{
	//p、q负责主要数组方向的遍历
	//np、nq、temp负责链表方向的遍历
	ubHashTB=new UBHashNode<Key,Value>[ubHashTBCapacity];
	UBHashNode<Key,Value>* p,*q,*np,*nq,*temp;
	p=ubHashTB;
	q=_ubHashTable.ubHashTB;
	for(int i=0;i<ubHashTBCapacity;i++)
	{
		*(p+i)=*(q+i);
		nq=(q+i)->next;
		np=p+i;
		while(nq != NULL)
		{
			temp=new UBHashNode<Key,Value>(nq->nodeKey,nq->nodeValue,nq->nodeState,NULL);
			np->next=temp;
			np=np->next;
			nq=nq->next;
		}
	}
}
template <typename Key,typename Value>
UBHashTable<Key,Value>::~UBHashTable()
{
	deleteUBHashTB();
}
template <typename Key,typename Value>
UBHashTable<Key,Value>& UBHashTable<Key,Value>::operator=(const UBHashTable& _ubHashTable)
{
	if(&_ubHashTable == this)
		return this;
	deleteUBHashTB();
	ubHashTBCapacity=_ubHashTable.ubHashTBCapacity;
	ubHash=_ubHashTable.ubHash;
	ubSame=_ubHashTable.ubSame;
	ubHashTB=new UBHashNode<Key,Value>[ubHashTBCapacity];
	UBHashNode<Key,Value>* p,*q,*np,*nq,*temp;
	p=ubHashTB;
	q=_ubHashTable.ubHashTB;
	for(int i=0;i<ubHashTBCapacity;i++)
	{
		*(p+i)=*(q+i);
		nq=(q+i)->next;
		np=p+i;
		while(nq != NULL)
		{
			temp=new UBHashNode<Key,Value>(nq->nodeKey,nq->nodeValue,nq->nodeState,NULL);
			np->next=temp;
			np=np->next;
			nq=nq->next;
		}
	}
	return *this;
}
//返回找到的元素的引用
//但是找不到则抛出异常
template <typename Key,typename Value>
Value& UBHashTable<Key,Value>::operator[](const UBHashNode<Key,Value>& _limitEntity)
{
	UBHashNode<Key,Value>* mainP=ubHashTB,*linkP;
	int index=ubHash(_limitEntity.nodeKey);
	if((*(mainP+index)).nodeState==ubHashNode_Active)
	{
		if(ubSame(*(mainP+index),_limitEntity))
			return (*(mainP+index)).nodeValue;
		linkP=(mainP+index)->next;
		while(linkP != NULL)
		{
			if(ubSame(*linkP,_limitEntity))
				return (*linkP).nodeValue;
			linkP=linkP->next;
		}
		throw UBException(-1,"Can't find the value....");
	}
	else
		throw UBException(-1,"Can't find the value...");
}
//返回是否包含对应的元素
template <typename Key,typename Value>
bool UBHashTable<Key,Value>::isContain(const UBHashNode<Key,Value>& _limitEntity)
{
	UBHashNode<Key,Value>* mainP=ubHashTB,*linkP;
	int index=ubHash(_limitEntity.nodeKey);
	if((*(mainP+index)).nodeState==ubHashNode_Active)
	{
		if(ubSame(*(mainP+index),_limitEntity))
			return true;
		linkP=(mainP+index)->next;
		while(linkP != NULL)
		{
			if(ubSame(*linkP,_limitEntity))
				return true;
			linkP=linkP->next;
		}
	}
	return false;
}
template <typename Key,typename Value>
int UBHashTable<Key,Value>::getValue(const UBHashNode<Key,Value>& _limitEntity,Value *_value)
{
	UBHashNode<Key,Value>* mainP=ubHashTB,*linkP;
	int index=ubHash(_limitEntity.nodeKey);
	if((*(mainP+index)).nodeState==ubHashNode_Active)
	{
		
		if(ubSame(*(mainP+index),_limitEntity))
		{
			*_value=(*(mainP+index)).nodeValue;
			return 0;
		}
		linkP=(mainP+index)->next;
		while(linkP != NULL)
		{
			if(ubSame(*linkP,_limitEntity))
			{
				*_value=(*linkP).nodeValue; 
				return 0;
			}
			linkP=linkP->next;
		}
	}
	return -1;
}
//插入节点，执行了插入动作才返回true，否则返回false
//对于主数组方向的插入：之需要修改对应的结构成员即可
//但是对于链表方向的插入：需要申请一个新节点，连接到最后的位置
template <typename Key,typename Value>
bool UBHashTable<Key,Value>::insertNode(const UBHashNode<Key,Value>& _limitEntity)
{
	UBHashNode<Key,Value>* mainP=ubHashTB,*linkP;
	int index=ubHash(_limitEntity.nodeKey);
	if((*(mainP+index)).nodeState==ubHashNode_Active)
	{
		if(ubSame(*(mainP+index),_limitEntity))
			return false;
		linkP=mainP+index;
		while(linkP->next != NULL)
		{
			if(ubSame(*(linkP->next),_limitEntity))
				return false;
			linkP=linkP->next;
		}
		linkP->next=new UBHashNode<Key,Value>(_limitEntity.nodeKey,_limitEntity.nodeValue,ubHashNode_Active,NULL);
		return true;
	}
	else
	{
		(*(mainP+index)).nodeKey=_limitEntity.nodeKey;
		(*(mainP+index)).nodeValue=_limitEntity.nodeValue;
		(*(mainP+index)).nodeState=ubHashNode_Active;
		(*(mainP+index)).next=NULL;
		return true;
	}
}
//更新节点：前提是找到节点,实际执行实际动作才返回true
//如果在主数组中，则修改只需要对应的结构成员即可
//对于链表方向的更新：也是修改其中的节点即可
template <typename Key,typename Value>
bool UBHashTable<Key,Value>::updateNode(const UBHashNode<Key,Value>& _limitEntity)
{
	UBHashNode<Key,Value>* mainP=ubHashTB,*linkP;
	int index=ubHash(_limitEntity.nodeKey);
	if((*(mainP+index)).nodeState==ubHashNode_Active)
	{
		if(ubSame(*(mainP+index),_limitEntity))
		{
			(*(mainP+index)).nodeKey=_limitEntity.nodeKey;
			(*(mainP+index)).nodeValue=_limitEntity.nodeValue;
			return true;
		}	
		linkP=mainP+index;
		while(linkP->next != NULL)
		{
			if(ubSame(*(linkP->next),_limitEntity))
			{
				(*(linkP->next)).nodeKey=_limitEntity.nodeKey;
				(*(linkP->next)).nodeValue=_limitEntity.nodeValue;
				return true;
			}
			linkP=linkP->next;
		}
	}
	return false;
}
//删除节点：实际执行删除动作返回true
//如果删除主数组中的元素：找到链表方向的最后一个元素，交换数值，并删除最后一个元素
//			  如果没有，则直接设置nodeState
//如果删除链表中的元素，则用两个指针执行
template <typename Key,typename Value>
bool UBHashTable<Key,Value>::deleteNode(const UBHashNode<Key,Value>& _limitEntity)
{
	UBHashNode<Key,Value>* mainP=ubHashTB,*linkP,*linkQ;
	int index=ubHash(_limitEntity.nodeKey);
	if((*(mainP+index)).nodeState==ubHashNode_Active)
	{
		if(ubSame(*(mainP+index),_limitEntity))
		{
			linkQ=mainP+index;
			linkP=linkQ->next;
			if(linkP == NULL)
				linkQ->nodeState=ubHashNode_Deleted;
			else 
			{
				while(linkP->next != NULL)
				{
					linkQ=linkP;
					linkP=linkP->next;
				}
				(*(mainP+index)).nodeKey=linkP->nodeKey;
				(*(mainP+index)).nodeValue=linkP->nodeValue;
				linkQ->next=NULL;
				delete linkP;
			}
			return true;
		}	
		linkQ=mainP+index;
		linkP=linkQ->next;
		if(linkP == NULL)
			return false;
		while(linkP != NULL)
		{
			if(ubSame(*linkP,_limitEntity))
			{
				linkQ->next=linkP->next;
				delete linkP;
				return true;
			}
			linkQ=linkP;
			linkP=linkP->next;
		}
	}
	return false;
}
template <typename Key,typename Value>
void UBHashTable<Key,Value>::clearHashTable()
{
	UBHashNode<Key,Value> *mainP,*linkP,*linkQ;
	mainP=ubHashTB;
	for(int i=0;i<ubHashTBCapacity;i++)
	{
		(mainP+i)->nodeState=ubHashNode_UnActive;
		linkP=(mainP+i)->next;
		while(linkP != NULL)
		{
			linkQ=linkP->next;
			delete linkP;
			linkP=linkQ;
			linkQ=linkP->next;
		}
	}
}
//返回容量，即槽的数量，不会包含的链表的中的数目
template <typename Key,typename Value>
int UBHashTable<Key,Value>::capacity() const
{
	return this->ubHashTBCapacity;
}
//返回活动节点的数目
template <typename Key,typename Value>
int UBHashTable<Key,Value>::activeDataLength() const
{
	int counter=0;
	UBHashNode<Key,Value> *mainP=ubHashTB,*linkP;
	for(int i=0;i<ubHashTBCapacity;i++)
	{
		if((mainP+i)->nodeState == ubHashNode_Active)
			++counter;
		linkP=(mainP+i)->next;
		while(linkP != NULL)
		{
			if(linkP->nodeState == ubHashNode_Active)
				++counter;
			linkP=linkP->next;
		}
	}
	return counter;
}
//返回未占用过的槽
template <typename Key,typename Value>
int UBHashTable<Key,Value>::unActiveDataLength() const
{
	int counter=0;
	UBHashNode<Key,Value> *mainP=ubHashTB,*linkP;
	for(int i=0;i<ubHashTBCapacity;i++)
	{
		if((mainP+i)->nodeState == ubHashNode_UnActive)
			++counter;
		linkP=(mainP+i)->next;
		while(linkP != NULL)
		{
			if(linkP->nodeState == ubHashNode_UnActive)
				++counter;
			linkP=linkP->next;
		}
	}
	return counter;
}
//返回删除过的槽
template <typename Key,typename Value>
int UBHashTable<Key,Value>::deletedDataLength() const
{
	int counter=0;
	UBHashNode<Key,Value> *mainP=ubHashTB,*linkP;
	for(int i=0;i<ubHashTBCapacity;i++)
	{
		if((mainP+i)->nodeState == ubHashNode_Deleted)
			++counter;
		linkP=(mainP+i)->next;
		while(linkP != NULL)
		{
			if(linkP->nodeState == ubHashNode_Deleted)
				++counter;
			linkP=linkP->next;
		}
	}
	return counter;
}
#endif
