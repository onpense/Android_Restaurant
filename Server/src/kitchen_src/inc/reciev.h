#ifndef __RECIEV_H__
#define __RECIEV_H__

#include "sock_encap.h"

/* �Ӽ��������������ϲ��ϵĽ�������*/
extern void recv_new_packets(fd_set rfds);  
/*�µ���������*/
extern void recv_new_connect(int sockfd);
/*���ý���������������������*/
extern void recv_setfds(fd_set *fds, int *nfds);
extern int fd_getnext(int n);
/*���ղ���ʹ�õ���Դ*/
extern void release_unusable(int fd);


#endif
