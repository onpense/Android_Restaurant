/*----------------------------------------------------------------------------
 * Name:    send_request.c
 * Purpose: Send the request to the server
 * Version: v1.0
 * Note(s): 
 *----------------------------------------------------------------------------
 * Copyright (c) 2013 
 *----------------------------------------------------------------------------
 * History:
 *          v1.0 Initial Version
 *----------------------------------------------------------------------------*/

#include <send_request.h>

int get_command(void)
{
	int com;

	scanf("%d", &com);
	CLEAR_BUF;

	return com;
}

int get_orderid(void *buf)
{
	printf("obtain order!\n");
	sprintf(buf, "{\"orderid\":0,\"deskid\":8,\"carte\":[{\"id\":1,\"number\":1},{\"id\":2,\"number\":10}],\"total number\":\"2\"}");

}


void *send_request(void *args)
{
	int i = 0;
	int com;
	int len = 0;
	char buf[BUFLEN];
	char json_buf[JSONSIZE];

	ENCAP_REQ(json_buf, (char *)args, TABLE);
	Printf("%s", json_buf);

	/*Send:The table information
	  ****************************/
usr_send:
	if(send_msg(json_buf, JSONSIZE, NORMAL_DATA) < 0){
		if(SENDS > i++)
			goto usr_send;
		else{
			printf("Table failure!\n");
			pthread_exit(NULL);
		}
	}
	
	while(1){
		
		HELP;
		com = get_command();

		switch(com){
		case 1:
			send_msg(MENU, 1, NORMAL_DATA);

			usleep(5000);
			
			get_orderid(buf);
			ENCAP_REQ(json_buf, buf, ORDER);
			send_msg(json_buf, JSONSIZE, NORMAL_DATA);
			break;

		case 2:
			send_msg(REMIN, 1, NORMAL_DATA);
			break;

		case 3:
			send_msg(COUNT, 1, NORMAL_DATA);
			break;

		default:
			break;

		}
	}

}


