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
  *餐厅的每个餐桌都配备点菜终端，终端机的编号和桌号匹配，客户可以根据终端机界面提示进行点餐；也可以支持客户通过智能手机安装本餐厅点餐软件进行。
  *此客户端模拟的是固定在餐桌上的终端机，所以终端机的桌号和用户名已经固定
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

	//设置分离属性
	//fix me ...

	//创建线程
	pthread_create(&snd_th, NULL, send_request, usr_buf);
	pthread_create(&rcv_th, NULL, rcv_feedback, NULL);

	while(1);

	return 0;
}


