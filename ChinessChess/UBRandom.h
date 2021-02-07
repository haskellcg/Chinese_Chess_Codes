#ifndef UBUNTU_RANDOM_H
#define UBUNTU_RANDOM_H
#include "UBTypeDef.h"
#include <ctime>
//生成随机数，需要设置种子
//线性同余法，由于使用了每次重新生成A、c，所以种子一样因为不会重复
//但是Square需要设置随即种子，否则每次生成对象生成序列一样
class UBRandom
{
	private:
		uint64_ub seed;
		uint32_ub result;
		uint32_ub random_A;
		uint32_ub random_C;
	public:
		UBRandom();
		UBRandom(uint32_ub _seed);
		void setSeed(uint32_ub _seed);
		//线性同余法
		uint32_ub randomLinear();
		//线性同余法改进--人子映射
		uint32_ub randomLinearBranch();
		//线性同余法--平方取中法
		uint32_ub randomSquareMiddle();
		uint64_ub randomLinear64();
		uint64_ub randomLinearBranch64();
		uint64_ub randomSquareMiddle64();
		//对于16位、以及8位的只需要取模即可
		//Example:
		//8:
		//	randomLinear()%64;
};
const uint32_ub RANDOM_A=1220703125;
const uint32_ub RANDOM_C=394967277;
const uint32_ub MAXINT32=4294967296-1;
#endif
