/*----------------------------------------------------------------------------
 * Name:    main.c
 * Purpose: server program main functions
 * Version: v1.0
 * Note(s): Can handle the basic request of the clients
 		   This version has a lot of bugs, later will use spare time to solve it
 *----------------------------------------------------------------------------
 * Copyright (c) 2013 
 *----------------------------------------------------------------------------
 * History:
 *          v1.0 Initial Version
 *----------------------------------------------------------------------------*/
#include "json_handle.h"
#include "db_encap.h"
#include "sock_encap.h"

sem_t pnd_non_empty;
sem_t pnd_non_full;
sem_t order_non_empty;
sem_t order_non_full;
pthread_mutex_t pnd_mutex;
pthread_mutex_t usr_mutex;
pthread_mutex_t order_mutex;
static pthread_t th_rcv, th_proc, th_upd;

usr_t usr_head;
pend_t pnd_head;
desk_t desk[DESKNUM];


/**********************************************
  *On server! Create a TCP socket use to accept connect. 
  *********************************************/
int init_tcp_server(unsigned short port, int *fd)
{
	int on = 1;
	struct sockaddr_in local_addr;


	*fd = Socket(AF_INET, SOCK_STREAM, 0);

	setsockopt(*fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(port);
	Bind(*fd, &local_addr, sizeof(local_addr));

	Listen(*fd, DEFAULT_BACKLOG);

	return *fd;
}


/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
void init_list_head(void)
{
	int i;

	INIT_LIST_HEAD(&usr_head.u_list);
	INIT_LIST_HEAD(&pnd_head.p_list);
	for(i=0; i<DESKNUM; i++){
		INIT_LIST_HEAD(&(desk[i].order.o_list));
	}
}

void init_resous(void)
{
	/*��Ϣ������Դ����*/
	sem_init(&pnd_non_empty, 0, 0);
	sem_init(&pnd_non_full, 0, 100);

	/*����������Դ����*/
	sem_init(&order_non_empty, 0, 0);
	sem_init(&order_non_full, 0, 100);

	pthread_mutex_init(&usr_mutex, 0);    //�����û������
	pthread_mutex_init(&pnd_mutex, 0);  //������Ϣ�����
	pthread_mutex_init(&order_mutex, 0);   //�����������

	return;
}

/***************************************************************************************
***************************************************************************************/

int main(void)
{
	int fd;
	unsigned short local_port = PORT;
	char buf[BUFLEN];
	pthread_attr_t attr;

	init_zero(desk, sizeof(desk));        //��ʼ��������Ϣ
	init_zero(fds, sizeof(fds));            //��ʼ��ͨ��fd����
	init_list_head();                           //��ʼ���û��ʹ�������Ϣ����
	init_dbase();                               //��ʼ�����ݿ�
	init_tcp_server(local_port, &fd);    //��ʼ��TCP�����׽���
	init_resous();                              //��ʼ���ź����ͻ�����

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&th_rcv, &attr, func_reciev, &fd);
	pthread_create(&th_proc, &attr, func_process,  &fd);
	//pthread_create(&th_upd, NULL, background,  &fd);



	printf("\n\n\t\t***************\n\t\t*system start!*\n\t\t***************\n\n\n");

	while(1){

		printf("Pls input command:\n");
		if(fgets(buf, BUFLEN, stdin) < 0){
			continue;
		}

		if(!strncmp(buf, "t", 1)){
			Printf("Ready to process json!\n");
			//���Դ�������Ϣ�����Ƿ�ɹ�
			struct list_head *pos, *n;
			pend_t *tmp_pend;
			list_for_each_safe(pos, n, &pnd_head.p_list){
				tmp_pend = list_entry(pos, pend_t, p_list);
				Printf("\npend list json:%s\n", tmp_pend->json_buf);
			}
		}
		if(!strncmp(buf, "f", 1)){
			//���¿��
		}
		else{
			//������
			if(atoi(&buf[0]) >= 0 && atoi(&buf[0]) < DESKNUM){
				cook_order(atoi(&buf[0]));
			}
		}


	}

	return 0;
}
