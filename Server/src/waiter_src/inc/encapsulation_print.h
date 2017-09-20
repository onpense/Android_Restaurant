#ifndef __ENCAPSULATION_PRINT_H__
#define __ENCAPSULATION_PRINT_H__

#include <stdarg.h>
#include <stdio.h>


#define __DEBUG__ 1
#if __DEBUG__

#define Printf(fmt,...)  do{printf("%s %d %s::",__FILE__, __LINE__, __func__); \
	printf(fmt,##__VA_ARGS__); printf("\n");}while(0)   
#define SLEEP(n) sleep(0)

#else

#define Printf(fmt,...)
#define SLEEP(n)

#endif

#endif

