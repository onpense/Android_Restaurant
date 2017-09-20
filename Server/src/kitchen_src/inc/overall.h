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
#define DESKNUM                          100     //��������
#define NAMESIZ                           30       //���ֳ���
#define BUFLEN                             2048
#define JSONSIZE                          4096
#define BACKSIZE                          512
#define TYPESIZE                          20
#define  SQL_STRING_LEN             1024

//5.�û���Ϣ�ṹ��
    typedef struct usr_t{
        int uid;                                       //�û�id
        char uname[NAMESIZ];                           //�û���
        int deskid;                                 //����
        int fd;                                         //ͨ���׽���
        char uo_state;                           //����״̬
        char uc_state;                           //�˵�״̬
        char repst_time[NAMESIZ];          //�Ͳ�ʱ��
        int uc_id;                                  //�û��˵�id
        struct list_head u_list;
    }usr_t;


//6.�����ṹ��
    typedef struct order_t{
        int uc_id;                                  //�û��˵���
        int dishid;                                //��Ʒ���
        char dishname[NAMESIZ];                    //��Ʒ��
        int dishprice;                          //��Ʒ�۸�
        int dishnum;                            //��Ʒ����
        int all_price;                            //�����Ʒ�ܼ�
        char booktime[20];                    //�µ�ʱ��
        char donetime[20];                    //��ɶ���ʱ��
        char flags;                              //������־λ
        struct list_head o_list;
    }order_t;

//7.���������ṹ��
    typedef struct desk_t{
        usr_t *usr;                                //ָ���û���Ϣ�ṹ��
        order_t order;                        //ָ��ò����ͻ��Ķ�������
        pthread_mutex_t o_mutx;      //������������
        int use_fd;                              //�����Ƿ�ռ��     0��ʾδռ��     ����0��ʾռ�ã����ã�����ͻ����ӳɹ��󷵻ص�newfd��
    }desk_t;

//8.��������
    typedef struct pend_t{
        char json_buf[JSONSIZE];                     //json��ʽ���ݣ��˳������Ƕ�����
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


