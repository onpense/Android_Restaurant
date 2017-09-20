﻿
#ifndef __OVERALL_H__
#define __OVERALL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include <encapsulation_print.h>


#define PORT 10000

#define  USRSIZE     1024
#define JSONSIZE    4096
#define SENDS  8

/*��Ҫ������������Ӧ�ĸ�������*/
#define TABLE     "t"                               //����
#define MENU      "m"                             //������
#define ORDER    "o"                             //����
#define REMIN     "r"                              //�ߵ�
#define COUNT    "c"                              //����

#define CLEAR_BUF do{while(getchar() != '\n');}while(0)

#define SYS_ERR(err)  do{\
	perror(err);\
	exit(-1);\
}while(0)

#define HELP \
		printf("\n\tWelcome use smart order-solft\n\n\n");\
		printf("1.book order\n2.reminder\n3.checkout\n\nPls choice:\n")

static inline void *Malloc(size_t size)
{
	void *ret;

	ret = malloc(size);
	if(ret == NULL){
		perror("malloc");
		exit(-1);
	}

	return ret;
}

extern int local_fd;





#endif
