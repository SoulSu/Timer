#include <stdio.h>

#include "tools.h"


// 开启调试模式
// #define DEBUG 1

#ifndef DEBUG
#define DEBUG_LOG(args...) printf(args)
#else
#define DEBUG_LOG(args...) do{} while (false)
#endif

char *hash(const char *name, long t)
{
	char *_hash_result = "1111";
	return _hash_result;
}