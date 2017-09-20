/*----------------------------------------------------------------------------
 * Name:    func_process.c
 * Purpose: Receive customer send data and put it in the queue
 * Version: v1.0
 * Note(s): 
 *----------------------------------------------------------------------------
 * Copyright (c) 2013 
 *----------------------------------------------------------------------------
 * History:
 *          v1.0 Initial Version
 *----------------------------------------------------------------------------*/
#include "overall.h"
#include <reciev.h>

int fds[DESKNUM];

int fd_getnext(int n)
{
	return fds[n];
}

void release_unusable(int fd)
{
	int i;
	struct list_head *pos, *n;
	order_t *tmp_ord;

	for(i=0; i<DESKNUM; i++){
		if(fd == fds[i]){
			fds[i] = 0;
			break;
		}
	}
	
	for(i=0; i<DESKNUM; i++){
		if(fd == desk[i].use_fd){
			list_del_init(&(desk[i].usr->u_list));
			SAFE_FREE(desk[i].usr);
			break;
		}
	}

/*
 *问题:当客户端意外退出，其订单还能保存吗
 *            当客户端重启还能找到之前的订单吗
 *            当
 */
	list_for_each_safe(pos, n, &(desk[i].order.o_list)){
		tmp_ord = list_entry(pos, order_t, o_list);
		list_del_init(pos);
		SAFE_FREE(tmp_ord);
	}

	close(fd);
	init_zero(&desk[i].use_fd, sizeof(int));
	pthread_mutex_destroy(&(desk[i].o_mutx));//销毁互斥锁

}

/*----------------------------------------------------------------------------
  set all clinet's fd into fdset
 *----------------------------------------------------------------------------*/
void recv_setfds(fd_set *fds, int *nfds)
{
	int i = 0;
	int fd = 0;
	
	while(i < DESKNUM){
		if(fd_getnext(i))
			fd = fd_getnext(i);
		i++;

		FD_SET(fd, fds);
		if(*nfds <= fd){
			*nfds = fd + 1;
		}
	}

	return;
}

/*----------------------------------------------------------------------------
  accept a new connect
 *----------------------------------------------------------------------------*/
void recv_new_connect(int sockfd)
{
	int i;
	int newfd;
	newfd = Accept(sockfd, NULL, NULL);
	Printf("A new connect:%d!\n", newfd);

	for(i=0; i<DESKNUM; i++){
		if(fds[i] == 0){
			fds[i] = newfd;
			break;
		}
	}

	return;
}

/*----------------------------------------------------------------------------
  set all clinet's fd into fdset
 *----------------------------------------------------------------------------*/
void recv_new_packets(fd_set rfds)
{
	int ret = -1;
	int i = 0;
	int fd = 0;
	char json_buf[JSONSIZE];
	char uname[NAMESIZ];

	while(0 != (fd = fd_getnext(i))){
		i++;

		if(FD_ISSET(fd, &rfds)){
			
			if(Recv(fd, json_buf, JSONSIZE, 0) == 0){	
				get_uname_by_fd(uname, fd, desk);
				printf("client<%s> logout!\n", uname);
				release_unusable(fd);
			}
			else{
				switch(json_buf[0]){
				case 't':
					desk_process(json_buf, fd);
					break;
				case 'm':
					menu_process(fd);
					break;
				case 'r':
					remind_process(fd);
					break;
				case 'c':
					count_process(fd);
					break;
				case 'o':
					order_process(json_buf+1, fd);
					break;

				}
				
			}
		}
	}

	return;
}


/*----------------------------------------------------------------------------
  A thread entry, receive local port from main thread to create a socket,
  then wait a connect or receive pakcets. 
 *----------------------------------------------------------------------------*/
void *func_reciev(void *args)
{
	int fd = *(int *)args;
	int nfds;
	fd_set readfds;

	while(1){
		nfds = fd + 1;
	
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);
		recv_setfds(&readfds, &nfds);
		
		Select(nfds, &readfds, NULL, NULL, NULL);

		if(FD_ISSET(fd, &readfds)){
			recv_new_connect(fd);
			continue;
		}

		recv_new_packets(readfds);
	}

	return;

}




