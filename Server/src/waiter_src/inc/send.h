
#ifndef __SEND_H__
#define __SEND_H__

#include <overall.h>
#include <message.h>
#include <encapsulation_sock.h>

#define URGENT_DATA  'U'     //��������(�����������ס��ߵ�������)
#define NORMAL_DATA  'N'     //�ͻ���Ϣ����������

extern int send_msg(char *buf, int len, char type);




#endif
