#ifndef UBUNTU_TYPEDEF_H
#define UBUNTU_TYPEDEF_H
/*
这里需要处理64的整数，但是由于不同编译器对于其处理不同：
	gcc(mingw32) g++(mingw32) 允许 long long 和 _int64的方式，但是输出只支持%l64d 和 cout
	gcc(linux i386) g++(linux i386) 只允许long long ,输入只支持cout、lld/llu
	VC6.0 只允许_int64 ,输出只支持%l64d/%l64d
所以对于不同的平台，我们需要不同的处理，不同的定义方式
	#ifdef _WIN32
	typedef unsigned _int64 	uint64_t;
	#else
	typedef unsigned long long 	uint64_t;
	#endif
	typedef unsigned int 		uint32_t;
	typedef unsigned short 		uint16_t;
	typedef unsigned char 		uint8_t;
对于输出采用如下的方式
	#ifdef _WIN32
	printf("%l64d",x);
	#else
	printf("%lld",x);
	#endif
移植时：
	#ifdef _WIN32
	#define APR_UINT64_T_HEX_FMT "l64x"
	#else
	#define APR_UINT64_T_HEX_FMT "l64x"
	#endif
	ex:sprintf(buf,"%"APR_UINT64_T_HEX_FMT,var)
	#define HOST_WIDEST_INT_PRINT_DEC	"%l64d"
	#define HOST_WIDEST_INT_PRINT_UNSIGNED	"%l64u"
	#define HOST_WIDEST_INT_PRINT_HEX	"0x%l64x"
*/
#ifdef _WIN32
typedef unsigned _int64 uint64_ub;
#else
typedef unsigned long long uint64_ub;
#endif
typedef unsigned int uint32_ub;
typedef unsigned short uint16_ub;
typedef unsigned char uint8_ub;
#endif
