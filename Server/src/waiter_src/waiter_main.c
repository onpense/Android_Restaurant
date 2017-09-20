/*----------------------------------------------------------------------------
 * Name:    waiter_main.c
 * Purpose: waiter program main functions
 * Version: v1.0
 * Note(s):
 *----------------------------------------------------------------------------
 * Copyright (c) 2013 
 *----------------------------------------------------------------------------
 * History:
 *          v1.0 Initial Version
 *----------------------------------------------------------------------------*/

#include <send_request.h>
#include <message.h>
#include <encapsulation_sock.h>
#include <rcv_feedback.h>

#define  USRID     321
#define USRNAME "zdiv"
#define  DESKID    8

int local_fd;
static pthread_t snd_th, rcv_th;

/*
  *������ÿ���������䱸����նˣ��ն˻��ı�ź�����ƥ�䣬�ͻ����Ը����ն˻�������ʾ���е�ͣ�Ҳ����֧�ֿͻ�ͨ�������ֻ���װ���������������С�
  *�˿ͻ���ģ����ǹ̶��ڲ����ϵ��ն˻��������ն˻������ź��û����Ѿ��̶�
  */
static inline int get_usr_info(char *usrinfo)
{
	return sprintf(usrinfo, "{\"uid\":%d, \"uname\":\"%s\", \"deskid\":%d}", USRID, USRNAME, DESKID);
}

int main(void)
{
	char usr_buf[USRSIZE];

	local_fd = init_tcp_client(PORT);

	get_usr_info(usr_buf);

	//���÷�������
	//fix me ...

	//�����߳�
	pthread_create(&snd_th, NULL, send_request, usr_buf);
	pthread_create(&rcv_th, NULL, rcv_feedback, NULL);

	while(1);

	return 0;
}


