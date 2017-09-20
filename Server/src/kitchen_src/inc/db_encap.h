#ifndef __ENCAPSULATE_DBASE_H__
#define __ENCAPSULATE_DBASE_H__

#include <sqlite3.h>
#include <stdio.h>
#include <stdbool.h>
#include "overall.h"

#define USR_COLUMN   "uid int, uname varchar(30) primary key not null, deskid int, repst_time varchar(20), uc_id int, uc_state char"
#define INVENTORY_COLUMN "dishid int primary key not null, dishname varchar(30), price int, storage int, type int, recommend int"
#define COUNT_COLUMN "uc_id int, dishid int, dishname varchar(30), dishprice int, dishnum int, all_price int, cookerid int, booktime varchar(20), donetime varchar(20), oth_info, text"

#define DB_FILE "ksystem.db"
#define USER_TABLE "usr_tbl"                           //uesr table
#define INVENTORY_TABLE "inventory_tbl"        //inventory table
#define COUNT_TABLE "count_tbl"                     //count table

static char *errmsg;

#define DB_GETERR(ret)  do{\
		if(ret != SQLITE_OK){\
			printf("error:%s\n", errmsg);\
			return -1;\
		}\
}while(0)

extern sqlite3 *db_open_file(const char *path_name);
extern void init_dbase(void);
extern void create_db_tbl(sqlite3 *p_db, char *name_tbl, char *column_define);
extern bool db_is_table_exist(sqlite3 *p_db, const char *name_tbl);
extern int init_inventory_tbl(sqlite3 *p_db, char *name_tbl, 
		int dishid,char *dishname,int price, int storage, int type, int recommand);
/*将用户信息插入用户表*/
extern int db_insert_userinfo(const char *name_tbl, usr_t *userinfo);
/*根据sql查询得到结果*/
extern int db_get_table(char *sql, char ***db_result, int *nrow, int *ncolumn);
extern int db_excute_sql(char * sql);  //执行一条sql语句
/*从库存表从得到原材料充足的菜品*/
extern int db_order_create(order_t *order);
/*把处理完的订单插入账单表生成账单*/
extern void db_order_insert_counttbl(order_t *fin_ord, int cookerid);


#endif
