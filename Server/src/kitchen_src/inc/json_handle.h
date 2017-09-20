
#ifndef __JSON_HANDLE_H__
#define __JSON_HANDLE_H__

#include "overall.h"
#include </usr/local/include/json/json.h>

#define JSON_GET_STRING(obj, tmp_obj, name, str) do{\
		tmp_obj = json_object_object_get(obj, name);\
		strcpy((*(str)), json_object_get_string(tmp_obj));\
	}while(0)

#define JSON_GET_INT(obj, tmp_obj, name, intrg) do{\
		tmp_obj = json_object_object_get(obj, name);\
		(*(intrg)) = json_object_get_int(tmp_obj);\
	}while(0)

#define JSON_GET_ARRAY(obj, arr_obj, arr,index) do{\
	(*(arr_obj)) = json_object_object_get(obj, arr);\
	(*(arr_obj)) = json_object_array_get_idx((*(arr_obj)), index);\
}while(0)


/*�����û���Ϣ*/
extern int data_json_to_userinfo(char *json_str, int fd, usr_t *userinfo);

/*����json��ʽ�Ķ�������*/
extern void data_json_array_create(char *json_buf, const char *arr_name, char **db_result, int nrow, int ncolumn);
/**/
extern void data_json_menu_create(const char *name_tbl, char *json_buf);

extern void data_json_get_orderid(const char *json_str, order_t *order, int index);
extern void data_json_order_analysis(pend_t  pend,int *type, int *total_num, int *deskid);
extern void data_json_obj_create(char *json_buf, const char *arr_name, char **db_result);
extern void data_json_checkout_create(const char *name_tbl, char *json_buf);


#endif

