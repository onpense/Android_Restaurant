#ifndef __REQ_DISPOSE_H__
#define __REQ_DISPOSE_H__

#include "overall.h"
#include "sock_encap.h"
#include "db_encap.h"
#include "json_handle.h"

#define copy_data(dst, fd, data) do{\
	memcpy(dst->json_buf, data, JSONSIZE);\
	dst->fd = fd;\
}while(0)

typedef struct req_data{
	int fd;
	char json_buf[JSONSIZE];
	void *private;
}req_data;

/*������������*/
extern int desk_process(char *data, int fd);
/*����ߵ�����*/
extern int remind_process(int fd);
/*�������������*/
extern int menu_process(int fd);
/*�����������*/
extern int count_process(int fd);
/*����������*/
extern int order_process(char *data, int fd);
extern int get_orderid(int *orderid, int fd);

/*(pending)����������������*/
extern void insert_pending(pend_t *pend);
extern void usrinfo_fill(req_data *trs_d, usr_t *userinfo);//����û��ṹ��
extern int deskinfo_fill(desk_t *deski, usr_t *userinfo);//�������ṹ��

extern void *desk_init(void *args);//���������߳�
extern void *build_menu(void *args);//���״����߳�
extern void *build_bill(void *args);//���˴����߳�
extern void set_time(char *timer);   //��ȡϵͳʱ��


/*����pending������json��ʽ��������*/
extern void *func_process(void * args);
extern int get_pend_node(pend_t  *pend);//��ȡpending���
extern int new_order_process(const char *json_str, int index, int deskid);/*�¶�������*/
extern void show_usr_order(int deskid);//��ʾ������������
/*���ݶ��������ж���ô������*/
extern int order_type_process(const char *json_str, int type, int num, int deskid);



/*��ʦ�̴߳�����*/
extern int cook_order(int deskid);


#endif

