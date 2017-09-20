#ifndef __OVERALL_H__
#define __OVERALL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "debug.h"
#include "dlist.h"

//#include <encapsulate_dbase.h>

#define OK                                    1
#define FAIL                                  -1

#define PORT                                10000
#define DESKNUM                          100     //餐桌数量
#define NAMESIZ                           30       //名字长度
#define BUFLEN                             2048
#define JSONSIZE                          4096
#define BACKSIZE                          512
#define TYPESIZE                          20
#define  SQL_STRING_LEN             1024

//5.用户信息结构体
    typedef struct usr_t{
        int uid;                                       //用户id
        char uname[NAMESIZ];                           //用户名
        int deskid;                                 //桌号
        int fd;                                         //通信套接字
        char uo_state;                           //订单状态
        char uc_state;                           //账单状态
        char repst_time[NAMESIZ];          //就餐时间
        int uc_id;                                  //用户账单id
        struct list_head u_list;
    }usr_t;


//6.订单结构体
    typedef struct order_t{
        int uc_id;                                  //用户账单号
        int dishid;                                //菜品编号
        char dishname[NAMESIZ];                    //菜品名
        int dishprice;                          //菜品价格
        int dishnum;                            //菜品数量
        int all_price;                            //所点菜品总价
        char booktime[20];                    //下单时间
        char donetime[20];                    //完成订单时间
        char flags;                              //订单标志位
        struct list_head o_list;
    }order_t;

//7.餐厅餐桌结构体
    typedef struct desk_t{
        usr_t *usr;                                //指向用户信息结构体
        order_t order;                        //指向该餐桌客户的订单链表
        pthread_mutex_t o_mutx;      //处理订单互斥锁
        int use_fd;                              //餐桌是否被占用     0表示未占用     大于0表示占用（复用：保存客户连接成功后返回的newfd）
    }desk_t;

//8.待处理结点
    typedef struct pend_t{
        char json_buf[JSONSIZE];                     //json格式数据（此程序里是订单）
        int uc_id;
	 int fd;
        struct list_head p_list;
    }pend_t;



#define SYS_ERR(err)  do{\
	perror(err);\
	exit(-1);\
}while(0)

#define SAFE_FREE(ptr) \
	do {	\
		if(ptr) { \
			free(ptr);	\
			ptr = NULL;	\
		}	\
	} while(0)

static inline void *Malloc(size_t size)
{
	void *ret;

	ret = malloc(size);
	if(ret == NULL){
		SYS_ERR("malloc");
	}

	return ret;
}

static inline void *init_zero(void *data, int len)
{
	return memset(data, 0, len);
}

static inline int get_uname_by_fd(char *name, int fd, desk_t *desk)
{
	int i;

	for(i=0; i<DESKNUM; i++){
		if(desk[i].use_fd == fd){
			strncpy(name, desk[i].usr->uname, NAMESIZ);
			return OK;
		}
	}

	return FAIL;
}

extern sem_t pnd_non_empty;
extern sem_t pnd_non_full;
extern sem_t order_non_empty;
extern sem_t order_non_full;
extern pthread_mutex_t pnd_mutex;
extern pthread_mutex_t usr_mutex;
extern pthread_mutex_t order_mutex;
extern desk_t desk[DESKNUM];
extern usr_t usr_head;
extern pend_t pnd_head;
extern int fds[DESKNUM];

extern void *func_reciev(void *args);
extern void *background(void *args);
extern void *func_process(void *args);


#endif


