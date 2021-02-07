#include "UBRandom.h"
UBRandom::UBRandom():result(0)
{
	setSeed(0);
	random_A=RANDOM_A-((uint32_ub)time(0))/2;
	random_C=RANDOM_C-((uint32_ub)time(0))/2;
}
UBRandom::UBRandom(uint32_ub _seed):result(0)
{
	setSeed(_seed);
	random_A=RANDOM_A-((uint32_ub)time(0))/2;
	random_C=RANDOM_C-((uint32_ub)time(0))/2;
}
void UBRandom::setSeed(uint32_ub _seed)
{
	if(_seed==0)
	{
		uint32_ub now=(uint32_ub)time(0);
		seed=((uint64_ub)now)*now;
	}
	else
	{
		seed=((uint64_ub)_seed)*_seed;
	}
}
uint32_ub UBRandom::randomLinear()
{
	if(result==0)
		result=(uint32_ub)(seed%MAXINT32);
	result=(uint32_ub)(((uint64_ub)result)*random_A+random_C)%MAXINT32;
	return result;
}
uint32_ub UBRandom::randomLinearBranch()
{
	if(result==0)
		result=(uint32_ub)(seed%MAXINT32);
	result=(uint32_ub)(((uint64_ub)result)*random_A+random_C)%MAXINT32;
	if(result < MAXINT32/2)
		result=2*result;
	else
		result=2*(MAXINT32-result)+1;
	return result;
}
uint32_ub UBRandom::randomSquareMiddle()
{
	if(result==0)
		result=(uint32_ub)(seed%MAXINT32);
	uint64_ub temp=((uint64_ub)result)*result;
	if(temp<10000)
		return  (uint32_ub)(result*((uint64_ub)time(0)));
	else
	{
		temp=temp/10000;
		temp=temp%10000000000000;
		return (uint32_ub)temp;
	}
}
uint64_ub UBRandom::randomLinear64()
{
	return randomLinear() ^ (((uint64_ub)randomLinear()) << 15) ^ (((uint64_ub)(randomLinear())) << 30);
}
uint64_ub UBRandom::randomLinearBranch64()
{
	return randomLinearBranch() ^ (((uint64_ub)randomLinearBranch()) << 15) ^ (((uint64_ub)(randomLinearBranch())) << 30);
}
uint64_ub UBRandom::randomSquareMiddle64()
{
	return randomSquareMiddle() ^ (((uint64_ub)randomSquareMiddle()) << 15) ^ (((uint64_ub)(randomSquareMiddle())) << 30);
}
