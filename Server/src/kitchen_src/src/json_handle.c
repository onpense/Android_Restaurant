#include "json_handle.h"

#if 0 
int json_get_int(struct json_object *obj, struct json_object *tmp_obj, const char *name)
{
	tmp_obj = json_object_object_get(obj, name);

	return json_object_get_int(tmp_obj);
}

const char *json_get_string(struct json_object *obj, struct json_object *tmp_obj, const char *name)
{
	tmp_obj = json_object_object_get(obj, name);

	return json_object_get_string(tmp_obj);
}
#endif

int data_json_to_userinfo(char *json_str, int fd, usr_t *userinfo)
{
	struct json_object *usr_obj;
	struct json_object  *tmp_obj;
	
	usr_obj = json_tokener_parse(json_str);/*��json�ַ���������json ����*/
	Printf("json_str = %s", json_str);

	/*��ȡ�û�id*/
	JSON_GET_INT(usr_obj, tmp_obj, "uid", &(userinfo->uid));
	Printf("uid = %d", userinfo->uid);
	
	/*��ȡ�û���*/
	JSON_GET_STRING(usr_obj, tmp_obj, "uname", &(userinfo->uname));
	Printf("uname = %s", userinfo->uname);
	
	/*��ȡ�û���������*/
	JSON_GET_INT(usr_obj, tmp_obj, "deskid", &(userinfo->deskid));
	Printf("deskid = %d", userinfo->deskid);
	
	userinfo->fd = fd;

	/*�ͷ�json������Դ*/
	json_object_put(usr_obj);
	json_object_put(tmp_obj);
	return 1;

}

void data_json_array_create(char *json_buf, const char *arr_name, char **db_result, int nrow, int ncolumn)
{
	int i, j;
	int index;
	struct json_object *my_array, *my_object, *new_obj;
	
	new_obj = json_object_new_object();
	my_array = json_object_new_array(); /*��������json����ֵ��������*/

	index = ncolumn;
	for(i=0; i<nrow; i++){
		my_object = json_object_new_object();
			
		for(j=0; j<ncolumn; j++){
			json_object_object_add(my_object, db_result[j], \
				json_object_new_string(db_result[index]));
			++index;
		}
			
		json_object_array_add(my_array, my_object);
	}
	json_object_object_add(new_obj, arr_name, my_array);
	strcpy(json_buf, json_object_to_json_string(new_obj));

	json_object_put(my_object);
	json_object_put(new_obj);
	json_object_put(my_array);

}

void data_json_menu_create(const char *name_tbl, char *json_buf)
{
	char **db_result;
	int nrow, ncolumn;
	char sql_buf[SQL_STRING_LEN];
	 
	sprintf(sql_buf, "select * from  %s", name_tbl);
	db_get_table(sql_buf, &db_result, &nrow, &ncolumn);

	data_json_array_create(json_buf, "carte", db_result, nrow, ncolumn);
	Printf("json_buf:%s\n", json_buf);

	return;
}

void data_json_get_orderid(const char *json_str, order_t *order, int index)
{
	struct json_object *ord_obj;
	struct json_object *tmp_obj;
	struct json_object *arr_obj;
	
	/*��json�ַ���������json ����*/
	ord_obj = json_tokener_parse(json_str);

	JSON_GET_ARRAY(ord_obj, &arr_obj, "carte", index);   //��ȡ��i����Ա
	JSON_GET_INT(arr_obj, tmp_obj, "id", &(order->dishid));
	JSON_GET_INT(arr_obj, tmp_obj, "number", &(order->dishnum));

	json_object_put(ord_obj);
	json_object_put(tmp_obj);
	json_object_put(arr_obj);

}

void data_json_order_analysis(pend_t  pend,int *type, int *total_num, int *deskid)
{
	struct json_object *ord_obj;
	struct json_object *tmp_obj;
	
	/*��json�ַ���������json ����*/
	ord_obj = json_tokener_parse(pend.json_buf);

	/*��ȡ��������*/
	JSON_GET_INT(ord_obj, tmp_obj, "orderid", type);
	/*��ȡ����*/
	JSON_GET_INT(ord_obj, tmp_obj, "deskid", deskid);
	/*��ȡ������������*/
	JSON_GET_INT(ord_obj, tmp_obj, "total number", total_num); 

	json_object_put(ord_obj);
	json_object_put(tmp_obj);
}

void data_json_obj_create(char *json_buf, const char *arr_name, char **db_result)
{
	struct json_object *obj;
	
	/*��json�ַ���������json ����*/
	obj = json_tokener_parse(json_buf);

	json_object_object_add(obj, arr_name, \
				json_object_new_string(db_result[1]));

	init_zero(json_buf, JSONSIZE);
	strcpy(json_buf, json_object_to_json_string(obj));

	json_object_put(obj);

}

/*data_json_array_create()
 * data_json_obj_create()
 *��������������װ�д��Ľ�
 *����data_json_obj_create()��������Ҫ��json_buf�ַ������ж�
 *(�Ƿ�ֻ��Ҫ�½�json����?
 *���ǰ�json_buf��json������½���json����ϲ�?)
 */
void data_json_checkout_create(const char *name_tbl, char *json_buf)
{
	char **db_result = NULL;
	int nrow, ncolumn;
	char sql_buf[SQL_STRING_LEN];
	 
	sprintf(sql_buf, \
		"select dishid, dishname, dishprice, dishnum, all_price from  %s", name_tbl);
	db_get_table(sql_buf, &db_result, &nrow, &ncolumn);

	if(db_result[ncolumn] != NULL){
		/*����json��ʽ���˵����������Ա*/
		data_json_array_create(json_buf, "carte", db_result, nrow, ncolumn);
		Printf("json_buf:%s\n", json_buf);
	}

	sprintf(sql_buf, \
		"select sum(all_price) from  %s", name_tbl);
	db_get_table(sql_buf, &db_result, &nrow, &ncolumn);

	if(db_result[ncolumn] != NULL){
		/*����json��ʽ���˵�����ֵ��Ա*/
		data_json_obj_create(json_buf, "total", db_result);
		Printf("json_buf:%s\n", json_buf);
	}
}

