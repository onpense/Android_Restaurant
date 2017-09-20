#include "req_dispose.h"

int get_pend_node(pend_t  *pend)
{
	pend_t *tmp_pnd;

	sem_wait(&pnd_non_empty);
	pthread_mutex_lock(&pnd_mutex);
	Printf("get node");

	if(list_empty(&pnd_head.p_list)){
		sem_post(&pnd_non_full); 
		pthread_mutex_unlock(&pnd_mutex);
		
		return FAIL;
	}
	else{
		tmp_pnd = list_entry(pnd_head.p_list.prev, pend_t, p_list);
		memcpy(pend, tmp_pnd, sizeof(pend_t));
		
		list_del_init(&(tmp_pnd->p_list));
		free(tmp_pnd);
		
		sem_post(&pnd_non_full); 
		pthread_mutex_unlock(&pnd_mutex);
		return OK;
	}


}

void set_time(char *timer)
{
	/*获取系统时间*/
}

int new_order_process(const char *json_str, int index, int deskid)
{
	int ret = -1;
	char buf[BACKSIZE];
	order_t *order;

	order = (order_t *)Malloc(sizeof(order_t));
	data_json_get_orderid(json_str, order, index);//获取订单中的菜品编号和数量
	ret = db_order_create(order);   //从库存表从得到原材料充足的菜品
	set_time(order->booktime);
	order->flags = 'n';
	order->all_price = (order->dishprice)*(order->dishnum);
	order->uc_id = deskid;//客户桌号作为用户账单号

	if(ret == OK){
		pthread_mutex_lock(&(desk[deskid].o_mutx));
		list_add(&(order->o_list), &(desk[deskid].order.o_list));//将客户所点菜品插入对应的桌号结构体，待厨师处理
		desk[deskid].usr->uo_state = 'd';   //标志客户已经下单
		pthread_mutex_unlock(&(desk[deskid].o_mutx));
		return OK;
	}
	else{
		sprintf(buf, "{\"result\":\"0\", \"dishid\":\"%d\"}", order->dishid);
		send_msg(buf, BACKSIZE, 0, desk[deskid].use_fd);
		SAFE_FREE(order);
		return FAIL;
	}

}

void show_usr_order(int deskid)
{
	struct list_head *pos, *n;
	struct order_t *tmp_ord;

	if(!list_empty(&(desk[deskid].order.o_list))){
		printf("There are new orders of customer deskid is %d!\n"
			"Please input deskid process order\n", deskid);
		printf("\ndishname\t\tdishnum\n");
		list_for_each_safe(pos, n, &(desk[deskid].order.o_list)){
			tmp_ord = list_entry(pos, order_t, o_list);
			printf("%s\t\t%d\n", tmp_ord->dishname, tmp_ord->dishnum);
		}
	}
}

 /*
  * 0: 表示新订单
  * 1：增加订单
  * 2：删除订单
  * 3：查询订单
  * 其他值： 订单编号
  */
int order_type_process(const char *json_str, int type, int num, int deskid)
{
	int ret = -1;
	int index;

	switch(type){
		case 0:
			for(index=0; index<num; index++)
				ret = new_order_process(json_str, index, deskid);
			show_usr_order(deskid);
			break;
		case 1:
			//ret = add_order_process();
			break;
		case 2:
			//ret = del_order_process();
			break;
		case 3:
			//ret = query_order_process();
			break;
		default:
			printf("Unable to identify your order type!\n");
			break;
	}

	return ret;
}


int desk_process(char *data, int fd)
{
	pthread_t tid;
	char buf[BACKSIZE];
	req_data *trs_data;
	pthread_attr_t attr;

	trs_data = (req_data *)Malloc(sizeof(req_data));
	init_zero(trs_data, sizeof(req_data));
	copy_data(trs_data, fd, data);

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	/*创建分离线程，初始化餐桌结构体*/
	if(pthread_create(&tid, &attr, desk_init, trs_data) < 0){
		sprintf(buf, "The self-made attempt failed!\n");
		send_msg(buf, BACKSIZE, 0, fd);
		free(trs_data);
		return FAIL;
	}
	else{
		usleep(5000);
		Printf("create a pthread process desk init!\n");
		return OK;
	}

}

int menu_process(int fd)
{
	pthread_t tid;
	char buf[BACKSIZE];
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	/*创建分离线程，读取库存表，将菜谱信息格式化成json格式发送给客户端*/
	if(pthread_create(&tid, &attr, build_menu, &fd) < 0){
		sprintf(buf, "Read the menu data failed!\n");
		send_msg(buf, BACKSIZE, 0, fd);
		return FAIL;
	}
	else{
		usleep(5000);
		return OK;
	}
}

int remind_process(int fd)
{
	char buf[BACKSIZE];
	char uname[NAMESIZ];

	sprintf(buf, "{\"result\":\"1\"}");
	send_msg(buf, BACKSIZE, 0, fd);

	get_uname_by_fd(uname, fd, desk);	
	printf("Customer %s undelivered!\n", uname);

	return OK;
}


int count_process(int fd)
{
	pthread_t tid;
	char buf[BACKSIZE];
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	/*创建分离线程，读取账单表，将客户账单信息格式化成json格式发送给客户端*/
	if(pthread_create(&tid, &attr, build_bill, &fd) < 0){
		sprintf(buf, "Failed to get the bill!\n");
		send_msg(buf, BACKSIZE, 0, fd);
		return FAIL;
	}
	else{
		usleep(5000);
		return OK;
	}
}

void insert_pending(pend_t *pend)
{
	/*------------------
	   insert message queue
	   ------------------*/
	sem_wait(&pnd_non_full);
	pthread_mutex_lock(&pnd_mutex);

	list_add(&pend->p_list, &pnd_head.p_list);

	sem_post(&pnd_non_empty);
	pthread_mutex_unlock(&pnd_mutex);	
	Printf("\n\n\t\tinsert msg ok!\n\n");
}

int get_orderid(int *orderid, int fd)
{
	struct list_head *pos, *n;
	usr_t *tmp_usr;

	/*从用户链表里找到该用户的桌号，将此桌号作为该客户的订单号*/
	list_for_each_safe(pos, n, &usr_head.u_list){
		tmp_usr = list_entry(pos, usr_t, u_list);
		if(tmp_usr->fd == fd){
			*orderid = tmp_usr->deskid;
			return OK;
		}
	}

	*orderid = -1;//当处理客户订单时，订单号为-1说明出错，反馈订单出错信息给客户并丢弃该订单
	return *orderid;
}

int order_process(char *data, int fd)
{
	pend_t *pend;
	int ret = -1;

	pend = (pend_t *)Malloc(sizeof(pend_t));
	init_zero(pend, sizeof(pend_t));
	strncpy(pend->json_buf, data, JSONSIZE);
	pend->fd = fd;
	/*桌号作为客户的订单号*/
	ret = get_orderid(&pend->uc_id, fd);
	if(ret == OK){
		/*将json格式订单插入待处理队列*/
		insert_pending(pend);
		Printf("pend->json_buf:%s\n", pend->json_buf);
		return OK;
	}

	return FAIL;
}

void usrinfo_fill(req_data *trs_d, usr_t *userinfo)
{
	Printf("usrinfo:%s", trs_d->json_buf);

	data_json_to_userinfo(trs_d->json_buf+1, trs_d->fd, userinfo);
	userinfo->uo_state = 'n';//表示客户未下单
	userinfo->uc_state = 'n';//表示客户未结帐
	set_time(userinfo->repst_time);
}

void usrinfo_init(usr_t *userinfo)
{
	pthread_mutex_lock(&usr_mutex);
	list_add(&userinfo->u_list, &usr_head.u_list);
	pthread_mutex_unlock(&usr_mutex);

	db_insert_userinfo(USER_TABLE, userinfo);
}

int deskinfo_fill(desk_t *deski, usr_t *userinfo)
{
	if(desk[userinfo->deskid].use_fd == 0){
		pthread_mutex_init(&(desk[userinfo->deskid].o_mutx), 0);  //管理订单结点锁	
		desk[userinfo->deskid].usr = userinfo;
		desk[userinfo->deskid].order.uc_id = userinfo->deskid;
		desk[userinfo->deskid].use_fd = userinfo->fd;
		return OK;
	}
	else
		return FAIL;

}

/*-------------------------------------------------------------------------
  *线程快速响应客户端请求
  *-----------------------------------------------------------------------*/

void *desk_init(void *args)//入座处理线程
{
	req_data *trs_data = (req_data *)args;
	usr_t *usr;
	char buf[BACKSIZE];
	int ret = -1;

	usr = (usr_t *)Malloc(sizeof(usr_t));
	init_zero(usr, sizeof(usr_t));

	usrinfo_fill(trs_data, usr);
	ret = deskinfo_fill(desk, usr);
	if(ret == FAIL){
		sprintf(buf, "The self-made failed!\n");
		send_msg(buf, BACKSIZE, 0, trs_data->fd);
		/*释放用户结点并释放申请的空间*/
		SAFE_FREE(trs_data);
		SAFE_FREE(usr);
	}
	else if(ret == OK){
		usrinfo_init(usr);
		sprintf(buf, "The self-made successful!\n");
		send_msg(buf, BACKSIZE, 0, trs_data->fd);
		printf("Customer %s successful table table number %d!\n", usr->uname, usr->deskid);
		SAFE_FREE(trs_data);
	}
	
	pthread_exit(NULL);
}

void *build_menu(void *args)//菜谱处理线程
{
	int fd = *(int *)args;
	char buf[JSONSIZE] = {"init!\n"};
	char uname[NAMESIZ];

	data_json_menu_create(INVENTORY_TABLE, buf);
	get_uname_by_fd(uname, fd, desk);
Printf("menu fd:%d", fd);
	printf("Barack %s to send the latest recipes to customers!\n", uname);
	send_msg(buf, JSONSIZE, 0, fd);
}

void *build_bill(void *args)//结账处理线程
{
	int fd = *(int *)args;
	char json_buf[JSONSIZE];
	char uname[NAMESIZ];

	//init_zero(json_buf, JSONSIZE);
	//data_json_checkout_create(COUNT_TABLE, json_buf);
	init_zero(json_buf, JSONSIZE);
	sprintf(json_buf, "{\"result\":\"1\"}");
	send_msg(json_buf, JSONSIZE, 0, fd);

	get_uname_by_fd(uname, fd, desk);
	printf("Invoicing customer Mr %s!\n", uname);
}

/*----------------------------------------------------------------------
 从pending 队列中取出json格式订单数据处理
----------------------------------------------------------------------*/
void *func_process(void * args)
{
	char buf[JSONSIZE];
	int ret = -1;
	int type = -1;
	pend_t  pend;
	int total_num;
	int deskid;

	while(1){
		ret = get_pend_node(&pend);
		Printf("pend.deskid = %d pend.fd = %d pend.json_buf = %s ", pend.uc_id, pend.fd, pend.json_buf);
		if(ret == OK){
			data_json_order_analysis(pend, &type, &total_num, &deskid);
			
			if(order_type_process(pend.json_buf, type, total_num, deskid) == OK){
				sprintf(buf, "{\"result\":\"%d\", \"orderid\":\"%d\"}", OK, pend.uc_id);
				Printf("order process ok!\n");
				send_msg(buf, JSONSIZE, 0, pend.fd);
			}
		}
		else{
			printf("get pending node failed!\n");
			continue;
		}
	}
}

int cook_order(int deskid)
{
	struct list_head *pos, *n;
	order_t *tmp_ord;

	if(list_empty(&(desk[deskid].order.o_list))){
		printf("Ramadhin for %d customers didn't order!\n", deskid);
		return FAIL;
	}
	
	pthread_mutex_lock(&(desk[deskid].o_mutx));
	list_for_each_safe(pos, n, &(desk[deskid].order.o_list)){
		list_del_init(pos);
		tmp_ord = list_entry(pos, order_t, o_list);
		set_time(tmp_ord->donetime);
		db_order_insert_counttbl(tmp_ord, 0);
		SAFE_FREE(tmp_ord);
	}
	pthread_mutex_unlock(&(desk[deskid].o_mutx));

	desk[deskid].usr->uo_state = 'o';  //该客户的订单烹饪完成
	printf("Ramadhin for %d customer orders to finish!\n", deskid);

	return OK;
}


